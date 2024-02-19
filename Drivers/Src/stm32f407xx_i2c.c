/*										@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
										@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
										@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
										@@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@             @@@         %   @@
										@@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
										@@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@
										@ @@@@@@@@@@@@@@                  @@@@@@@@@@@@@@
										@@  @@@@@@@@@                  @@@@@@@@@@@@@@@@@
										@@@@  @@@    @@@@@@@&         .@@@@@@@@@@@@@@@@@
										@@@@@@@#   ###@@@@( @        &@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@#     @@     (@     @@@@@@@@@@@@@
										@@@@@@@@@@@@@@     @@@@     @@     @@@@@@@@@@@@@
										@@@@@@@@@@@&     @@@@@@/   @@@@@@    @@@@@@@@@@@
										@@@@@@@@@@@*    @@@@@@@@  @@@@@@@@      @@@@@@@@
										@@@@@@@@@@@      @@@@@@@  @@@@@@@@   %  @@@@@@@@
										@@@@@@@@@@@@       /&@@@  @@@@@@&   @ @@@@@@@@@@
										@@@@@@@@@@@@@@&  ,@@@@@@@@@@@@  @ @@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@@  @@@@@@@@@@@%@@@@@@@@@@@@@@@@													*/

#include <stm32f407xx_i2c.h>

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle); //needed to release SCL after writing

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle );
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle );


static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1.bit.start = SET;
}

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	//clear 0th bit because 0th bit is 'read/write' bit and '0' stands for 'write'
	SlaveAddr &= ~(1);
	pI2Cx->DR.bit.dr = SlaveAddr;
}


static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	//SlaveAddr is Slave address + r/w bit=1
	SlaveAddr |= 1;
	pI2Cx->DR.bit.dr = SlaveAddr;
}


static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle )
{
	uint32_t read_statusreg_dummy;

	//check for device mode
	if(ENABLE == pI2CHandle->pI2Cx->SR2.bit.msl)
	{
		//device is in master mode
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxSize  == 1)
			{
				//first disable the ack
				I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

				//clear the ADDR flag ( read SR1 , read SR2)
				read_statusreg_dummy = pI2CHandle->pI2Cx->SR1.reg;
				read_statusreg_dummy = pI2CHandle->pI2Cx->SR2.reg;
				(void)read_statusreg_dummy;
			}
		}
		else
		{
			//clear the ADDR flag ( read SR1 , read SR2)
			read_statusreg_dummy = pI2CHandle->pI2Cx->SR1.reg;
			read_statusreg_dummy = pI2CHandle->pI2Cx->SR2.reg;
			(void)read_statusreg_dummy;

		}
	}
	else
	{
		//device is in slave mode
		//clear the ADDR flag ( read SR1 , read SR2)
		read_statusreg_dummy = pI2CHandle->pI2Cx->SR1.reg;
		read_statusreg_dummy = pI2CHandle->pI2Cx->SR2.reg;
		(void)read_statusreg_dummy;
	}
}


void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1.bit.stop = SET;
}


void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2Cx,uint8_t state)
{
	pI2Cx->CR2.bit.itevten = state;
	pI2Cx->CR2.bit.itbufen = state;
	pI2Cx->CR2.bit.iterren = state;
}

/********************************************************************************************************/
/* @function name 		- I2C_PeripheralControl															*/
/*																										*/
/* @brief				- This function controls I2C peripheral state ( enable or disable)				*/
/*																										*/
/* @parameter[in]		- I2C port address																*/
/*																										*/
/* @parameter[in]		- state: ENABLE or DISABLE														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t state)
{
	pI2Cx->CR1.bit.pe = state;
}

/********************************************************************************************************/
/* @function name 		- I2C_PeriphClockControl														*/
/*																										*/
/* @brief				- This function enables or disables peripheral clock for the given I2C			*/
/*																										*/
/* @parameter[in]		- pointer to I2C peripheral base address										*/
/*																										*/
/* @parameter[in]		- ENABLE or DISABLE macro or 1/0												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void I2C_PeriphClockControl(I2C_RegDef_t *pI2Cx, uint8_t state)
{
		if(pI2Cx == I2C1)
		{
			RCC->APB1ENR.bit.i2c1en = state;
		}
		else if (pI2Cx == I2C2)
		{
			RCC->APB1ENR.bit.i2c2en = state;
		}
		else if (pI2Cx == I2C3)
		{
			RCC->APB1ENR.bit.i2c3en = state;
		}

}

/*
 * Initialization and de-initialization
 */
/********************************************************************************************************/
/* @function name 		- I2C_Init																		*/
/*																										*/
/* @brief				- This function initializes given I2C											*/
/*																										*/
/* @parameter[in]		- pointer to I2C handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t temp = 0;
	uint8_t trise;

	//enable the clock for the I2Cx peripheral
	I2C_PeriphClockControl(pI2CHandle->pI2Cx,ENABLE);

	//configure the FREQ field of CR2
	temp = 0;
	temp |= RCC_GetPCLK1Freq() /1000000U;
	pI2CHandle->pI2Cx->CR2.bit.freq = temp;

   //program the device own address
	temp = 0;
	temp |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	//reference manual states that it should always be kept equal to 1 by software
	temp |= ( 1 << 14);
//	temp |= (1 << 15);	//uncomment if you are using 10-bit slave address
	pI2CHandle->pI2Cx->OAR1.reg = temp;

	//CCR calculations
	I2C_CCR_Reg_t CCR_temp = {0};
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_STANDARD)
	{
		//mode is standard mode
		//CCR = Tscl/(2*Tpclk1) => CCR = f(pclk1)/[2*f(scl)]
		CCR_temp.bit.ccr = (RCC_GetPCLK1Freq() / ( 2 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
	}
	else
	{
		//mode is fast mode
		CCR_temp.bit.fs =  ENABLE;
		CCR_temp.bit.duty = pI2CHandle->I2C_Config.I2C_FMDutyCycle;
		if(I2C_FM_DUTY_2 == pI2CHandle->I2C_Config.I2C_FMDutyCycle)
		{
			CCR_temp.bit.ccr = (RCC_GetPCLK1Freq() / ( 3 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		else
		{
			CCR_temp.bit.ccr = (RCC_GetPCLK1Freq() / ( 25 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
	}
	pI2CHandle->pI2Cx->CCR.reg = CCR_temp.reg;

	//TRISE Configuration
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_STANDARD)
	{
		//mode is standard mode
		trise = (RCC_GetPCLK1Freq() /1000000U) + 1 ;

	}
	else
	{
		//mode is fast mode
		trise = ( (RCC_GetPCLK1Freq() * 300) / 1000000000U ) + 1;

	}

	pI2CHandle->pI2Cx->TRISE.bit.trise = trise;

	//I2C peripheral enabled
	I2C_PeripheralControl(pI2CHandle->pI2Cx, ENABLE);

	//ACK control bit set
	I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
}


/********************************************************************************************************/
/* @function name 		- I2C_DeInit																	*/
/*																										*/
/* @brief				- This function de-initializes given I2C										*/
/*																										*/
/* @parameter[in]		- pointer to I2C peripheral base address										*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
	if(pI2Cx == I2C1)
	{
		RCC->APB1RSTR.bit.i2c1rst = SET;
		RCC->APB1RSTR.bit.i2c1rst = RESET;
	}else if(pI2Cx == I2C2)
	{
		RCC->APB1RSTR.bit.i2c2rst = SET;
		RCC->APB1RSTR.bit.i2c2rst = RESET;
	}else if(pI2Cx == I2C3)
	{
		RCC->APB1RSTR.bit.i2c3rst = SET;
		RCC->APB1RSTR.bit.i2c3rst = RESET;
	}
}

/********************************************************************************************************/
/* @function name 		- I2C_GetFlagStatus																*/
/*																										*/
/* @brief				- I2C status register 1 flag function											*/
/*																										*/
/* @parameter[in]		- pointer to I2C peripheral base address										*/
/*																										*/
/* @parameter[in]		- flag name																		*/
/*																										*/
/* @return				- flag state																	*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName)
{
	switch(FlagName)
	{
		case I2C_SR1_SB:
		{
			return pI2Cx->SR1.bit.sb;
			break;
		}
		case I2C_SR1_ADDR:
		{
			return pI2Cx->SR1.bit.addr;
			break;
		}
		case I2C_SR1_BTF:
		{
			return pI2Cx->SR1.bit.btf;
			break;
		}
		case I2C_SR1_ADD10:
		{
			return pI2Cx->SR1.bit.add10;
			break;
		}
		case I2C_SR1_STOPF:
		{
			return pI2Cx->SR1.bit.stopf;
			break;
		}
		case I2C_SR1_RxNE:
		{
			return pI2Cx->SR1.bit.rxne;
			break;
		}
		case I2C_SR1_TxE:
		{
			return pI2Cx->SR1.bit.txe;
			break;
		}
		case I2C_SR1_BERR:
		{
			return pI2Cx->SR1.bit.berr;
			break;
		}
		case I2C_SR1_ARLO:
		{
			return pI2Cx->SR1.bit.arlo;
			break;
		}
		case I2C_SR1_AF:
		{
			return pI2Cx->SR1.bit.af;
			break;
		}
		case I2C_SR1_OVR:
		{
			return pI2Cx->SR1.bit.ovr;
			break;
		}
		case I2C_SR1_PECERR:
		{
			return pI2Cx->SR1.bit.pecerr;
			break;
		}
		case I2C_SR1_TIMEOUT:
		{
			return pI2Cx->SR1.bit.timeout;
			break;
		}
		case I2C_SR1_SMBALERT:
		{
			return pI2Cx->SR1.bit.smbalert;
			break;
		}
		default:
			return RESET;
	}
}


/*
 * Data send and receive
 */
/********************************************************************************************************/
/* @function name 		- I2C_MasterSendData															*/
/*																										*/
/* @brief				- This function sends data to given slave over given I2C						*/
/*																										*/
/* @parameter[in]		- pointer to I2C handle address													*/
/*																										*/
/* @parameter[in]		- pointer of TX buffer															*/
/*																										*/
/* @parameter[in]		- Tx buffer length																*/
/*																										*/
/* @parameter[in]		- slave address																	*/
/*																										*/
/* @parameter[in]		- repeated start enable/disable													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- blocking function !															*/
/********************************************************************************************************/
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	// 1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. confirm that start generation is completed by checking the SB flag in the SR1
	//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_SR1_SB))
	{

	}

	//3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits )
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,SlaveAddr);

	//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_SR1_ADDR))
	{

	}

	//5. clear the ADDR flag according to its software sequence
	//   Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
	I2C_ClearADDRFlag(pI2CHandle);

	//6. send the data until len becomes 0
	while(Len > 0)
	{
		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_SR1_TxE))
		{
			//Wait till TXE is set
		}
		pI2CHandle->pI2Cx->DR.bit.dr = *pTxbuffer;
		pTxbuffer++;
		Len--;
	}

	//7. when Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
	/* Note: TXE=1 , BTF=1 , means that both SR and DR are empty and next transmission should begin
	   when BTF=1 SCL will be stretched (pulled to LOW) */

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_SR1_TxE))
	{

	}

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_SR1_BTF))
	{

	}

	//8. Generate STOP condition and master need not to wait for the completion of stop condition.
	//   Note: generating STOP, automatically clears the BTF
	if(Sr == DISABLE)
	{
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
	}
}

/********************************************************************************************************/
/* @function name 		- I2C_MasterReceiveData															*/
/*																										*/
/* @brief				- This function receives data from given slave over given I2C					*/
/*																										*/
/* @parameter[in]		- pointer to I2C handle address													*/
/*																										*/
/* @parameter[in]		- pointer of RX buffer															*/
/*																										*/
/* @parameter[in]		- Tx buffer length																*/
/*																										*/
/* @parameter[in]		- slave address																	*/
/*																										*/
/* @parameter[in]		- repeated start enable/disable													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- blocking function																*/
/********************************************************************************************************/
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr)
{

	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. confirm that start generation is completed by checking the SB flag in the SR1
	//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_SR1_SB))
	{

	}

	//3. Send the address of the slave with r/nw bit set to R(1) (total 8 bits )
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,SlaveAddr);

	//4. wait until address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_SR1_ADDR))
	{

	}
	//procedure to read only 1 byte from slave
	if(Len == 1)
	{
		//Disable Acking
		I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_SR1_RxNE))
		{
			//wait until  RXNE becomes 1
		}

		//generate STOP condition
		if(Sr == DISABLE)
		{
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
		}
		//read data in to buffer
		*pRxBuffer = pI2CHandle->pI2Cx->DR.bit.dr;
	}

    //procedure to read data from slave when Len > 1
	if(Len > 1)
	{
		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//read the data until Len becomes zero
		for ( uint32_t i = Len ; i > 0 ; i--)
		{
			while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_SR1_RxNE))
			{
				//wait until RXNE becomes 1
			}
			//if last 2 bytes are remaining
			if(i == 2)
			{
				//Disable Acking
				I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

				//generate STOP condition
				if(Sr == DISABLE )
				{
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
				}
			}
			//read the data from data register in to buffer
			*pRxBuffer = pI2CHandle->pI2Cx->DR.bit.dr;

			//increment the buffer address
			pRxBuffer++;
		}
	}

	//re-enable ACKing
	if(I2C_ACK_ENABLE == pI2CHandle->I2C_Config.I2C_AckControl)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);
	}

}

/********************************************************************************************************/
/* @function name 		- I2C_ManageAcking																*/
/*																										*/
/* @brief				- This function changes the state of ACKing for given I2C ( enables and 		*/
/*						  disables ACK)																	*/
/*																										*/
/* @parameter[in]		- pointer to I2C peripheral base address										*/
/*																										*/
/* @parameter[in]		- I2C_ACK_ENABLE or I2C_ACK_DISABLE												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t state)
{
	pI2Cx->CR1.bit.ack = state;
}

/*
 * IRQ configuration and ISR handling
 */
/********************************************************************************************************/
/* @function name 		- I2C_IRQConfig																	*/
/*																										*/
/* @brief				- I2C interrupt routine															*/
/*																										*/
/* @parameter[in]		- predefined IRQ Number															*/
/*																										*/
/* @parameter[in]		- interrupt priority															*/
/*																										*/
/* @parameter[in]		- on/off state																	*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void I2C_IRQConfig(uint8_t IRQNumber, uint32_t IRQPriority, uint8_t state)
{
	if(state == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			NVIC->ISER[0] |= ( 1 << IRQNumber );

		}//32 to 63
		else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			//program ISER1 register
			NVIC->ISER[1] |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ISER2 register //64 to 95
			NVIC->ISER[3] |= ( 1 << (IRQNumber % 64) );
		}
	}
	else
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			NVIC->ICER[0] |= ( 1 << IRQNumber );
		}
		else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			//program ICER1 register
			NVIC->ICER[1] |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 6 && IRQNumber < 96 )
		{
			//program ICER2 register
			NVIC->ICER[3] |= ( 1 << (IRQNumber % 64) );
		}
	}

	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount = (8 * iprx_section) + 4;
	*(NVIC_IPR_BASE + iprx) |= IRQPriority << shift_amount;
}

/********************************************************************************************************/
/* @function name 		- I2C_MasterSendDataIT															*/
/*																										*/
/* @brief				- This function is an interrupt function to send data							*/
/*																										*/
/* @parameter[in]		- pointer to I2C handle address													*/
/*																										*/
/* @parameter[in]		- pointer to TX buffer															*/
/*																										*/
/* @parameter[in]		- TX buffer length																*/
/*																										*/
/* @parameter[in]		- slave address																	*/
/*																										*/
/* @parameter[in]		- repeated start enable/disable													*/
/*																										*/
/* @return				- busy state																	*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	uint8_t busy_state = pI2CHandle->TxRxState;

	if( (I2C_BUSY_IN_TX != busy_state) && (I2C_BUSY_IN_RX != busy_state))
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2.bit.itbufen = SET;

		//enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->CR2.bit.itevten = SET;

		//enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2.bit.iterren = SET;
	}
	return busy_state;
}


/********************************************************************************************************/
/* @function name 		- I2C_MasterSendDataIT															*/
/*																										*/
/* @brief				- This function is an interrupt function to receive data						*/
/*																										*/
/* @parameter[in]		- pointer to I2C handle address													*/
/*																										*/
/* @parameter[in]		- pointer to RX buffer															*/
/*																										*/
/* @parameter[in]		- RX buffer length																*/
/*																										*/
/* @parameter[in]		- slave address																	*/
/*																										*/
/* @parameter[in]		- repeated start enable/disable													*/
/*																										*/
/* @return				- busy state																	*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	uint8_t busy_state = pI2CHandle->TxRxState;

	if( (I2C_BUSY_IN_TX != busy_state) && (I2C_BUSY_IN_RX != busy_state))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = Len;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2.bit.itbufen = SET;

		//enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->CR2.bit.itevten = SET;

		//enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2.bit.iterren = SET;
	}
	return busy_state;
}


static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle )
{

	if(pI2CHandle->TxLen > 0)
	{
		//1. load the data in to DR
		pI2CHandle->pI2Cx->DR.bit.dr = *(pI2CHandle->pTxBuffer);

		//2. decrement the TxLen
		pI2CHandle->TxLen--;

		//3. Increment the buffer address
		pI2CHandle->pTxBuffer++;

	}

}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle )
{
	//We have to do the data reception
	if(1 == pI2CHandle->RxSize)
	{
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR.bit.dr;
		pI2CHandle->RxLen--;

	}


	if(pI2CHandle->RxSize > 1)
	{
		if(2 == pI2CHandle->RxLen)
		{
			//clear the ack bit
			I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);
		}

			//read DR
			*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR.bit.dr;
			pI2CHandle->pRxBuffer++;
			pI2CHandle->RxLen--;
	}

	if(0 == pI2CHandle->RxLen)
	{
		//close the I2C data reception and notify the application

		//1. generate the stop condition
		if(DISABLE == pI2CHandle->Sr)
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//2 . Close the I2C rx
		I2C_CloseReceiveData(pI2CHandle);

		//3. Notify the application
		I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_RX_CMPLT);
	}
}

/********************************************************************************************************/
/* @function name 		- I2C_CloseReceiveData															*/
/*																										*/
/* @brief				- This function terminates I2C reception										*/
/*																										*/
/* @parameter[in]		- pointer to I2C handle address													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
	//disable ITBUFEN Control Bit
	pI2CHandle->pI2Cx->CR2.bit.itbufen = RESET;

	//disable ITEVFEN Control Bit
	pI2CHandle->pI2Cx->CR2.bit.itevten = RESET;

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLen = 0;
	pI2CHandle->RxSize = 0;

	if(I2C_ACK_ENABLE == pI2CHandle->I2C_Config.I2C_AckControl)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);
	}

}

/********************************************************************************************************/
/* @function name 		- I2C_CloseSendData																*/
/*																										*/
/* @brief				- This function terminates I2C transmission										*/
/*																										*/
/* @parameter[in]		- pointer to I2C handle address													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
	//disable ITBUFEN Control Bit
	pI2CHandle->pI2Cx->CR2.bit.itbufen = RESET;

	//disable ITEVFEN Control Bit
	pI2CHandle->pI2Cx->CR2.bit.itevten = RESET;


	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;
}



void I2C_SlaveSendData(I2C_RegDef_t *pI2C,uint8_t data)
{
	pI2C->DR.bit.dr = data;
}

uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C)
{
    return (uint8_t) pI2C->DR.bit.dr;
}


/********************************************************************************************************/
/* @function name 		- I2C_Event_IRQHandling															*/
/*																										*/
/* @brief				- Interrupt mode event handling													*/
/*																										*/
/* @parameter[in]		- pointer to I2C handle address													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void I2C_Event_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	//Interrupt handling for both master and slave mode of a device

	uint32_t event_it_en, buffer_it_en, status_temp;

	event_it_en  =  pI2CHandle->pI2Cx->CR2.bit.itevten;
	buffer_it_en   = pI2CHandle->pI2Cx->CR2.bit.itbufen;

	status_temp  = pI2CHandle->pI2Cx->SR1.bit.sb;
	//1. Handle For interrupt generated by SB event
	//	Note : SB flag is only applicable in Master mode
	if((ENABLE == event_it_en) && (SET == status_temp))
	{
		//The interrupt is generated because of SB event
		//This block will not be executed in slave mode because for slave SB is always zero
		//In this block lets executed the address phase
		if(I2C_BUSY_IN_TX == pI2CHandle->TxRxState)
		{
			I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,pI2CHandle->DevAddr);
		}
		else if(I2C_BUSY_IN_RX == pI2CHandle->TxRxState)
		{
			I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,pI2CHandle->DevAddr);
		}
	}
	status_temp  = pI2CHandle->pI2Cx->SR1.bit.addr;
	//2. Handle For interrupt generated by ADDR event
	//Note : When master mode : Address is sent
	//		 When Slave mode   : Address matched with own address
	if((ENABLE == event_it_en) && (SET == status_temp))
	{
		// interrupt is generated because of ADDR event
		I2C_ClearADDRFlag(pI2CHandle);
	}

	status_temp  = pI2CHandle->pI2Cx->SR1.bit.btf;
	//3. Handle For interrupt generated by BTF(Byte Transfer Finished) event
	if((ENABLE == event_it_en) && (SET == status_temp))
	{
		//BTF flag is set
		if(I2C_BUSY_IN_TX == pI2CHandle->TxRxState)
		{
			//make sure that TXE is also set
			if(SET == pI2CHandle->pI2Cx->SR1.bit.txe)
			{
				//BTF, TXE = 1
				if(0 == pI2CHandle->TxLen)
				{
					//1. generate the STOP condition
					if(pI2CHandle->Sr == DISABLE)
					{
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
					}
					//2. reset all the member elements of the handle structure.
					I2C_CloseSendData(pI2CHandle);

					//3. notify the application about transmission complete
					I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_TX_CMPLT);
				}
			}
		}
		else if(I2C_BUSY_IN_RX == pI2CHandle->TxRxState)
		{
			//nothing to do
		}
	}

	status_temp  = pI2CHandle->pI2Cx->SR1.bit.stopf;
	//4. Handle For interrupt generated by STOPF event
	// Note : Stop detection flag is applicable only slave mode . For master this flag will never be set
	//The below code block will not be executed by the master since STOPF will not set in master mode
	if((ENABLE == event_it_en) && (SET == status_temp))
	{
		//STOF flag is set
		/*Clear the STOPF (
		i.e 1) read SR1
			2) Write to CR1) */

		pI2CHandle->pI2Cx->CR1.reg = 0x0000;

		//Notify the application that STOP is detected
		I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_STOP);
	}


	status_temp  = pI2CHandle->pI2Cx->SR1.bit.txe;
	//5. Handle For interrupt generated by TXE event
	if((ENABLE == event_it_en) && (ENABLE == buffer_it_en) && (SET == status_temp))
	{
		//Check for device mode
		//if in master mode
		if(ENABLE == pI2CHandle->pI2Cx->SR2.bit.msl)
		{
			//TXE flag is set
			//We have to do the data transmission
			if(I2C_BUSY_IN_TX == pI2CHandle->TxRxState)
			{
				I2C_MasterHandleTXEInterrupt(pI2CHandle);
			}
		}
		else
		{
			//slave
			//make sure that the slave is really in transmitter mode
		    if(SET == pI2CHandle->pI2Cx->SR2.bit.tra)
		    {
		    	I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_DATA_REQ);
		    }
		}
	}

	status_temp  = pI2CHandle->pI2Cx->SR1.bit.rxne;
	//6. Handle For interrupt generated by RXNE event
	if((ENABLE == event_it_en) && (ENABLE == buffer_it_en) && (SET == status_temp))
	{
		//check device mode
		if(ENABLE == pI2CHandle->pI2Cx->SR2.bit.msl)
		{
			//The device is master

			//RXNE flag is set
			if(I2C_BUSY_IN_RX == pI2CHandle->TxRxState)
			{
				I2C_MasterHandleRXNEInterrupt(pI2CHandle);
			}

		}
		else
		{
			//slave
			//make sure that the slave is really in receiver mode
			if(!(SET == pI2CHandle->pI2Cx->SR2.bit.tra))
			{
				I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_DATA_RCV);
			}
		}
	}
}



/********************************************************************************************************/
/* @function name 		- I2C_Error_IRQHandling															*/
/*																										*/
/* @brief				- Interrupt mode error handling													*/
/*																										*/
/* @parameter[in]		- pointer to I2C handle address													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void I2C_Error_IRQHandling(I2C_Handle_t *pI2CHandle)
{

	uint32_t error_temp,error_it_en;

    //Learn the status of ITERREN control bit in the CR2
	error_it_en = pI2CHandle->pI2Cx->CR2.bit.iterren;


/***********************Check for Bus error************************************/
	error_temp = pI2CHandle->pI2Cx->SR1.bit.berr;
	if((SET == error_temp)  && (ENABLE == error_it_en))
	{
		//This is Bus error
		//clear the bus error flag
		pI2CHandle->pI2Cx->SR1.bit.berr == RESET;

		//notify the application about the error
	   I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/
	error_temp = pI2CHandle->pI2Cx->SR1.bit.arlo;
	if((SET == error_temp)  && (ENABLE == error_it_en))
	{
		//This is arbitration lost error
		//clear the arbitration lost error flag
		pI2CHandle->pI2Cx->SR1.bit.arlo = RESET;

		//notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_ARLO);

	}

/***********************Check for ACK failure  error************************************/

	error_temp = pI2CHandle->pI2Cx->SR1.bit.af;
	if((SET == error_temp)  && (ENABLE == error_it_en))
	{
		//This is ACK failure error
	    //clear the ACK failure error flag
		pI2CHandle->pI2Cx->SR1.bit.af = RESET;

		//notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_AF);
	}

/***********************Check for Overrun/underrun error************************************/
	error_temp = pI2CHandle->pI2Cx->SR1.bit.ovr;
	if((SET == error_temp)  && (ENABLE == error_it_en))
	{
		//This is Overrun/underrun
	    //clear the Overrun/underrun error flag
		pI2CHandle->pI2Cx->SR1.bit.ovr = RESET;

		//notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_OVR);
	}

/***********************Check for Time out error************************************/
	error_temp = pI2CHandle->pI2Cx->SR1.bit.timeout;
	if((SET == error_temp)  && (ENABLE == error_it_en))
	{
		//This is Time out error

	    //clear the Time out error flag
		pI2CHandle->pI2Cx->SR1.bit.timeout = RESET;

		//notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_TIMEOUT);
	}

}

/****************************************************** End of file *************************************************************/


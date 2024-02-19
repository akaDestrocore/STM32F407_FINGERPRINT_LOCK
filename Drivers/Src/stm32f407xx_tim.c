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

#include <stm32f407xx_tim.h>

// helper API prototypes
static void tim_base_setConfig(TIM_Handle_t *pTIMHandle);
static void tim_CCxChannelCmd(TIM_Handle_t *pTIMHandle, uint8_t Channel, uint32_t ChannelStateEnorDi);
static void tim_channel_state_set(TIM_Handle_t *pTIMHandle, uint8_t Channel, uint32_t ChannelState);
static void tim_channel_n_state_set(TIM_Handle_t *pTIMHandle, uint8_t Channel, uint32_t ChannelNState);
static void tim_it_control(TIM_Handle_t *pTIMHandle, uint8_t Interrupt, uint8_t State);
uint32_t tim_read_ccmrx(TIM_Handle_t *pTIMHandle, uint8_t TIM_CCMRx);
uint32_t tim_read_ccer(TIM_Handle_t *pTIMHandle);
uint32_t tim_read_cr2(TIM_Handle_t *pTIMHandle);
uint32_t tim_read_smcr(TIM_Handle_t *pTIMHandle);
uint32_t tim_read_ccrx(TIM_Handle_t *pTIMHandle, uint8_t TIM_CCRx);
static void tim_write_to_cr2(TIM_Handle_t *pTIMHandle, uint32_t CR2tempReg);
static void tim_write_to_ccer(TIM_Handle_t *pTIMHandle, uint32_t CCERtempReg);
static void tim_write_to_ccmrx(TIM_Handle_t *pTIMHandle, uint8_t TIM_CCMRx ,uint32_t CCMRxtempReg);
static void tim_write_to_ccrx(TIM_Handle_t *pTIMHandle, uint8_t TIM_CCRx, uint32_t CCRxtemp);
static void tim_write_to_smcr(TIM_Handle_t *pTIMHandle, uint32_t SMCRtempReg);
static void tim_write_to_egr(TIM_Handle_t *pTIMHandle, uint32_t EGRtempReg);
static void tim_oc1_set_config(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *OC_Config);
static void tim_oc2_set_config(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *OC_Config);
static void tim_oc3_set_config(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *OC_Config);
static void tim_oc4_set_config(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *OC_Config);
static void tim_ti1_set_config(TIM_Handle_t *pTIMHandle, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
        uint32_t TIM_ICFilter);
static void tim_ti2_set_config(TIM_Handle_t *pTIMHandle, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
        uint32_t TIM_ICFilter);
static void tim_ti3_set_config(TIM_Handle_t *pTIMHandle, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
        uint32_t TIM_ICFilter);
static void tim_ti4_set_config(TIM_Handle_t *pTIMHandle, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
        uint32_t TIM_ICFilter);
static void tim_etr_set_config(TIM_Handle_t *pTIMHandle, uint32_t TIM_ExtTRGPrescaler, uint32_t TIM_ExtTRGPolarity,
		uint32_t ExtTRGFilter);
static void tim_itrx_set_config(TIM_Handle_t *pTIMHandle, uint32_t InputTriggerSource);
static void tim_ti1_config_input_stage(TIM_Handle_t *pTIMHandle, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter);
static void tim_ti2_config_input_stage(TIM_Handle_t *pTIMHandle, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter);
uint8_t tim_slave_timer_set_config(TIM_Handle_t *pTIMHandle,TIM_Slave_Config_t *sSlaveConfig);




/*
 * TIM Peripheral Control
 */
/********************************************************************************************************/
/* @function name 		- TIM_PeriphControl																*/
/*																										*/
/* @brief				- This function enables or disables given timer peripheral						*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- ENABLE or DISABLE macro or 1/0												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_PeriphControl(TIM_Handle_t *pTIMHandle, uint8_t state)
{
	switch(pTIMHandle->TIMx)
	{
		case TIM_1:
		{
			TIM1->CR1.bit.cen = state;
			break;
		}
		case TIM_2:
		{
			TIM2->CR1.bit.cen = state;
			break;
		}
		case TIM_3:
		{
			TIM3->CR1.bit.cen = state;
			break;
		}
		case TIM_4:
		{
			TIM4->CR1.bit.cen = state;
			break;
		}
		case TIM_5:
		{
			TIM5->CR1.bit.cen = state;
			break;
		}
		case TIM_6:
		{
			TIM6->CR1.bit.cen = state;
			break;
		}
		case TIM_7:
		{
			TIM7->CR1.bit.cen = state;
			break;
		}
		case TIM_8:
		{
			TIM8->CR1.bit.cen = state;
			break;
		}
		case TIM_9:
		{
			TIM9->CR1.bit.cen = state;
			break;
		}
		case TIM_10:
		{
			TIM10->CR1.bit.cen = state;
			break;
		}
		case TIM_11:
		{
			TIM11->CR1.bit.cen = state;
			break;
		}
		case TIM_12:
		{
			TIM12->CR1.bit.cen = state;
			break;
		}
		case TIM_13:
		{
			TIM13->CR1.bit.cen = state;
			break;
		}
		case TIM_14:
		{
			TIM14->CR1.bit.cen = state;
			break;
		}
		default:
			return;
	}
}

/*
 * Time base functions
 */
/********************************************************************************************************/
/* @function name 		- TIM_Base_Init																	*/
/*																										*/
/* @brief				- This function initializes given TIM Base according to the specified parameters*/
/*						   in the TIM_Handle_t and initializes the associated handle					*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_Base_Init(TIM_Handle_t *pTIMHandle)
{
	//set TIM state
	pTIMHandle->State = TIM_STATE_BUSY;

	tim_base_setConfig(pTIMHandle);

	// initialize the DMA burst operation state
	pTIMHandle->DMABurstState = DMA_BURST_STATE_READY;

	// initialize the TIM channels state
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_READY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_READY);

	// initialize the TIM state
	pTIMHandle->State = TIM_STATE_READY;
}


/********************************************************************************************************/
/* @function name 		- TIM_Base_DeInit																*/
/*																										*/
/* @brief				- This function de-initializes given TIM Base									*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_Base_DeInit(TIM_Handle_t *pTIMHandle)
{
	//set busy state
	pTIMHandle->State = TIM_STATE_BUSY;

	if(TIM_1 == pTIMHandle->TIMx)
	{
		TIM1->CCER.reg = RESET;
	}else if(TIM_2 == pTIMHandle->TIMx)
	{
		TIM2->CCER.reg = RESET;
	}else if(TIM_3 == pTIMHandle->TIMx)
	{
		TIM3->CCER.reg = RESET;
	}else if(TIM_4 == pTIMHandle->TIMx)
	{
		TIM4->CCER.reg = RESET;
	}else if(TIM_5 == pTIMHandle->TIMx)
	{
		TIM5->CCER.reg = RESET;
	}else if(TIM_8 == pTIMHandle->TIMx)
	{
		TIM8->CCER.reg = RESET;
	}else if(TIM_9 == pTIMHandle->TIMx)
	{
		TIM9->CCER.reg = RESET;
	}else if(TIM_10 == pTIMHandle->TIMx)
	{
		TIM10->CCER.reg = RESET;
	}else if(TIM_11 == pTIMHandle->TIMx)
	{
		TIM11->CCER.reg = RESET;
	}else if(TIM_12 == pTIMHandle->TIMx)
	{
		TIM12->CCER.reg = RESET;
	}else if(TIM_13 == pTIMHandle->TIMx)
	{
		TIM13->CCER.reg = RESET;
	}else if(TIM_14 == pTIMHandle->TIMx)
	{
		TIM14->CCER.reg = RESET;
	}

	TIM_PeriphControl(pTIMHandle, DISABLE);

	// change the DMA burst operation state
	pTIMHandle->DMABurstState = DMA_BURST_STATE_RESET;

	// change the TIM channels state
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_RESET);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_RESET);

	// change the TIM  state
	pTIMHandle->State = TIM_STATE_RESET;
}


/********************************************************************************************************/
/* @function name 		- TIM_Base_Start																*/
/*																										*/
/* @brief				- This function starts TIM base generation for given timer						*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_Base_Start(TIM_Handle_t *pTIMHandle)
{
	if(TIM_STATE_READY != pTIMHandle->State)
	{
		return;
	}

	//set busy state
	pTIMHandle->State = TIM_STATE_BUSY;

	//enable timer
	TIM_PeriphControl(pTIMHandle, ENABLE);
}


/********************************************************************************************************/
/* @function name 		- TIM_Base_Stop																	*/
/*																										*/
/* @brief				- This function stops TIM base generation for given timer						*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_Base_Stop(TIM_Handle_t *pTIMHandle)
{
	// disable timer
	TIM_PeriphControl(pTIMHandle, DISABLE);

	// set ready state
	pTIMHandle->State = TIM_STATE_READY;
}

/********************************************************************************************************/
/* @function name 		- TIM_Base_StartIT																*/
/*																										*/
/* @brief				- This function starts TIM base generation for given timer in interrupt mode		*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_Base_StartIT(TIM_Handle_t *pTIMHandle)
{
	// set state busy
	pTIMHandle->State = TIM_STATE_BUSY;

	// enable the TIM update interrupt
	tim_it_control(pTIMHandle, TIM_IT_UPDATE, ENABLE);

	TIM_PeriphControl(pTIMHandle, ENABLE);

}


/********************************************************************************************************/
/* @function name 		- TIM_Base_StopIT																*/
/*																										*/
/* @brief				- This function stops TIM base generation for given timer in interrupt mode		*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_Base_StopIT(TIM_Handle_t *pTIMHandle)
{
	// disable the TIM update interrupt
	tim_it_control(pTIMHandle, TIM_IT_UPDATE, DISABLE);

	TIM_PeriphControl(pTIMHandle, DISABLE);

	//set TIM state ready
	pTIMHandle->State = TIM_STATE_READY;
}


/*
 * Timer Output Compare functions
 */
/********************************************************************************************************/
/* @function name 		- TIM_OC_Init																	*/
/*																										*/
/* @brief				- This function initializes given TIM Base according to the specified parameters*/
/*						   in the TIM_Handle_t and initializes the associated handle					*/
/*																										*/
/* @parameter[in]		- pointer to TIM Output Compare handle base address								*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_OC_Init(TIM_Handle_t *pTIMHandle)
{
	if(NULL == pTIMHandle)
	{
		return;
	}

	//set TIM state
	pTIMHandle->State = TIM_STATE_BUSY;

	// initialize base time for OC
	tim_base_setConfig(pTIMHandle);

	//initialize DMA burst
	pTIMHandle->DMABurstState = DMA_BURST_STATE_READY;

	// initialize the TIM channels state
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_READY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_READY);

	// initialize the TIM state
	pTIMHandle->State = TIM_STATE_READY;
}

/********************************************************************************************************/
/* @function name 		- TIM_OC_DeInit																	*/
/*																										*/
/* @brief				- This function de-initializes given TIM Output Compare							*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_OC_DeInit(TIM_Handle_t *pTIMHandle)
{
	//set busy state
	pTIMHandle->State = TIM_STATE_BUSY;

	//disable TIM peripheral clock
	if(TIM_1 == pTIMHandle->TIMx)
	{
		TIM1->CCER.reg = RESET;
	}else if(TIM_2 == pTIMHandle->TIMx)
	{
		TIM2->CCER.reg = RESET;
	}else if(TIM_3 == pTIMHandle->TIMx)
	{
		TIM3->CCER.reg = RESET;
	}else if(TIM_4 == pTIMHandle->TIMx)
	{
		TIM4->CCER.reg = RESET;
	}else if(TIM_5 == pTIMHandle->TIMx)
	{
		TIM5->CCER.reg = RESET;
	}else if(TIM_8 == pTIMHandle->TIMx)
	{
		TIM8->CCER.reg = RESET;
	}else if(TIM_9 == pTIMHandle->TIMx)
	{
		TIM9->CCER.reg = RESET;
	}else if(TIM_10 == pTIMHandle->TIMx)
	{
		TIM10->CCER.reg = RESET;
	}else if(TIM_11 == pTIMHandle->TIMx)
	{
		TIM11->CCER.reg = RESET;
	}else if(TIM_12 == pTIMHandle->TIMx)
	{
		TIM12->CCER.reg = RESET;
	}else if(TIM_13 == pTIMHandle->TIMx)
	{
		TIM13->CCER.reg = RESET;
	}else if(TIM_14 == pTIMHandle->TIMx)
	{
		TIM14->CCER.reg = RESET;
	}

	TIM_PeriphControl(pTIMHandle, DISABLE);

	//change the DMA burst operation state
	pTIMHandle->DMABurstState = DMA_BURST_STATE_RESET;

	// change the TIM channels state
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_RESET);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_RESET);

	// change the TIM state
	pTIMHandle->State = TIM_STATE_RESET;
}


/********************************************************************************************************/
/* @function name 		- TIM_OC_Start																	*/
/*																										*/
/* @brief				- This function starts TIM Output Compare signal generation for given timer		*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM channel to be enabled														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_OC_Start(TIM_Handle_t *pTIMHandle, uint8_t Channel)
{
	if((TIM_CHANNEL_1 == pTIMHandle->Channel) && (TIM_CH_STATE_READY != pTIMHandle->ChannelState[0]))
	{
		return;
	}else if((TIM_CHANNEL_2 == pTIMHandle->Channel) && (TIM_CH_STATE_READY != pTIMHandle->ChannelState[1]))
	{
		return;
	}else if((TIM_CHANNEL_3 == pTIMHandle->Channel) && (TIM_CH_STATE_READY != pTIMHandle->ChannelState[2]))
	{
		return;
	}else if((TIM_CHANNEL_4 == pTIMHandle->Channel) && (TIM_CH_STATE_READY != pTIMHandle->ChannelState[3]))
	{
		return;
	}

	//set channel state
	tim_channel_state_set(pTIMHandle, Channel, TIM_CH_STATE_BUSY);

	//enable the Output Compare channel
	tim_CCxChannelCmd(pTIMHandle, Channel, ENABLE);

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		//enable main output
		switch(pTIMHandle->TIMx)
		{
		case TIM_1:
		{
			TIM1->BDTR.bit.moe = ENABLE;
			break;
		}
		case TIM_8:
		{
			TIM8->BDTR.bit.moe = ENABLE;
			break;
		}
		}
	}

	//enable TIM peripheral
	TIM_PeriphControl(pTIMHandle, ENABLE);
}


/********************************************************************************************************/
/* @function name 		- TIM_OC_Stop																	*/
/*																										*/
/* @brief				- This function stops TIM Output Compare signal generation for given timer		*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM channel to be disabled													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_OC_Stop(TIM_Handle_t *pTIMHandle, uint8_t Channel)
{
	// disable TIM peripheral
	TIM_PeriphControl(pTIMHandle, DISABLE);

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		// disable main output
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->BDTR.bit.moe = DISABLE;
				break;
			}
			case TIM_8:
			{
				TIM8->BDTR.bit.moe = DISABLE;
				break;
			}
		}
	}

	// set the TIM channel state
	tim_channel_state_set(pTIMHandle, Channel, TIM_CH_STATE_READY);
}


/********************************************************************************************************/
/* @function name 		- TIM_OC_StartIT																*/
/*																										*/
/* @brief				- This function starts TIM Output Compare signal generation for given timer in 	*/
/* 						  interrupt	mode																*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM channel to be enabled														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_OC_StartIT(TIM_Handle_t *pTIMHandle, uint8_t Channel)
{
	if((TIM_CHANNEL_1 == pTIMHandle->Channel) && (TIM_CH_STATE_READY != pTIMHandle->ChannelState[0]))
	{
		return;
	}else if((TIM_CHANNEL_2 == pTIMHandle->Channel) && (TIM_CH_STATE_READY != pTIMHandle->ChannelState[1]))
	{
		return;
	}else if((TIM_CHANNEL_3 == pTIMHandle->Channel) && (TIM_CH_STATE_READY != pTIMHandle->ChannelState[2]))
	{
		return;
	}else if((TIM_CHANNEL_4 == pTIMHandle->Channel) && (TIM_CH_STATE_READY != pTIMHandle->ChannelState[3]))
	{
		return;
	}

	// set the TIM channel state
	tim_channel_state_set(pTIMHandle, Channel, TIM_CH_STATE_BUSY);

	switch(Channel)
	{
		case TIM_CHANNEL_1:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC1, ENABLE);
			break;
		}
		case TIM_CHANNEL_2:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC2, ENABLE);
			break;
		}
		case TIM_CHANNEL_3:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC3, ENABLE);
			break;
		}
		case TIM_CHANNEL_4:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC4, ENABLE);
			break;
		}
		default:
			break;
	}

	// enable the output compare channel
	tim_CCxChannelCmd(pTIMHandle, Channel, ENABLE);

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		// enable main output
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->BDTR.bit.moe = ENABLE;
				break;
			}
			case TIM_8:
			{
				TIM8->BDTR.bit.moe = ENABLE;
				break;
			}
		}
	}

	TIM_PeriphControl(pTIMHandle, ENABLE);
}

/********************************************************************************************************/
/* @function name 		- TIM_OC_StopIT																	*/
/*																										*/
/* @brief				- This function stops TIM Output Compare signal generation for given timer in 	*/
/* 						  interrupt	mode																*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM channel to be disabled													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_OC_StopIT(TIM_Handle_t *pTIMHandle, uint8_t Channel)
{
	switch(Channel)
	{
		case TIM_CHANNEL_1:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC1, DISABLE);
			break;
		}
		case TIM_CHANNEL_2:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC2, DISABLE);
			break;
		}
		case TIM_CHANNEL_3:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC3, DISABLE);
			break;
		}
		case TIM_CHANNEL_4:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC4, DISABLE);
			break;
		}
		default:
			break;
	}

	tim_CCxChannelCmd(pTIMHandle, Channel, DISABLE);

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		// disable main output
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->BDTR.bit.moe = DISABLE;
				break;
			}
			case TIM_8:
			{
				TIM8->BDTR.bit.moe = DISABLE;
				break;
			}
		}
	}

	// disable TIM
	TIM_PeriphControl(pTIMHandle, DISABLE);

	tim_channel_state_set(pTIMHandle, Channel, TIM_CH_STATE_READY);
}


/*
 * Timer PWM functions
 */
/********************************************************************************************************/
/* @function name 		- TIM_PWM_Init																	*/
/*																										*/
/* @brief				- This function initializes given TIM PWM according to the specified parameters */
/*						   in the TIM_Handle_t and initializes the associated handle					*/
/*																										*/
/* @parameter[in]		- pointer to TIM PWM handle base address										*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- reset timer after switching between center aligned mode and edge counter mode	*/
/********************************************************************************************************/
void TIM_PWM_Init(TIM_Handle_t *pTIMHandle)
{
	//set TIM state
	pTIMHandle->State = TIM_STATE_BUSY;

	// initialize base time for PWM
	tim_base_setConfig(pTIMHandle);

	//initialize DMA burst operation state
	pTIMHandle->DMABurstState = DMA_BURST_STATE_READY;

	//initialize TIM channel state
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_READY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_READY);

	// initialize TIM state
	pTIMHandle->State = TIM_STATE_READY;
}


/********************************************************************************************************/
/* @function name 		- TIM_PWM_DeInit																*/
/*																										*/
/* @brief				- This function de-initializes given TIM PWM according to the specified 		*/
/* 						  parameters in the TIM_Handle_t and initializes the associated handle			*/
/*																										*/
/* @parameter[in]		- pointer to TIM PWM handle base address										*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_PWM_DeInit(TIM_Handle_t *pTIMHandle)
{
	pTIMHandle->State = TIM_STATE_BUSY;

	//disable TIMx
	TIM_PeriphControl(pTIMHandle, DISABLE);

	// change DMA burst operation state
	pTIMHandle->DMABurstState = DMA_BURST_STATE_RESET;

	//change channel state
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_RESET);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_RESET);

	//change TIM state
	pTIMHandle->State = TIM_STATE_RESET;
}


/********************************************************************************************************/
/* @function name 		- TIM_PWM_Start																	*/
/*																										*/
/* @brief				- This function starts TIM PWM generation for given timer						*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIMx channel to be enabled													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_PWM_Start(TIM_Handle_t *pTIMHandle, uint8_t Channel)
{
	//set channel state
	tim_channel_state_set(pTIMHandle, Channel, TIM_CH_STATE_BUSY);

	//enable capture compare mode
	tim_CCxChannelCmd(pTIMHandle, Channel, ENABLE);

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		//enable main output
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->BDTR.bit.moe = ENABLE;
				break;
			}
			case TIM_8:
			{
				TIM8->BDTR.bit.moe = ENABLE;
				break;
			}
		}
	}

	// enable timer
	TIM_PeriphControl(pTIMHandle, ENABLE);
}


/********************************************************************************************************/
/* @function name 		- TIM_PWM_Stop																	*/
/*																										*/
/* @brief				- This function stops TIM PWM generation for given timer						*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIMx channel to be disabled													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_PWM_Stop(TIM_Handle_t *pTIMHandle, uint8_t Channel)
{
	//diable capture compare mode
	tim_CCxChannelCmd(pTIMHandle, Channel, DISABLE);

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		//enable main output
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->BDTR.bit.moe = DISABLE;
				break;
			}
			case TIM_8:
			{
				TIM8->BDTR.bit.moe = DISABLE;
				break;
			}
		}
	}

	// disable timer
	TIM_PeriphControl(pTIMHandle, DISABLE);

	// set channel state ready
	tim_channel_state_set(pTIMHandle, Channel, TIM_CH_STATE_READY);
}


/********************************************************************************************************/
/* @function name 		- TIM_PWM_StartIT																*/
/*																										*/
/* @brief				- This function starts TIM PWM signal generation for given timer in interrupt	*/
/* 						  mode																			*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM channel to be enabled														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_PWM_StartIT(TIM_Handle_t *pTIMHandle, uint8_t Channel)
{
	//set channel state
	tim_channel_state_set(pTIMHandle, Channel, TIM_CH_STATE_BUSY);

	switch(Channel)
	{
		case TIM_CHANNEL_1:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC1, ENABLE);
			break;
		}
		case TIM_CHANNEL_2:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC2, ENABLE);
			break;
		}
		case TIM_CHANNEL_3:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC3, ENABLE);
			break;
		}
		case TIM_CHANNEL_4:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC4, ENABLE);
			break;
		}
		default:
			break;
	}

	// enable capture compare mode
	tim_CCxChannelCmd(pTIMHandle, Channel, ENABLE);

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		//enable main output
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->BDTR.bit.moe = ENABLE;
				break;
			}
			case TIM_8:
			{
				TIM8->BDTR.bit.moe = ENABLE;
				break;
			}
		}
	}

	TIM_PeriphControl(pTIMHandle, ENABLE);
}


/********************************************************************************************************/
/* @function name 		- TIM_PWM_StartIT																*/
/*																										*/
/* @brief				- This function stops TIM PWM signal generation for given timer in interrupt	*/
/* 						  mode																			*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM channel to be disabled													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_PWM_StopIT(TIM_Handle_t *pTIMHandle, uint8_t Channel)
{
	switch(Channel)
	{
		case TIM_CHANNEL_1:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC1, DISABLE);
			break;
		}
		case TIM_CHANNEL_2:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC2, DISABLE);
			break;
		}
		case TIM_CHANNEL_3:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC3, DISABLE);
			break;
		}
		case TIM_CHANNEL_4:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC4, DISABLE);
			break;
		}
		default:
			break;
	}

	tim_CCxChannelCmd(pTIMHandle, Channel, DISABLE);

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		//disable main output
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->BDTR.bit.moe = DISABLE;
				break;
			}
			case TIM_8:
			{
				TIM8->BDTR.bit.moe = DISABLE;
				break;
			}
		}
	}

	// disable timer
	TIM_PeriphControl(pTIMHandle, DISABLE);

	// set channel state
	tim_channel_state_set(pTIMHandle, Channel, TIM_CH_STATE_READY);
}


/*
 * Timer Input Capture functions
 */
/********************************************************************************************************/
/* @function name 		- TIM_IC_Init																	*/
/*																										*/
/* @brief				- This function initializes given TIM Base according to the specified parameters*/
/*						   in the TIM_Handle_t and initializes the associated handle					*/
/*																										*/
/* @parameter[in]		- pointer to TIM Input Compare handle base address								*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_IC_Init(TIM_Handle_t *pTIMHandle)
{
	if(NULL == pTIMHandle)
	{
		return;
	}

	//set TIM state
	pTIMHandle->State = TIM_STATE_BUSY;

	// initialize base time for IC
	tim_base_setConfig(pTIMHandle);

	//initialize DMA burst
	pTIMHandle->DMABurstState = DMA_BURST_STATE_READY;

	// initialize the TIM channels state
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_READY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_READY);

	// initialize the TIM state
	pTIMHandle->State = TIM_STATE_READY;
}


/********************************************************************************************************/
/* @function name 		- TIM_IC_DeInit																	*/
/*																										*/
/* @brief				- This function de-initializes given TIM Input Compare							*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_IC_DeInit(TIM_Handle_t *pTIMHandle)
{
	//set busy state
	pTIMHandle->State = TIM_STATE_BUSY;

	//disable TIM peripheral clock
	if(TIM_1 == pTIMHandle->TIMx)
	{
		TIM1->CCER.reg = RESET;
	}else if(TIM_2 == pTIMHandle->TIMx)
	{
		TIM2->CCER.reg = RESET;
	}else if(TIM_3 == pTIMHandle->TIMx)
	{
		TIM3->CCER.reg = RESET;
	}else if(TIM_4 == pTIMHandle->TIMx)
	{
		TIM4->CCER.reg = RESET;
	}else if(TIM_5 == pTIMHandle->TIMx)
	{
		TIM5->CCER.reg = RESET;
	}else if(TIM_8 == pTIMHandle->TIMx)
	{
		TIM8->CCER.reg = RESET;
	}else if(TIM_9 == pTIMHandle->TIMx)
	{
		TIM9->CCER.reg = RESET;
	}else if(TIM_10 == pTIMHandle->TIMx)
	{
		TIM10->CCER.reg = RESET;
	}else if(TIM_11 == pTIMHandle->TIMx)
	{
		TIM11->CCER.reg = RESET;
	}else if(TIM_12 == pTIMHandle->TIMx)
	{
		TIM12->CCER.reg = RESET;
	}else if(TIM_13 == pTIMHandle->TIMx)
	{
		TIM13->CCER.reg = RESET;
	}else if(TIM_14 == pTIMHandle->TIMx)
	{
		TIM14->CCER.reg = RESET;
	}

	TIM_PeriphControl(pTIMHandle, DISABLE);

	//change the DMA burst operation state
	pTIMHandle->DMABurstState = DMA_BURST_STATE_RESET;

	// change the TIM channels state
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_RESET);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_ALL, TIM_CH_STATE_RESET);

	// change the TIM state
	pTIMHandle->State = TIM_STATE_RESET;
}


/********************************************************************************************************/
/* @function name 		- TIM_IC_Start																	*/
/*																										*/
/* @brief				- This function starts TIM Input Compare signal generation for given timer		*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM channel to be enabled														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_IC_Start(TIM_Handle_t *pTIMHandle, uint8_t Channel)
{
	if((TIM_CHANNEL_1 == pTIMHandle->Channel) && ((TIM_CH_STATE_READY != pTIMHandle->ChannelState[0])
			|| (TIM_CH_STATE_READY != pTIMHandle->ChannelNState[0])))
	{
		return;
	}else if((TIM_CHANNEL_2 == pTIMHandle->Channel) && ((TIM_CH_STATE_READY != pTIMHandle->ChannelState[1])
			|| (TIM_CH_STATE_READY != pTIMHandle->ChannelNState[1])))
	{
		return;
	}else if((TIM_CHANNEL_3 == pTIMHandle->Channel) && ((TIM_CH_STATE_READY != pTIMHandle->ChannelState[2])
			|| (TIM_CH_STATE_READY != pTIMHandle->ChannelNState[2])))
	{
		return;
	}else if((TIM_CHANNEL_4 == pTIMHandle->Channel) && ((TIM_CH_STATE_READY != pTIMHandle->ChannelState[3])
			|| (TIM_CH_STATE_READY != pTIMHandle->ChannelNState[3])))
	{
		return;
	}

	//set channel state
	tim_channel_state_set(pTIMHandle, Channel, TIM_CH_STATE_BUSY);

	//enable the Input Compare channel
	tim_CCxChannelCmd(pTIMHandle, Channel, ENABLE);

	//enable TIM peripheral
	TIM_PeriphControl(pTIMHandle, ENABLE);
}


/********************************************************************************************************/
/* @function name 		- TIM_IC_Stop																	*/
/*																										*/
/* @brief				- This function stops TIM Input Compare signal generation for given timer		*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM channel to be disabled													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_IC_Stop(TIM_Handle_t *pTIMHandle, uint8_t Channel)
{
	// disable input compare channel
	tim_CCxChannelCmd(pTIMHandle, Channel, DISABLE);

	// disable TIM peripheral
	TIM_PeriphControl(pTIMHandle, DISABLE);

	// set the TIM channel state
	tim_channel_state_set(pTIMHandle, Channel, TIM_CH_STATE_READY);
	tim_channel_n_state_set(pTIMHandle, Channel, TIM_CH_STATE_READY);
}


/********************************************************************************************************/
/* @function name 		- TIM_IC_StartIT																*/
/*																										*/
/* @brief				- This function starts TIM Input Compare signal generation for given timer in 	*/
/* 						  interrupt	mode																*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM channel to be enabled														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_IC_StartIT(TIM_Handle_t *pTIMHandle, uint8_t Channel)
{
	if((TIM_CHANNEL_1 == pTIMHandle->Channel) && ((TIM_CH_STATE_READY != pTIMHandle->ChannelState[0])
			|| (TIM_CH_STATE_READY != pTIMHandle->ChannelNState[0])))
	{
		return;
	}else if((TIM_CHANNEL_2 == pTIMHandle->Channel) && ((TIM_CH_STATE_READY != pTIMHandle->ChannelState[1])
			|| (TIM_CH_STATE_READY != pTIMHandle->ChannelNState[1])))
	{
		return;
	}else if((TIM_CHANNEL_3 == pTIMHandle->Channel) && ((TIM_CH_STATE_READY != pTIMHandle->ChannelState[2])
			|| (TIM_CH_STATE_READY != pTIMHandle->ChannelNState[2])))
	{
		return;
	}else if((TIM_CHANNEL_4 == pTIMHandle->Channel) && ((TIM_CH_STATE_READY != pTIMHandle->ChannelState[3])
			|| (TIM_CH_STATE_READY != pTIMHandle->ChannelNState[3])))
	{
		return;
	}

	// set the TIM channel state
	tim_channel_state_set(pTIMHandle, Channel, TIM_CH_STATE_BUSY);
	tim_channel_n_state_set(pTIMHandle, Channel, TIM_CH_STATE_BUSY);

	switch(Channel)
	{
		case TIM_CHANNEL_1:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC1, ENABLE);
			break;
		}
		case TIM_CHANNEL_2:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC2, ENABLE);
			break;
		}
		case TIM_CHANNEL_3:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC3, ENABLE);
			break;
		}
		case TIM_CHANNEL_4:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC4, ENABLE);
			break;
		}
		default:
			break;
	}

	// enable the input compare channel
	tim_CCxChannelCmd(pTIMHandle, Channel, ENABLE);

	TIM_PeriphControl(pTIMHandle, ENABLE);
}


/********************************************************************************************************/
/* @function name 		- TIM_IC_StopIT																	*/
/*																										*/
/* @brief				- This function stops TIM Input Compare signal generation for given timer in 	*/
/* 						  interrupt	mode																*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM channel to be disabled													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_IC_StopIT(TIM_Handle_t *pTIMHandle, uint8_t Channel)
{
	switch(Channel)
	{
		case TIM_CHANNEL_1:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC1, DISABLE);
			break;
		}
		case TIM_CHANNEL_2:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC2, DISABLE);
			break;
		}
		case TIM_CHANNEL_3:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC3, DISABLE);
			break;
		}
		case TIM_CHANNEL_4:
		{
			tim_it_control(pTIMHandle, TIM_IT_CC4, DISABLE);
			break;
		}
		default:
			break;
	}

	tim_CCxChannelCmd(pTIMHandle, Channel, DISABLE);

	// disable TIM
	TIM_PeriphControl(pTIMHandle, DISABLE);

	tim_channel_state_set(pTIMHandle, Channel, TIM_CH_STATE_READY);
	tim_channel_n_state_set(pTIMHandle, Channel, TIM_CH_STATE_READY);
}


/*
 * Timer One Pulse functions
 */
/********************************************************************************************************/
/* @function name 		- TIM_OnePulse_Init																*/
/*																										*/
/* @brief				- This function initializes given TIM One Pulse Base according to the specified */
/* 						  parameters in the TIM_Handle_t and initializes the associated handle			*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- OnePulseMode @TIM_One_Pulse_Mode (TIM_OPMODE_REPETITIVE or TIM_OPMODE_SINGLE)	*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- CH1 and CH2 are reserved and can't be used in this mode 						*/
/********************************************************************************************************/
void TIM_OnePulse_Init(TIM_Handle_t *pTIMHandle, uint32_t OnePulseMode)
{
	//set TIM state
	pTIMHandle->State = TIM_STATE_BUSY;

	tim_base_setConfig(pTIMHandle);

	// configure OPM bit
	switch(pTIMHandle->TIMx)
	{
		case TIM_1:
		{
			TIM1->CR1.bit.opm = OnePulseMode;
			break;
		}
		case TIM_2:
		{
			TIM2->CR1.bit.opm = OnePulseMode;
			break;
		}
		case TIM_3:
		{
			TIM3->CR1.bit.opm = OnePulseMode;
			break;
		}
		case TIM_4:
		{
			TIM4->CR1.bit.opm = OnePulseMode;
			break;
		}
		case TIM_5:
		{
			TIM5->CR1.bit.opm = OnePulseMode;
			break;
		}
		case TIM_6:
		{
			TIM6->CR1.bit.opm = OnePulseMode;
			break;
		}
		case TIM_7:
		{
			TIM7->CR1.bit.opm = OnePulseMode;
			break;
		}
		case TIM_8:
		{
			TIM8->CR1.bit.opm = OnePulseMode;
			break;
		}
		case TIM_9:
		{
			TIM9->CR1.bit.opm = OnePulseMode;
			break;
		}
		case TIM_10:
		{
			TIM10->CR1.bit.opm = OnePulseMode;
			break;
		}
		case TIM_11:
		{
			TIM11->CR1.bit.opm = OnePulseMode;
			break;
		}
		case TIM_12:
		{
			TIM12->CR1.bit.opm = OnePulseMode;
			break;
		}
		case TIM_13:
		{
			TIM13->CR1.bit.opm = OnePulseMode;
			break;
		}
		case TIM_14:
		{
			TIM14->CR1.bit.opm = OnePulseMode;
			break;
		}
		default:
			break;
	}

	// initialize the DMA burst operation state
	pTIMHandle->DMABurstState = DMA_BURST_STATE_READY;

	// initialize the TIM channels state
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_1, TIM_CH_STATE_READY);
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_2, TIM_CH_STATE_READY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_1, TIM_CH_STATE_READY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_2, TIM_CH_STATE_READY);

	// initialize the TIM state
	pTIMHandle->State = TIM_STATE_READY;
}


/********************************************************************************************************/
/* @function name 		- TIM_OnePulse_DeInit															*/
/*																										*/
/* @brief				- This function de-initializes given TIM One Pulse Mode							*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_OnePulse_DeInit(TIM_Handle_t *pTIMHandle)
{
	//set busy state
	pTIMHandle->State = TIM_STATE_BUSY;

	//disable TIM peripheral clock
	if(TIM_1 == pTIMHandle->TIMx)
	{
		TIM1->CCER.reg = RESET;
	}else if(TIM_2 == pTIMHandle->TIMx)
	{
		TIM2->CCER.reg = RESET;
	}else if(TIM_3 == pTIMHandle->TIMx)
	{
		TIM3->CCER.reg = RESET;
	}else if(TIM_4 == pTIMHandle->TIMx)
	{
		TIM4->CCER.reg = RESET;
	}else if(TIM_5 == pTIMHandle->TIMx)
	{
		TIM5->CCER.reg = RESET;
	}else if(TIM_8 == pTIMHandle->TIMx)
	{
		TIM8->CCER.reg = RESET;
	}else if(TIM_9 == pTIMHandle->TIMx)
	{
		TIM9->CCER.reg = RESET;
	}else if(TIM_10 == pTIMHandle->TIMx)
	{
		TIM10->CCER.reg = RESET;
	}else if(TIM_11 == pTIMHandle->TIMx)
	{
		TIM11->CCER.reg = RESET;
	}else if(TIM_12 == pTIMHandle->TIMx)
	{
		TIM12->CCER.reg = RESET;
	}else if(TIM_13 == pTIMHandle->TIMx)
	{
		TIM13->CCER.reg = RESET;
	}else if(TIM_14 == pTIMHandle->TIMx)
	{
		TIM14->CCER.reg = RESET;
	}

	TIM_PeriphControl(pTIMHandle, DISABLE);

	//change the DMA burst operation state
	pTIMHandle->DMABurstState = DMA_BURST_STATE_RESET;

	// change the TIM channels state
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_1, TIM_CH_STATE_RESET);
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_2, TIM_CH_STATE_RESET);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_1, TIM_CH_STATE_RESET);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_2, TIM_CH_STATE_RESET);

	// change the TIM state
	pTIMHandle->State = TIM_STATE_RESET;
}


/********************************************************************************************************/
/* @function name 		- TIM_OnePulse_Start															*/
/*																										*/
/* @brief				- This function starts TIM One Pulse signal generation for given timer			*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_OnePulse_Start(TIM_Handle_t *pTIMHandle)
{
	if((TIM_CHANNEL_1 == pTIMHandle->Channel) && ((TIM_CH_STATE_READY != pTIMHandle->ChannelState[0])
			|| (TIM_CH_STATE_READY != pTIMHandle->ChannelNState[0])))
	{
		return;
	}
	if((TIM_CHANNEL_2 == pTIMHandle->Channel) && ((TIM_CH_STATE_READY != pTIMHandle->ChannelState[1])
			|| (TIM_CH_STATE_READY != pTIMHandle->ChannelNState[1])))
	{
		return;
	}

	//set channel state busy
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_1, TIM_CH_STATE_BUSY);
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_2, TIM_CH_STATE_BUSY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_1, TIM_CH_STATE_BUSY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_2, TIM_CH_STATE_BUSY);

	/* enable the capture compare and the input compare channels,
	   if TIM_CHANNEL_1 is used as output, TIM_CHANNEL_2 will be used as input and vice versa;
	   TIM_CHANNEL_1 and TIM_CHANNEL_2 both must be enabled together */
	tim_CCxChannelCmd(pTIMHandle, TIM_CHANNEL_1, ENABLE);
	tim_CCxChannelCmd(pTIMHandle, TIM_CHANNEL_2, ENABLE);

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		//enable main output
		switch(pTIMHandle->TIMx)
		{
		case TIM_1:
		{
			TIM1->BDTR.bit.moe = ENABLE;
			break;
		}
		case TIM_8:
		{
			TIM8->BDTR.bit.moe = ENABLE;
			break;
		}
		}
	}
}


/********************************************************************************************************/
/* @function name 		- TIM_OnePulse_Stop																*/
/*																										*/
/* @brief				- This function stops TIM One Pulse signal generation for given timer			*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_OnePulse_Stop(TIM_Handle_t *pTIMHandle)
{
	/* disable the capture compare and the input capture channels,
	   if TIM_CHANNEL_1 is used as output, TIM_CHANNEL_2 will be used as input and vice versa;
	   TIM_CHANNEL_1 and TIM_CHANNEL_2 both must be enabled together*/

	tim_CCxChannelCmd(pTIMHandle, TIM_CHANNEL_1, DISABLE);
	tim_CCxChannelCmd(pTIMHandle, TIM_CHANNEL_2, DISABLE);

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		//disable main output
		switch(pTIMHandle->TIMx)
		{
		case TIM_1:
		{
			TIM1->BDTR.bit.moe = DISABLE;
			break;
		}
		case TIM_8:
		{
			TIM8->BDTR.bit.moe = DISABLE;
			break;
		}
		}
	}

	// disable TIM
	TIM_PeriphControl(pTIMHandle, DISABLE);

	//set channel state ready
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_1, TIM_CH_STATE_READY);
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_2, TIM_CH_STATE_READY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_1, TIM_CH_STATE_READY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_2, TIM_CH_STATE_READY);
}


/********************************************************************************************************/
/* @function name 		- TIM_OnePulse_StartIT															*/
/*																										*/
/* @brief				- This function starts TIM One Pulse signal generation for given timer in 		*/
/* 						  interrupt	mode																*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_OnePulse_StartIT(TIM_Handle_t *pTIMHandle)
{
	if((TIM_CHANNEL_1 == pTIMHandle->Channel) && ((TIM_CH_STATE_READY != pTIMHandle->ChannelState[0])
			|| (TIM_CH_STATE_READY != pTIMHandle->ChannelNState[0])))
	{
		return;
	}
	if((TIM_CHANNEL_2 == pTIMHandle->Channel) && ((TIM_CH_STATE_READY != pTIMHandle->ChannelState[1])
			|| (TIM_CH_STATE_READY != pTIMHandle->ChannelNState[1])))
	{
		return;
	}

	// set channel state busy
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_1, TIM_CH_STATE_BUSY);
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_2, TIM_CH_STATE_BUSY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_1, TIM_CH_STATE_BUSY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_2, TIM_CH_STATE_BUSY);

	/* enable the capture compare and the input compare channels,
		   if TIM_CHANNEL_1 is used as output, TIM_CHANNEL_2 will be used as input and vice versa;
		   TIM_CHANNEL_1 and TIM_CHANNEL_2 both must be enabled together */

	// enable capture compare 1 interrupt
	tim_it_control(pTIMHandle, TIM_IT_CC1, ENABLE);

	// enable capture compare 2 interrupt
	tim_it_control(pTIMHandle, TIM_IT_CC2, ENABLE);

	// enable the output compare channel
	tim_CCxChannelCmd(pTIMHandle, TIM_CHANNEL_1, ENABLE);
	tim_CCxChannelCmd(pTIMHandle, TIM_CHANNEL_2, ENABLE);

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		// enable main output
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->BDTR.bit.moe = ENABLE;
				break;
			}
			case TIM_8:
			{
				TIM8->BDTR.bit.moe = ENABLE;
				break;
			}
		}
	}
}


/********************************************************************************************************/
/* @function name 		- TIM_OnePulse_StopIT															*/
/*																										*/
/* @brief				- This function stops TIM One Pulse signal generation for given timer in 		*/
/* 						  interrupt	mode																*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_OnePulse_StopIT(TIM_Handle_t *pTIMHandle)
{
	// disable capture compare 1 interrupt
	tim_it_control(pTIMHandle, TIM_IT_CC1, DISABLE);

	// disable capture compare 2 interrupt
	tim_it_control(pTIMHandle, TIM_IT_CC2, DISABLE);

	/* disable the capture compare and the input capture channels,
	   if TIM_CHANNEL_1 is used as output, TIM_CHANNEL_2 will be used as input and vice versa;
	   TIM_CHANNEL_1 and TIM_CHANNEL_2 both must be enabled together*/

	tim_CCxChannelCmd(pTIMHandle, TIM_CHANNEL_1, DISABLE);
	tim_CCxChannelCmd(pTIMHandle, TIM_CHANNEL_2, DISABLE);

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		//disable main output
		switch(pTIMHandle->TIMx)
		{
		case TIM_1:
		{
			TIM1->BDTR.bit.moe = DISABLE;
			break;
		}
		case TIM_8:
		{
			TIM8->BDTR.bit.moe = DISABLE;
			break;
		}
		}
	}

	// disable TIM
	TIM_PeriphControl(pTIMHandle, DISABLE);

	//set channel state ready
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_1, TIM_CH_STATE_READY);
	tim_channel_state_set(pTIMHandle, TIM_CHANNEL_2, TIM_CH_STATE_READY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_1, TIM_CH_STATE_READY);
	tim_channel_n_state_set(pTIMHandle, TIM_CHANNEL_2, TIM_CH_STATE_READY);
}


/*
 * Timer Interrupt Handler functions
 */
/********************************************************************************************************/
/* @function name 		- TIM_GetFlagStatus																*/
/*																										*/
/* @brief				- TIM status register flag function												*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- flag name, refer to @TIM flags												*/
/*																										*/
/* @return				- flag state																	*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t TIM_GetFlagStatus(TIM_Handle_t *pTIMHandle, TIM_SR_flag_t StatusFlagName)
{
	if(TIM_SR_UPDATE == StatusFlagName)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->SR.bit.uif;
				break;
			}
			case TIM_2:
			{
				return TIM2->SR.bit.uif;
				break;
			}
			case TIM_3:
			{
				return TIM3->SR.bit.uif;
				break;
			}
			case TIM_4:
			{
				return TIM4->SR.bit.uif;
				break;
			}
			case TIM_5:
			{
				return TIM5->SR.bit.uif;
				break;
			}
			case TIM_6:
			{
				return TIM6->SR.bit.uif;
				break;
			}
			case TIM_7:
			{
				return TIM7->SR.bit.uif;
				break;
			}
			case TIM_8:
			{
				return TIM8->SR.bit.uif;
				break;
			}
			case TIM_9:
			{
				return TIM9->SR.bit.uif;
				break;
			}
			case TIM_10:
			{
				return TIM10->SR.bit.uif;
				break;
			}
			case TIM_11:
			{
				return TIM11->SR.bit.uif;
				break;
			}
			case TIM_12:
			{
				return TIM12->SR.bit.uif;
				break;
			}
			case TIM_13:
			{
				return TIM13->SR.bit.uif;
				break;
			}
			case TIM_14:
			{
				return TIM14->SR.bit.uif;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_SR_CC1 == StatusFlagName)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->SR.bit.cc1if;
				break;
			}
			case TIM_2:
			{
				return TIM2->SR.bit.cc1if;
				break;
			}
			case TIM_3:
			{
				return TIM3->SR.bit.cc1if;
				break;
			}
			case TIM_4:
			{
				return TIM4->SR.bit.cc1if;
				break;
			}
			case TIM_5:
			{
				return TIM5->SR.bit.cc1if;
				break;
			}
			case TIM_8:
			{
				return TIM8->SR.bit.cc1if;
				break;
			}
			case TIM_9:
			{
				return TIM9->SR.bit.cc1if;
				break;
			}
			case TIM_10:
			{
				return TIM10->SR.bit.cc1if;
				break;
			}
			case TIM_11:
			{
				return TIM11->SR.bit.cc1if;
				break;
			}
			case TIM_12:
			{
				return TIM12->SR.bit.cc1if;
				break;
			}
			case TIM_13:
			{
				return TIM13->SR.bit.cc1if;
				break;
			}
			case TIM_14:
			{
				return TIM14->SR.bit.cc1if;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_SR_CC2 == StatusFlagName)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->SR.bit.cc2if;
				break;
			}
			case TIM_2:
			{
				return TIM2->SR.bit.cc2if;
				break;
			}
			case TIM_3:
			{
				return TIM3->SR.bit.cc2if;
				break;
			}
			case TIM_4:
			{
				return TIM4->SR.bit.cc2if;
				break;
			}
			case TIM_5:
			{
				return TIM5->SR.bit.cc2if;
				break;
			}
			case TIM_8:
			{
				return TIM8->SR.bit.cc2if;
				break;
			}
			case TIM_9:
			{
				return TIM9->SR.bit.cc2if;
				break;
			}
			case TIM_12:
			{
				return TIM12->SR.bit.cc2if;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_SR_CC3 == StatusFlagName)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->SR.bit.cc3if;
				break;
			}
			case TIM_2:
			{
				return TIM2->SR.bit.cc3if;
				break;
			}
			case TIM_3:
			{
				return TIM3->SR.bit.cc3if;
				break;
			}
			case TIM_4:
			{
				return TIM4->SR.bit.cc3if;
				break;
			}
			case TIM_5:
			{
				return TIM5->SR.bit.cc3if;
				break;
			}
			case TIM_8:
			{
				return TIM8->SR.bit.cc3if;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_SR_CC4 == StatusFlagName)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->SR.bit.cc4if;
				break;
			}
			case TIM_2:
			{
				return TIM2->SR.bit.cc4if;
				break;
			}
			case TIM_3:
			{
				return TIM3->SR.bit.cc4if;
				break;
			}
			case TIM_4:
			{
				return TIM4->SR.bit.cc4if;
				break;
			}
			case TIM_5:
			{
				return TIM5->SR.bit.cc4if;
				break;
			}
			case TIM_8:
			{
				return TIM8->SR.bit.cc4if;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_SR_COM == StatusFlagName)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->SR.bit.comif;
				break;
			}
			case TIM_8:
			{
				return TIM8->SR.bit.comif;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_SR_TRIGGER == StatusFlagName)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->SR.bit.tif;
				break;
			}
			case TIM_2:
			{
				return TIM2->SR.bit.tif;
				break;
			}
			case TIM_3:
			{
				return TIM3->SR.bit.tif;
				break;
			}
			case TIM_4:
			{
				return TIM4->SR.bit.tif;
				break;
			}
			case TIM_5:
			{
				return TIM5->SR.bit.tif;
				break;
			}
			case TIM_8:
			{
				return TIM8->SR.bit.tif;
				break;
			}
			case TIM_9:
			{
				return TIM9->SR.bit.tif;
				break;
			}
			case TIM_12:
			{
				return TIM12->SR.bit.tif;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_SR_BREAK == StatusFlagName)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->SR.bit.bif;
				break;
			}
			case TIM_8:
			{
				return TIM8->SR.bit.bif;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_SR_CC1OF == StatusFlagName)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->SR.bit.cc1of;
				break;
			}
			case TIM_2:
			{
				return TIM2->SR.bit.cc1of;
				break;
			}
			case TIM_3:
			{
				return TIM3->SR.bit.cc1of;
				break;
			}
			case TIM_4:
			{
				return TIM4->SR.bit.cc1of;
				break;
			}
			case TIM_5:
			{
				return TIM5->SR.bit.cc1of;
				break;
			}
			case TIM_8:
			{
				return TIM8->SR.bit.cc1of;
				break;
			}
			case TIM_9:
			{
				return TIM9->SR.bit.cc1of;
				break;
			}
			case TIM_10:
			{
				return TIM10->SR.bit.cc1of;
				break;
			}
			case TIM_11:
			{
				return TIM11->SR.bit.cc1of;
				break;
			}
			case TIM_12:
			{
				return TIM12->SR.bit.cc1of;
				break;
			}
			case TIM_13:
			{
				return TIM13->SR.bit.cc1of;
				break;
			}
			case TIM_14:
			{
				return TIM14->SR.bit.cc1of;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_SR_CC2OF == StatusFlagName)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->SR.bit.cc2of;
				break;
			}
			case TIM_2:
			{
				return TIM2->SR.bit.cc2of;
				break;
			}
			case TIM_3:
			{
				return TIM3->SR.bit.cc2of;
				break;
			}
			case TIM_4:
			{
				return TIM4->SR.bit.cc2of;
				break;
			}
			case TIM_5:
			{
				return TIM5->SR.bit.cc2of;
				break;
			}
			case TIM_8:
			{
				return TIM8->SR.bit.cc2of;
				break;
			}
			case TIM_9:
			{
				return TIM9->SR.bit.cc2of;
				break;
			}
			case TIM_12:
			{
				return TIM12->SR.bit.cc2of;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_SR_CC3OF == StatusFlagName)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->SR.bit.cc3of;
				break;
			}
			case TIM_2:
			{
				return TIM2->SR.bit.cc3of;
				break;
			}
			case TIM_3:
			{
				return TIM3->SR.bit.cc3of;
				break;
			}
			case TIM_4:
			{
				return TIM4->SR.bit.cc3of;
				break;
			}
			case TIM_5:
			{
				return TIM5->SR.bit.cc3of;
				break;
			}
			case TIM_8:
			{
				return TIM8->SR.bit.cc3of;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_SR_CC4OF == StatusFlagName)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->SR.bit.cc4of;
				break;
			}
			case TIM_2:
			{
				return TIM2->SR.bit.cc4of;
				break;
			}
			case TIM_3:
			{
				return TIM3->SR.bit.cc4of;
				break;
			}
			case TIM_4:
			{
				return TIM4->SR.bit.cc4of;
				break;
			}
			case TIM_5:
			{
				return TIM5->SR.bit.cc4of;
				break;
			}
			case TIM_8:
			{
				return TIM8->SR.bit.cc4of;
				break;
			}
			default:
				break;
		}
	}
	//wrong input
	return 0x3;
}


/********************************************************************************************************/
/* @function name 		- TIM_IRQHandling																*/
/*																										*/
/* @brief				- TIM interrupt handling function												*/
/*																										*/
/* @parameter[in]		- handle pointer																*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_IRQHandling(TIM_Handle_t *pTIMHandle)
{
	TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};

	TIM_1_8_CCMR2_Reg_t CCMR2temp = {0};

	// first check the SR for capture compare event 1
	if((RESET != TIM_GetFlagStatus(pTIMHandle, TIM_SR_CC1)) && (RESET != TIM_GetITStatus(pTIMHandle, TIM_IT_CC1)))
	{
		tim_it_control(pTIMHandle, TIM_IT_CC1, RESET);
		pTIMHandle->Channel = TIM_ACTIVE_CHANNEL_1;

		CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);

		//input capture event
		if(RESET != CCMR1temp.bit.cc1s)
		{
			TIM_IC_CaptureCallback(pTIMHandle);
		}

		//output capture event
		else
		{
			TIM_OC_DelayElapsedCallback(pTIMHandle);
			TIM_PWM_PulseFinishedCallback(pTIMHandle);
		}

		pTIMHandle->Channel = TIM_ACTIVE_CHANNEL_CLEARED;
	}

	// check the SR for capture compare event 2
	if((RESET != TIM_GetFlagStatus(pTIMHandle, TIM_SR_CC2)) && (RESET != TIM_GetITStatus(pTIMHandle, TIM_IT_CC2)))
	{
		tim_it_control(pTIMHandle, TIM_IT_CC2, RESET);
		pTIMHandle->Channel = TIM_ACTIVE_CHANNEL_2;

		CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);

		//input capture event
		if(RESET != CCMR1temp.bit.cc2s)
		{
			TIM_IC_CaptureCallback(pTIMHandle);
		}

		//output capture event
		else
		{
			TIM_OC_DelayElapsedCallback(pTIMHandle);
			TIM_PWM_PulseFinishedCallback(pTIMHandle);
		}
	}

	//check the SR for capture compare event 3
	if((RESET != TIM_GetFlagStatus(pTIMHandle, TIM_SR_CC3)) && (RESET != TIM_GetITStatus(pTIMHandle, TIM_IT_CC3)))
	{
		tim_it_control(pTIMHandle, TIM_IT_CC3, RESET);
		pTIMHandle->Channel = TIM_ACTIVE_CHANNEL_3;

		CCMR2temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR2);

		//input capture event
		if(RESET != CCMR2temp.bit.cc3s)
		{
			TIM_IC_CaptureCallback(pTIMHandle);
		}

		//output capture event
		else
		{
			TIM_OC_DelayElapsedCallback(pTIMHandle);
			TIM_PWM_PulseFinishedCallback(pTIMHandle);
		}

		pTIMHandle->Channel = TIM_ACTIVE_CHANNEL_CLEARED;
	}

	//check the SR for capture compare event 4
	if((RESET != TIM_GetFlagStatus(pTIMHandle, TIM_SR_CC4)) && (RESET != TIM_GetITStatus(pTIMHandle, TIM_IT_CC4)))
	{
		tim_it_control(pTIMHandle, TIM_IT_CC4, RESET);
		pTIMHandle->Channel = TIM_ACTIVE_CHANNEL_4;

		CCMR2temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR2);

		//input capture event
		if(RESET != CCMR2temp.bit.cc4s)
		{
			TIM_IC_CaptureCallback(pTIMHandle);
		}

		//output capture event
		else
		{
			TIM_OC_DelayElapsedCallback(pTIMHandle);
			TIM_PWM_PulseFinishedCallback(pTIMHandle);
		}

		pTIMHandle->Channel = TIM_ACTIVE_CHANNEL_CLEARED;
	}

	// timer update event
	if((RESET != TIM_GetFlagStatus(pTIMHandle, TIM_SR_UPDATE)) && (RESET != TIM_GetITStatus(pTIMHandle, TIM_IT_UPDATE)))
	{
		tim_it_control(pTIMHandle, TIM_IT_UPDATE, RESET);
		TIM_PeriodElapsedCallback(pTIMHandle);
	}

	//timer break input event
	if((RESET != TIM_GetFlagStatus(pTIMHandle, TIM_SR_BREAK)) && (RESET != TIM_GetITStatus(pTIMHandle, TIM_IT_BREAK)))
	{
		tim_it_control(pTIMHandle, TIM_IT_BREAK, RESET);
		//insert time break callback
	}

	// timer trigger detection event
	if((RESET != TIM_GetFlagStatus(pTIMHandle, TIM_SR_TRIGGER)) && (RESET != TIM_GetITStatus(pTIMHandle, TIM_IT_TRIGGER)))
	{
		tim_it_control(pTIMHandle, TIM_IT_TRIGGER, RESET);
		TIM_TriggerCallback(pTIMHandle);
	}

	//timer commutation event
	if((RESET != TIM_GetFlagStatus(pTIMHandle, TIM_SR_COM)) && (RESET != TIM_GetITStatus(pTIMHandle, TIM_IT_COM)))
	{
		tim_it_control(pTIMHandle, TIM_IT_COM, RESET);
		//insert commutation callback if needed
	}

}


/*
 * Control functions
 */
/********************************************************************************************************/
/* @function name 		- TIM_OC_ConfigChannel															*/
/*																										*/
/* @brief				- This function initializes the TIM Output Compare Channels according to the 	*/
/* 	 	 	 	 	 	  specified parameters in the TIM_Handle_t and initializes the associated handle*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM Output Compare configuration structure									*/
/*																										*/
/* @parameter[in]		- TIM Channel to configure														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_OC_ConfigChannel(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *sConfig, uint8_t Channel)
{
	switch(Channel)
	{
		case TIM_CHANNEL_1:
		{
			// configure TIM Channel 1 in output compare
			tim_oc1_set_config(pTIMHandle, sConfig);
			break;
		}
		case TIM_CHANNEL_2:
		{
			// configure TIM Channel 2 in output compare
			tim_oc2_set_config(pTIMHandle, sConfig);
			break;
		}
		case TIM_CHANNEL_3:
		{
			// configure TIM Channel 2 in output compare
			tim_oc3_set_config(pTIMHandle, sConfig);
			break;
		}
		case TIM_CHANNEL_4:
		{
			// configure TIM Channel 2 in output compare
			tim_oc4_set_config(pTIMHandle, sConfig);
			break;
		}
		default:
			break;
	}
}


/********************************************************************************************************/
/* @function name 		- TIM_PWM_ConfigChannel															*/
/*																										*/
/* @brief				- This function initializes the TIM PWM Channels according to the specified		*/
/* 						  parameters in the TIM_Handle_t and initializes the associated handle			*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM PWM configuration structure												*/
/*																										*/
/* @parameter[in]		- TIM Channel to configure														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_PWM_ConfigChannel(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *sConfig, uint8_t Channel)
{
	TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
	TIM_1_8_CCMR2_Reg_t CCMR2temp = {0};

	switch(Channel)
	{
		case TIM_CHANNEL_1:
		{
			// configure TIM Channel 1 in output compare
			tim_oc1_set_config(pTIMHandle, sConfig);

			CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);

			// set Preload enable bit
			CCMR1temp.bit.oc1pe = SET;

			// configure the output fast mode
			CCMR1temp.bit.oc1fe = sConfig->OCFastMode;

			tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);
			break;
		}
		case TIM_CHANNEL_2:
		{
			// configure TIM Channel 2 in output compare
			tim_oc2_set_config(pTIMHandle, sConfig);

			CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);

			// set Preload enable bit
			CCMR1temp.bit.oc2pe = SET;

			// configure the output fast mode
			CCMR1temp.bit.oc2fe = sConfig->OCFastMode;

			tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);
			break;
		}
		case TIM_CHANNEL_3:
		{
			// configure TIM Channel 2 in output compare
			tim_oc3_set_config(pTIMHandle, sConfig);

			CCMR2temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR2);

			// set Preload enable bit
			CCMR2temp.bit.oc3pe = SET;

			// configure the output fast mode
			CCMR2temp.bit.oc3fe = sConfig->OCFastMode;

			tim_write_to_ccmrx(pTIMHandle, TIM_CCMR2, CCMR2temp.reg);
			break;
		}
		case TIM_CHANNEL_4:
		{
			// configure TIM Channel 2 in output compare
			tim_oc4_set_config(pTIMHandle, sConfig);

			CCMR2temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR2);

			// set Preload enable bit
			CCMR2temp.bit.oc4pe = SET;

			// configure the output fast mode
			CCMR2temp.bit.oc4fe = sConfig->OCFastMode;

			tim_write_to_ccmrx(pTIMHandle, TIM_CCMR2, CCMR2temp.reg);
			break;
		}
		default:
			break;
	}
}


/********************************************************************************************************/
/* @function name 		- TIM_IC_ConfigChannel															*/
/*																										*/
/* @brief				- This function initializes the TIM Input Capture Channels according to the 	*/
/* 						  specified parameters in the TIM_Handle_t and initializes the associated handle*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM Input Capture configuration structure										*/
/*																										*/
/* @parameter[in]		- TIM Channel to configure														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_IC_ConfigChannel(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *sConfig, uint8_t Channel)
{
	if(TIM_CHANNEL_1 == Channel)
	{
		//configure TI1
		tim_ti1_set_config(pTIMHandle, sConfig->ICPolarity, sConfig->ICSelection, sConfig->ICFilter);

		TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
		CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);

		//reset IC1PSC bits
		CCMR1temp.bit.oc1fe = RESET;
		CCMR1temp.bit.oc1pe = RESET;

		CCMR1temp.bit.oc1fe = sConfig->ICPrescaler;

		tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);
	}
	else if(TIM_CHANNEL_2 == Channel)
	{
		// configure TI2
		tim_ti2_set_config(pTIMHandle, sConfig->ICPolarity, sConfig->ICSelection, sConfig->ICFilter);

		TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
		CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);

		//reset IC2PSC bits
		CCMR1temp.bit.oc2fe = RESET;
		CCMR1temp.bit.oc2pe = RESET;

		CCMR1temp.bit.oc2fe = sConfig->ICPrescaler;

		tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);
	}
	else if(TIM_CHANNEL_3)
	{
		// configure TI3
		tim_ti3_set_config(pTIMHandle, sConfig->ICPolarity, sConfig->ICSelection, sConfig->ICFilter);

		TIM_1_8_CCMR2_Reg_t CCMR2temp = {0};
		CCMR2temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR2);

		//reset IC3PSC bits
		CCMR2temp.bit.oc3fe = RESET;
		CCMR2temp.bit.oc3pe = RESET;

		CCMR2temp.bit.oc3fe = sConfig->ICPrescaler;

		tim_write_to_ccmrx(pTIMHandle, TIM_CCMR2, CCMR2temp.reg);
	}
	else if(TIM_CHANNEL_4)
	{
		// configure TI4
		tim_ti4_set_config(pTIMHandle, sConfig->ICPolarity, sConfig->ICSelection, sConfig->ICFilter);

		TIM_1_8_CCMR2_Reg_t CCMR2temp = {0};
		CCMR2temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR2);

		//reset IC4PSC bits
		CCMR2temp.bit.oc4fe = RESET;
		CCMR2temp.bit.oc4pe = RESET;

		CCMR2temp.bit.oc4fe = sConfig->ICPrescaler;

		tim_write_to_ccmrx(pTIMHandle, TIM_CCMR2, CCMR2temp.reg);
	}
}


/********************************************************************************************************/
/* @function name 		- TIM_OnePulse_ConfigChannel													*/
/*																										*/
/* @brief				- This function initializes the TIM One Pulse Channels according to the 		*/
/*						  specified parameters in the TIM_Handle_t and initializes the associated handle*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM One Pulse configuration structure											*/
/*																										*/
/* @parameter[in]		- TIM Output Channel to configure												*/
/*																										*/
/* @parameter[in]		- TIM Input Channel to configure												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_OnePulse_ConfigChannel(TIM_Handle_t *pTIMHandle, TIM_OnePulse_Config_t *sConfig,
		uint8_t OutputChannel,  uint8_t InputChannel)
{
	TIM_OC_Config_t temp1 = {0};

	if (OutputChannel != InputChannel)
	{
		pTIMHandle->State = TIM_STATE_BUSY;

		// get OC configuration from sConfig
		temp1.OCMode = sConfig->OCMode;
		temp1.Pulse = sConfig->Pulse;
		temp1.OCPolarity = sConfig->OCPolarity;
		temp1.OCNPolarity = sConfig->OCNPolarity;
		temp1.OCIdleState = sConfig->OCIdleState;
		temp1.OCNIdleState = sConfig->OCNIdleState;

		switch (OutputChannel)
		{
		  case TIM_CHANNEL_1:
		  {
			tim_oc1_set_config(pTIMHandle, &temp1);
			break;
		  }
		  case TIM_CHANNEL_2:
		  {
			tim_oc2_set_config(pTIMHandle, &temp1);
			break;
		  }

		  default:
			break;
		}

		switch(InputChannel)
		{
			case TIM_CHANNEL_1:
			{
				tim_ti1_set_config(pTIMHandle, sConfig->ICPolarity,sConfig->ICSelection, sConfig->ICFilter);

				TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
				CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);

				//reset IC1PSC bits
				CCMR1temp.bit.oc1fe = RESET;
				CCMR1temp.bit.oc1pe = RESET;

				tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);

				// select trigger source
				TIM_1_8_SMCR_Reg_t SMCRtemp = {0};

				SMCRtemp.bit.ts = RESET;
				//set slave mode trigger
				SMCRtemp.bit.ts = 0x5U;	// 101: Filtered Timer Input 1 (TI1FP1)

				tim_write_to_smcr(pTIMHandle, SMCRtemp.reg);
				break;
			}
		    case TIM_CHANNEL_2:
		    {

		    	tim_ti2_set_config(pTIMHandle, sConfig->ICPolarity,sConfig->ICSelection, sConfig->ICFilter);

		    	TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
				CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);

				//reset IC2PSC bits
				CCMR1temp.bit.oc2fe = RESET;
				CCMR1temp.bit.oc2pe = RESET;

				tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);

				// select trigger source
				TIM_1_8_SMCR_Reg_t SMCRtemp = {0};

				SMCRtemp.bit.ts = RESET;
				//set trigger selection to Filtered Timer Input 2
				SMCRtemp.bit.ts = 0x6U;

				SMCRtemp.bit.sms = RESET;
				//set slave mode trigger
				SMCRtemp.bit.sms = 0x6U; // 110: Trigger Mode

				tim_write_to_smcr(pTIMHandle, SMCRtemp.reg);

		    	break;
		    }
			  default:
				break;
		}
	}

	pTIMHandle->State = TIM_STATE_READY;
}


/********************************************************************************************************/
/* @function name 		- TIM_ConfigOCrefClear															*/
/*																										*/
/* @brief				- This function Configures the OCRef clear feature								*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- pointer to a TIM_Clear_Input_Config_t structure that contains the OCREF 		*/
/*						  clear feature and parameters for the TIM peripheral							*/
/*																										*/
/* @parameter[in]		- TIM Channel																	*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_ConfigOCrefClear(TIM_Handle_t *pTIMHandle, TIM_Clear_Input_Config_t *sClearInputConfig, uint8_t Channel)
{
	pTIMHandle->State = TIM_STATE_BUSY;

	switch (sClearInputConfig->ClearInputSource)
	{
		case TIM_CLEARINPUTSOURCE_NONE:
		{
			TIM_1_8_SMCR_Reg_t SMCRtemp = {0};

			SMCRtemp.reg = tim_read_smcr(pTIMHandle);

			//clear the OCREF clear selection bit and the the ETR Bits
			SMCRtemp.bit.etf = RESET;
			SMCRtemp.bit.etps = RESET;
			SMCRtemp.bit.ece = RESET;
			SMCRtemp.bit.etp = RESET;

			tim_write_to_smcr(pTIMHandle, SMCRtemp.reg);
			break;
		}
		case TIM_CLEARINPUTSOURCE_ETR:
		{
			if(0 != sClearInputConfig->ClearInputPrescaler)
			{
				pTIMHandle->State = TIM_STATE_READY;
			}

			tim_etr_set_config(pTIMHandle, sClearInputConfig->ClearInputPrescaler,
							               sClearInputConfig->ClearInputPolarity,
										   sClearInputConfig->ClearInputFilter);
			break;
		}
		default:
			break;
	}

	switch(Channel)
	{
		case TIM_CHANNEL_1:
		{
			TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
			CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);

			if(((uint32_t)0) != sClearInputConfig->ClearInputState)
			{

				// enable the OCREF clear feature for Channel 1
				CCMR1temp.bit.oc1ce = SET;
			}
			else
			{
				// disable the OCREF clear feature for Channel 1
				CCMR1temp.bit.oc1ce = RESET;
			}

			tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);
			break;
		}
		case TIM_CHANNEL_2:
		{
			TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
			CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);

			if(((uint32_t)0) != sClearInputConfig->ClearInputState)
			{

				// enable the OCREF clear feature for Channel 2
				CCMR1temp.bit.oc2ce = SET;
			}
			else
			{
				// disable the OCREF clear feature for Channel 2
				CCMR1temp.bit.oc2ce = RESET;
			}

			tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);
			break;
		}
		case TIM_CHANNEL_3:
		{
			TIM_1_8_CCMR2_Reg_t CCMR2temp = {0};
			CCMR2temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR2);

			if(((uint32_t)0) != sClearInputConfig->ClearInputState)
			{

				// enable the OCREF clear feature for Channel 3
				CCMR2temp.bit.oc3ce = SET;
			}
			else
			{
				// disable the OCREF clear feature for Channel 3
				CCMR2temp.bit.oc3ce = RESET;
			}

			tim_write_to_ccmrx(pTIMHandle, TIM_CCMR2, CCMR2temp.reg);
			break;
		}
		case TIM_CHANNEL_4:
		{
			TIM_1_8_CCMR2_Reg_t CCMR2temp = {0};
			CCMR2temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR2);

			if(((uint32_t)0) != sClearInputConfig->ClearInputState)
			{

				// enable the OCREF clear feature for Channel 4
				CCMR2temp.bit.oc4ce = SET;
			}
			else
			{
				// disable the OCREF clear feature for Channel 4
				CCMR2temp.bit.oc4ce = RESET;
			}

			tim_write_to_ccmrx(pTIMHandle, TIM_CCMR2, CCMR2temp.reg);
			break;
		}
		default:
			break;
	}

	pTIMHandle->State = TIM_STATE_READY;
}


/********************************************************************************************************/
/* @function name 		- TIM_ConfigClockSource															*/
/*																										*/
/* @brief				- configures the clock source to be used 										*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- pointer to a TIM_Clock_Config_t structure that contains the clock source 		*/
/*						  information for the TIM peripheral.	@TIM_Clock_Source						*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_ConfigClockSource(TIM_Handle_t *pTIMHandle, TIM_Clock_Config_t *sClockSourceConfig)
{
	TIM_1_8_SMCR_Reg_t SMCRtemp = {0};

	pTIMHandle->State = TIM_STATE_BUSY;

	// reset the SMS, TS, ECE, ETPS and ETRF bits
	SMCRtemp.bit.sms = RESET;
	SMCRtemp.bit.ts = RESET;
	SMCRtemp.bit.ece = RESET;
	SMCRtemp.bit.etps = RESET;
	SMCRtemp.bit.etf = RESET;
	SMCRtemp.bit.etp = RESET;

	tim_write_to_smcr(pTIMHandle, SMCRtemp.reg);

	SMCRtemp.reg = tim_read_smcr(pTIMHandle);

	switch(sClockSourceConfig->ClockSource)
	{
		case TIM_CLOCKSOURCE_INTERNAL:
		{
			break;
		}
		case TIM_CLOCKSOURCE_ETRMODE1:
		{
			// configure the ETR Clock source
			tim_etr_set_config(pTIMHandle,
					sClockSourceConfig->ClockPrescaler,
                    sClockSourceConfig->ClockPolarity,
                    sClockSourceConfig->ClockFilter);

			// select the external clock mode1 and the ETRF trigger
			SMCRtemp.bit.sms = 0x7U;  // External Clock Mode 1
			SMCRtemp.bit.ts = 0x7U;	  // 111: External Trigger input (ETRF)

			// write to SMCR
			tim_write_to_smcr(pTIMHandle, SMCRtemp.reg);
			break;
		}
		case TIM_CLOCKSOURCE_ETRMODE2:
		{
			// configure the ETR Clock source
			tim_etr_set_config(pTIMHandle,
							   sClockSourceConfig->ClockPrescaler,
							   sClockSourceConfig->ClockPolarity,
							   sClockSourceConfig->ClockFilter);

			// enable the External clock mode2
			SMCRtemp.bit.ece = SET;

			tim_write_to_smcr(pTIMHandle, SMCRtemp.reg);
			break;
		}
		case TIM_CLOCKSOURCE_TI1:
		{
			tim_ti1_config_input_stage(pTIMHandle,
                    sClockSourceConfig->ClockPolarity,
                    sClockSourceConfig->ClockFilter);
			tim_itrx_set_config(pTIMHandle, TIM_CLOCKSOURCE_TI1);
			break;
		}
		case TIM_CLOCKSOURCE_TI2:
		{
			tim_ti2_config_input_stage(pTIMHandle, sClockSourceConfig->ClockPolarity, sClockSourceConfig->ClockFilter);

			tim_itrx_set_config(pTIMHandle, TIM_CLOCKSOURCE_TI2);
			break;
		}
		case TIM_CLOCKSOURCE_TI1ED:
		{
			tim_ti1_config_input_stage(pTIMHandle,
			                    sClockSourceConfig->ClockPolarity,
			                    sClockSourceConfig->ClockFilter);
			tim_itrx_set_config(pTIMHandle, TIM_CLOCKSOURCE_TI1ED);
			break;
		}
		case TIM_CLOCKSOURCE_ITR0:
		case TIM_CLOCKSOURCE_ITR1:
		case TIM_CLOCKSOURCE_ITR2:
		case TIM_CLOCKSOURCE_ITR3:
		{
			tim_itrx_set_config(pTIMHandle, sClockSourceConfig->ClockSource);
			break;
		}
		default:
		{
			break;
		}
		pTIMHandle->State = TIM_STATE_READY;
	}
}


/********************************************************************************************************/
/* @function name 		- TIM_ConfigTI1Input															*/
/*																										*/
/* @brief				- selects the signal connected to the TI1 input: direct from CH1_input			*/
/*         				   or a XOR combination between CH1_input, CH2_input & CH3_input				*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- indicator of whether or not channel 1 is connected to the output of a XOR gate*/
/*						  @TIM_TI1_Selection															*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_ConfigTI1Input(TIM_Handle_t *pTIMHandle, uint32_t TI1_Selection)
{
	TIM_1_8_CR2_Reg_t CR2temp = {0};

	// get the CR2 register value
	CR2temp.reg = tim_read_cr2(pTIMHandle);

	// set TI1
	CR2temp.bit.ti1s = TI1_Selection;

	// write to CR2
	tim_write_to_cr2(pTIMHandle, CR2temp.reg);
}


/********************************************************************************************************/
/* @function name 		- TIM_SlaveConfigSynchro														*/
/*																										*/
/* @brief				- configures the TIM in Slave mode												*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- sSlaveConfig pointer to a TIM_Slave_Config_t structure						*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_SlaveConfigSynchro(TIM_Handle_t *pTIMHandle, TIM_Slave_Config_t *sSlaveConfig)
{
	pTIMHandle->State = TIM_STATE_BUSY;

	if (0 != tim_slave_timer_set_config(pTIMHandle, sSlaveConfig))
	{
		pTIMHandle->State = TIM_STATE_READY;
		return;
	}

	// disable trigger interrupt
	tim_it_control(pTIMHandle, TIM_IT_TRIGGER, DISABLE);

	// disable trigger DMA request
	tim_it_control(pTIMHandle, TIM_IT_DMA_TRIGGER, DISABLE);

	pTIMHandle->State = TIM_STATE_READY;
}


/********************************************************************************************************/
/* @function name 		- TIM_SlaveConfigSynchro_IT														*/
/*																										*/
/* @brief				- configures the TIM in Slave mode in interrupt mode							*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- sSlaveConfig pointer to a TIM_Slave_Config_t structure						*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM_SlaveConfigSynchro_IT(TIM_Handle_t *pTIMHandle, TIM_Slave_Config_t *sSlaveConfig)
{
	pTIMHandle->State = TIM_STATE_BUSY;

	if(0 != tim_slave_timer_set_config(pTIMHandle, sSlaveConfig))
	{
		pTIMHandle->State = TIM_STATE_READY;
		return;
	}

	// enable trigger interrupt
	tim_it_control(pTIMHandle, TIM_IT_TRIGGER, ENABLE);

	//disable trigger DMA request
	tim_it_control(pTIMHandle, TIM_IT_DMA_TRIGGER, DISABLE);

	pTIMHandle->State = TIM_STATE_READY;
}


/********************************************************************************************************/
/* @function name 		- TIM_GenerateEvent																*/
/*																										*/
/* @brief				- generates software event														*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- @TIM_Event_Source																*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- basic timers can only generate an update event.TIM_EVENTSOURCE_COM is 		*/
/* 						 relevant only with advanced timer instances. TIM_EVENTSOURCE_BREAK are relevant*/
/*						  only for timer instances														*/
/********************************************************************************************************/
void TIM_GenerateEvent(TIM_Handle_t *pTIMHandle, uint32_t EventSource)
{
	TIM_1_8_EGR_Reg_t EGRtemp = {0};

	pTIMHandle->State = TIM_STATE_BUSY;

	// set the event sources

	switch(EventSource)
	{
		case TIM_EVENTSOURCE_UPDATE:
		{
			EGRtemp.bit.ug = SET;
			break;
		}
		case TIM_EVENTSOURCE_CC1:
		{
			EGRtemp.bit.cc1g = SET;
			break;
		}
		case TIM_EVENTSOURCE_CC2:
		{
			EGRtemp.bit.cc2g = SET;
			break;
		}
		case TIM_EVENTSOURCE_CC3:
		{
			EGRtemp.bit.cc3g = SET;
			break;
		}
		case TIM_EVENTSOURCE_CC4:
		{
			EGRtemp.bit.cc4g = SET;
			break;
		}
		case TIM_EVENTSOURCE_COM:
		{
			EGRtemp.bit.comg = SET;
			break;
		}
		case TIM_EVENTSOURCE_TRIGGER:
		{
			EGRtemp.bit.tg = SET;
			break;
		}
		case TIM_EVENTSOURCE_BREAK:
		{
			EGRtemp.bit.bg = SET;
			break;
		}
		default:
			break;
	}

	tim_write_to_egr(pTIMHandle, EGRtemp.reg);

	pTIMHandle->State = TIM_STATE_READY;
}


/********************************************************************************************************/
/* @function name 		- TIM_ReadCapturedValue															*/
/*																										*/
/* @brief				- read the captured value from Capture Compare unit								*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @parameter[in]		- TIM Channels to be enabled													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint32_t TIM_ReadCapturedValue(TIM_Handle_t *pTIMHandle, uint32_t Channel)
{
	uint32_t tempreg = 0U;

	switch(Channel)
	{
		case TIM_CHANNEL_1:
		{
			tempreg = tim_read_ccrx(pTIMHandle, TIM_CCR1);
			break;
		}
		case TIM_CHANNEL_2:
		{
			tempreg = tim_read_ccrx(pTIMHandle, TIM_CCR2);
			break;
		}
		case TIM_CHANNEL_3:
		{
			tempreg = tim_read_ccrx(pTIMHandle, TIM_CCR3);
			break;
		}
		case TIM_CHANNEL_4:
		{
			tempreg = tim_read_ccrx(pTIMHandle, TIM_CCR4);
			break;
		}
		default:
			break;
	}
	return tempreg;
}

/*
 * Callback in Interrupt and DMA modes
 */
/********************************************************************************************************/
/* @function name 		- TIM_PeriodElapsedCallback														*/
/*																										*/
/* @brief				- Period elapsed callback in non-blocking mode									*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
__weak void TIM_PeriodElapsedCallback(TIM_Handle_t *pTIMHandle)
{
	//weak implementation
}

/********************************************************************************************************/
/* @function name 		- TIM_PeriodElapsedHalfCpltCallback												*/
/*																										*/
/* @brief				- Period elapsed half complete callback in non-blocking mode					*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
__weak void TIM_PeriodElapsedHalfCpltCallback(TIM_Handle_t *pTIMHandle)
{
	//weak implementation
}


/********************************************************************************************************/
/* @function name 		- TIM_OC_DelayElapsedCallback													*/
/*																										*/
/* @brief				- Output Compare callback in non-blocking mode									*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
__weak void TIM_OC_DelayElapsedCallback(TIM_Handle_t *pTIMHandle)
{
	//weak implementation
}


/********************************************************************************************************/
/* @function name 		- TIM_IC_CaptureCallback														*/
/*																										*/
/* @brief				- Input Capture callback in non-blocking mode									*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
__weak void TIM_IC_CaptureCallback(TIM_Handle_t *pTIMHandle)
{
	//weak implementation
}


/********************************************************************************************************/
/* @function name 		- TIM_IC_CaptureHalfCpltCallback												*/
/*																										*/
/* @brief				- Input Capture half complete callback in non-blocking mode						*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
__weak void TIM_IC_CaptureHalfCpltCallback(TIM_Handle_t *pTIMHandle)
{
	//weak implementation
}


/********************************************************************************************************/
/* @function name 		- TIM_PWM_PulseFinishedCallback													*/
/*																										*/
/* @brief				- PWM Pulse finished callback in non-blocking mode								*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
__weak void TIM_PWM_PulseFinishedCallback(TIM_Handle_t *pTIMHandle)
{
	//weak implementation
}


/********************************************************************************************************/
/* @function name 		- TIM_PWM_PulseFinishedHalfCpltCallback											*/
/*																										*/
/* @brief				- PWM Pulse finished half complete callback in non-blocking mode				*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
__weak void TIM_PWM_PulseFinishedHalfCpltCallback(TIM_Handle_t *pTIMHandle)
{
	//weak implementation
}


/********************************************************************************************************/
/* @function name 		- TIM_TriggerCallback															*/
/*																										*/
/* @brief				- Hall Trigger detection callback in non-blocking mode							*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
__weak void TIM_TriggerCallback(TIM_Handle_t *pTIMHandle)
{
	//weak implementation
}


/********************************************************************************************************/
/* @function name 		- TIM_TriggerHalfCpltCallback													*/
/*																										*/
/* @brief				- Hall Trigger detection half complete callback in non-blocking mode			*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
__weak void TIM_TriggerHalfCpltCallback(TIM_Handle_t *pTIMHandle)
{
	//weak implementation
}



/********************************************************************************************************/
/* @function name 		- TIM_ErrorCallback																*/
/*																										*/
/* @brief				- Timer error callback in non-blocking mode										*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
__weak void TIM_ErrorCallback(TIM_Handle_t *pTIMHandle)
{
	//weak implementation
}


/*
 * Peripheral State functions
 */
/********************************************************************************************************/
/* @function name 		- TIM_Base_GetState																*/
/*																										*/
/* @brief				- return the TIM Base handle state												*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t TIM_Base_GetState(TIM_Handle_t *pTIMHandle)
{
	return pTIMHandle->State;
}


/********************************************************************************************************/
/* @function name 		- TIM_Base_GetState																*/
/*																										*/
/* @brief				- return the TIM OC handle state												*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t TIM_OC_GetState(TIM_Handle_t *pTIMHandle)
{
	  return pTIMHandle->State;
}


/********************************************************************************************************/
/* @function name 		- TIM_PWM_GetState																*/
/*																										*/
/* @brief				- return the TIM PWM handle state												*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t TIM_PWM_GetState(TIM_Handle_t *pTIMHandle)
{
	return pTIMHandle->State;
}


/********************************************************************************************************/
/* @function name 		- TIM_IC_GetState																*/
/*																										*/
/* @brief				- return the TIM Input Capture handle state										*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t TIM_IC_GetState(TIM_Handle_t *pTIMHandle)
{
	return pTIMHandle->State;
}


/********************************************************************************************************/
/* @function name 		- TIM_OnePulse_GetState															*/
/*																										*/
/* @brief				- return the TIM One Pulse Mode handle state									*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t TIM_OnePulse_GetState(TIM_Handle_t *pTIMHandle)
{
	return pTIMHandle->State;
}


/*
 * Channel State functions
 */
/********************************************************************************************************/
/* @function name 		- TIM_GetActiveChannel															*/
/*																										*/
/* @brief				- return the TIM Encoder Mode handle state										*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- active channel																*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
TIM_ActiveChannel_t TIM_GetActiveChannel(TIM_Handle_t *pTIMHandle)
{
	return pTIMHandle->Channel;
}


/********************************************************************************************************/
/* @function name 		- TIM_GetChannelState															*/
/*																										*/
/* @brief				- return actual state of the TIM channel										*/
/*																										*/
/* @parameter[in]		- channel																		*/
/*																										*/
/* @parameter[in]		- pointer to TIM handle base address											*/
/*																										*/
/* @return				- active channel																*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t TIM_GetChannelState(TIM_Handle_t *pTIMHandle,  uint8_t Channel)
{
	TIM_Channel_t channel_state;

	channel_state = pTIMHandle->ChannelState[pTIMHandle->Channel/4];

	return channel_state;
}



//API helper functions
static void tim_base_setConfig(TIM_Handle_t *pTIMHandle)
{
/******************************** Configuration of TIM Base ******************************************/
	if(TIM_1 == pTIMHandle->TIMx)
	{
		TIM_1_8_CR1_Reg_t TIM1_CR1_temp = {0};

		TIM1_CR1_temp.reg = TIM1->CR1.reg;

		//set TIM time base unit parameters
		TIM1_CR1_temp.bit.dir = pTIMHandle->TIM_Config.CounterMode;
		TIM1_CR1_temp.bit.cms = pTIMHandle->TIM_Config.CenterAlignedMode;
		TIM1_CR1_temp.bit.ckd = pTIMHandle->TIM_Config.ClockDivision;
		TIM1_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM1->CR1.reg = TIM1_CR1_temp.reg;

		TIM1->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM1->PSC = pTIMHandle->TIM_Config.Prescaler;

		TIM1->RCR.bit.rep = pTIMHandle->TIM_Config.RepetitionCounter;
		TIM1->EGR.bit.ug = ENABLE;
	}else if(TIM_2 == pTIMHandle->TIMx)
	{
		TIM_2_5_CR1_Reg_t TIM2_CR1_temp = {0};

		TIM2_CR1_temp.reg = TIM2->CR1.reg;

		//set TIM time base unit parameters
		TIM2_CR1_temp.bit.dir = pTIMHandle->TIM_Config.CounterMode;
		TIM2_CR1_temp.bit.cms = pTIMHandle->TIM_Config.CenterAlignedMode;
		TIM2_CR1_temp.bit.ckd = pTIMHandle->TIM_Config.ClockDivision;
		TIM2_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM2->CR1.reg = TIM2_CR1_temp.reg;

		TIM2->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM2->PSC = pTIMHandle->TIM_Config.Prescaler;


		TIM2->EGR.bit.ug = ENABLE;
	}else if(TIM_3 == pTIMHandle->TIMx)
	{
		TIM_2_5_CR1_Reg_t TIM3_CR1_temp = {0};

		TIM3_CR1_temp.reg = TIM3->CR1.reg;

		//set TIM time base unit parameters
		TIM3_CR1_temp.bit.dir = pTIMHandle->TIM_Config.CounterMode;
		TIM3_CR1_temp.bit.cms = pTIMHandle->TIM_Config.CenterAlignedMode;
		TIM3_CR1_temp.bit.ckd = pTIMHandle->TIM_Config.ClockDivision;
		TIM3_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM3->CR1.reg = TIM3_CR1_temp.reg;

		TIM3->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM3->PSC = pTIMHandle->TIM_Config.Prescaler;

		TIM3->EGR.bit.ug = ENABLE;
	}else if(TIM_4 == pTIMHandle->TIMx)
	{
		TIM_2_5_CR1_Reg_t TIM4_CR1_temp = {0};

		TIM4_CR1_temp.reg = TIM4->CR1.reg;

		//set TIM time base unit parameters
		TIM4_CR1_temp.bit.dir = pTIMHandle->TIM_Config.CounterMode;
		TIM4_CR1_temp.bit.cms = pTIMHandle->TIM_Config.CenterAlignedMode;
		TIM4_CR1_temp.bit.ckd = pTIMHandle->TIM_Config.ClockDivision;
		TIM4_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM4->CR1.reg = TIM4_CR1_temp.reg;

		TIM4->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM4->PSC = pTIMHandle->TIM_Config.Prescaler;

		TIM4->EGR.bit.ug = ENABLE;
	}else if(TIM_5 == pTIMHandle->TIMx)
	{
		TIM_2_5_CR1_Reg_t TIM5_CR1_temp = {0};

		TIM5_CR1_temp.reg = TIM5->CR1.reg;

		//set TIM time base unit parameters
		TIM5_CR1_temp.bit.dir = pTIMHandle->TIM_Config.CounterMode;
		TIM5_CR1_temp.bit.cms = pTIMHandle->TIM_Config.CenterAlignedMode;
		TIM5_CR1_temp.bit.ckd = pTIMHandle->TIM_Config.ClockDivision;
		TIM5_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM5->CR1.reg = TIM5_CR1_temp.reg;

		TIM5->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM5->PSC = pTIMHandle->TIM_Config.Prescaler;

		TIM5->EGR.bit.ug = ENABLE;
	}else if(TIM_6 == pTIMHandle->TIMx)
	{
		TIM_6_7_CR1_Reg_t TIM6_CR1_temp = {0};

		TIM6_CR1_temp.reg = TIM6->CR1.reg;

		//set TIM time base unit parameters
		TIM6_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM6->CR1.reg = TIM6_CR1_temp.reg;

		TIM6->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM6->PSC = pTIMHandle->TIM_Config.Prescaler;

		TIM6->EGR.bit.ug = ENABLE;
	}else if(TIM_7 == pTIMHandle->TIMx)
	{
		TIM_6_7_CR1_Reg_t TIM7_CR1_temp = {0};

		TIM7_CR1_temp.reg = TIM7->CR1.reg;

		//set TIM time base unit parameters
		TIM7_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM7->CR1.reg = TIM7_CR1_temp.reg;

		TIM7->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM7->PSC = pTIMHandle->TIM_Config.Prescaler;

		TIM7->EGR.bit.ug = ENABLE;
	}else if(TIM_8 == pTIMHandle->TIMx)
	{
		TIM_1_8_CR1_Reg_t TIM8_CR1_temp = {0};

		TIM8_CR1_temp.reg = TIM8->CR1.reg;

		//set TIM time base unit parameters
		TIM8_CR1_temp.bit.dir = pTIMHandle->TIM_Config.CounterMode;
		TIM8_CR1_temp.bit.cms = pTIMHandle->TIM_Config.CenterAlignedMode;
		TIM8_CR1_temp.bit.ckd = pTIMHandle->TIM_Config.ClockDivision;
		TIM8_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM8->CR1.reg = TIM8_CR1_temp.reg;

		TIM8->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM8->PSC = pTIMHandle->TIM_Config.Prescaler;

		TIM8->RCR.bit.rep = pTIMHandle->TIM_Config.RepetitionCounter;
		TIM8->EGR.bit.ug = ENABLE;
	}else if(TIM_9 == pTIMHandle->TIMx)
	{
		TIM_9_12_CR1_Reg_t TIM9_CR1_temp = {0};

		TIM9_CR1_temp.reg = TIM9->CR1.reg;

		//set TIM time base unit parameters
		TIM9_CR1_temp.bit.ckd = pTIMHandle->TIM_Config.ClockDivision;
		TIM9_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM9->CR1.reg = TIM9_CR1_temp.reg;

		TIM9->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM9->PSC = pTIMHandle->TIM_Config.Prescaler;

		TIM9->EGR.bit.ug = ENABLE;
	}else if(TIM_10 == pTIMHandle->TIMx)
	{
		TIM_10_14_CR1_Reg_t TIM10_CR1_temp = {0};

		TIM10_CR1_temp.reg = TIM10->CR1.reg;

		//set TIM time base unit parameters
		TIM10_CR1_temp.bit.ckd = pTIMHandle->TIM_Config.ClockDivision;
		TIM10_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM10->CR1.reg = TIM10_CR1_temp.reg;

		TIM10->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM10->PSC = pTIMHandle->TIM_Config.Prescaler;

		TIM10->EGR.bit.ug = ENABLE;
	}else if(TIM_11 == pTIMHandle->TIMx)
	{
		TIM_10_14_CR1_Reg_t TIM11_CR1_temp = {0};

		TIM11_CR1_temp.reg = TIM11->CR1.reg;

		//set TIM time base unit parameters
		TIM11_CR1_temp.bit.ckd = pTIMHandle->TIM_Config.ClockDivision;
		TIM11_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM11->CR1.reg = TIM11_CR1_temp.reg;

		TIM11->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM11->PSC = pTIMHandle->TIM_Config.Prescaler;

		TIM11->EGR.bit.ug = ENABLE;
	}else if(TIM_12 == pTIMHandle->TIMx)
	{
		TIM_9_12_CR1_Reg_t TIM12_CR1_temp = {0};

		TIM12_CR1_temp.reg = TIM12->CR1.reg;

		//set TIM time base unit parameters
		TIM12_CR1_temp.bit.ckd = pTIMHandle->TIM_Config.ClockDivision;
		TIM12_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM12->CR1.reg = TIM12_CR1_temp.reg;

		TIM12->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM12->PSC = pTIMHandle->TIM_Config.Prescaler;

		TIM12->EGR.bit.ug = ENABLE;
	}else if(TIM_13 == pTIMHandle->TIMx)
	{
		TIM_10_14_CR1_Reg_t TIM13_CR1_temp = {0};

		TIM13_CR1_temp.reg = TIM13->CR1.reg;

		//set TIM time base unit parameters
		TIM13_CR1_temp.bit.ckd = pTIMHandle->TIM_Config.ClockDivision;
		TIM13_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM13->CR1.reg = TIM13_CR1_temp.reg;

		TIM13->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM13->PSC = pTIMHandle->TIM_Config.Prescaler;

		TIM13->EGR.bit.ug = ENABLE;
	}else if(TIM_14 == pTIMHandle->TIMx)
	{
		TIM_10_14_CR1_Reg_t TIM14_CR1_temp = {0};

		TIM14_CR1_temp.reg = TIM14->CR1.reg;

		//set TIM time base unit parameters
		TIM14_CR1_temp.bit.ckd = pTIMHandle->TIM_Config.ClockDivision;
		TIM14_CR1_temp.bit.arpe = pTIMHandle->TIM_Config.AutoReloadPreload;

		TIM14->CR1.reg = TIM14_CR1_temp.reg;

		TIM14->ARR = (uint32_t)pTIMHandle->TIM_Config.Period;
		TIM14->PSC = pTIMHandle->TIM_Config.Prescaler;

		TIM14->EGR.bit.ug = ENABLE;
	}
	else return;
}

static void tim_CCxChannelCmd(TIM_Handle_t *pTIMHandle, uint8_t Channel, uint32_t ChannelStateEnorDi)
{
	if(TIM_1 == pTIMHandle->TIMx)
	{
		switch(Channel)
		{
			case TIM_CHANNEL_1:
			{
				TIM1->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_2:
			{
				TIM1->CCER.bit.cc2e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_3:
			{
				TIM1->CCER.bit.cc3e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_4:
			{
				TIM1->CCER.bit.cc4e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_ALL:
			{
				TIM1->CCER.bit.cc1e = ChannelStateEnorDi;
				TIM1->CCER.bit.cc2e = ChannelStateEnorDi;
				TIM1->CCER.bit.cc3e = ChannelStateEnorDi;
				TIM1->CCER.bit.cc4e = ChannelStateEnorDi;
				break;
			}
			default:
				return;
		}
	}else if(TIM_2 == pTIMHandle->TIMx)
	{
		switch(Channel)
		{
			case TIM_CHANNEL_1:
			{
				TIM2->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_2:
			{
				TIM2->CCER.bit.cc2e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_3:
			{
				TIM2->CCER.bit.cc3e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_4:
			{
				TIM2->CCER.bit.cc4e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_ALL:
			{
				TIM2->CCER.bit.cc1e = ChannelStateEnorDi;
				TIM2->CCER.bit.cc2e = ChannelStateEnorDi;
				TIM2->CCER.bit.cc3e = ChannelStateEnorDi;
				TIM2->CCER.bit.cc4e = ChannelStateEnorDi;
				break;
			}
			default:
				return;
		}
	}else if(TIM_3 == pTIMHandle->TIMx)
	{
		switch(Channel)
		{
			case TIM_CHANNEL_1:
			{
				TIM3->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_2:
			{
				TIM3->CCER.bit.cc2e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_3:
			{
				TIM3->CCER.bit.cc3e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_4:
			{
				TIM3->CCER.bit.cc4e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_ALL:
			{
				TIM3->CCER.bit.cc1e = ChannelStateEnorDi;
				TIM3->CCER.bit.cc2e = ChannelStateEnorDi;
				TIM3->CCER.bit.cc3e = ChannelStateEnorDi;
				TIM3->CCER.bit.cc4e = ChannelStateEnorDi;
				break;
			}
			default:
				return;
		}
	}else if(TIM_4 == pTIMHandle->TIMx)
	{
		switch(Channel)
		{
			case TIM_CHANNEL_1:
			{
				TIM4->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_2:
			{
				TIM4->CCER.bit.cc2e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_3:
			{
				TIM4->CCER.bit.cc3e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_4:
			{
				TIM4->CCER.bit.cc4e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_ALL:
			{
				TIM4->CCER.bit.cc1e = ChannelStateEnorDi;
				TIM4->CCER.bit.cc2e = ChannelStateEnorDi;
				TIM4->CCER.bit.cc3e = ChannelStateEnorDi;
				TIM4->CCER.bit.cc4e = ChannelStateEnorDi;
				break;
			}
			default:
				return;
		}
	}else if(TIM_5 == pTIMHandle->TIMx)
	{
		switch(Channel)
		{
			case TIM_CHANNEL_1:
			{
				TIM5->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_2:
			{
				TIM5->CCER.bit.cc2e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_3:
			{
				TIM5->CCER.bit.cc3e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_4:
			{
				TIM5->CCER.bit.cc4e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_ALL:
			{
				TIM5->CCER.bit.cc1e = ChannelStateEnorDi;
				TIM5->CCER.bit.cc2e = ChannelStateEnorDi;
				TIM5->CCER.bit.cc3e = ChannelStateEnorDi;
				TIM5->CCER.bit.cc4e = ChannelStateEnorDi;
				break;
			}
			default:
				return;
		}
	}else if(TIM_8 == pTIMHandle->TIMx)
	{
		switch(Channel)
		{
			case TIM_CHANNEL_1:
			{
				TIM8->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_2:
			{
				TIM8->CCER.bit.cc2e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_3:
			{
				TIM8->CCER.bit.cc3e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_4:
			{
				TIM8->CCER.bit.cc4e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_ALL:
			{
				TIM8->CCER.bit.cc1e = ChannelStateEnorDi;
				TIM8->CCER.bit.cc2e = ChannelStateEnorDi;
				TIM8->CCER.bit.cc3e = ChannelStateEnorDi;
				TIM8->CCER.bit.cc4e = ChannelStateEnorDi;
				break;
			}
			default:
				return;
		}
	}else if(TIM_9 == pTIMHandle->TIMx)
	{
		switch(Channel)
		{
			case TIM_CHANNEL_1:
			{
				TIM9->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_2:
			{
				TIM9->CCER.bit.cc2e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_3:
			{
				return;
			}
			case TIM_CHANNEL_4:
			{
				return;
			}
			case TIM_CHANNEL_ALL:
			{
				TIM9->CCER.bit.cc1e = ChannelStateEnorDi;
				TIM9->CCER.bit.cc2e = ChannelStateEnorDi;
				break;
			}
			default:
				return;
		}
	}else if(TIM_10 == pTIMHandle->TIMx)
	{
		switch(Channel)
		{
			case TIM_CHANNEL_1:
			{
				TIM10->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_2:
			{
				return;
			}
			case TIM_CHANNEL_3:
			{
				return;
			}
			case TIM_CHANNEL_4:
			{
				return;
			}
			case TIM_CHANNEL_ALL:
			{
				TIM10->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			default:
				return;
		}
	}else if(TIM_11 == pTIMHandle->TIMx)
	{
		switch(Channel)
		{
			case TIM_CHANNEL_1:
			{
				TIM11->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_2:
			{
				return;
			}
			case TIM_CHANNEL_3:
			{
				return;
			}
			case TIM_CHANNEL_4:
			{
				return;
			}
			case TIM_CHANNEL_ALL:
			{
				TIM11->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			default:
				return;
		}
	}else if(TIM_12 == pTIMHandle->TIMx)
	{
		switch(Channel)
		{
			case TIM_CHANNEL_1:
			{
				TIM12->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_2:
			{
				TIM12->CCER.bit.cc2e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_3:
			{
				return;
			}
			case TIM_CHANNEL_4:
			{
				return;
			}
			case TIM_CHANNEL_ALL:
			{
				TIM12->CCER.bit.cc1e = ChannelStateEnorDi;
				TIM12->CCER.bit.cc2e = ChannelStateEnorDi;
				break;
			}
			default:
				return;
		}
	}else if(TIM_13 == pTIMHandle->TIMx)
	{
		switch(Channel)
		{
			case TIM_CHANNEL_1:
			{
				TIM13->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_2:
			{
				return;
			}
			case TIM_CHANNEL_3:
			{
				return;
			}
			case TIM_CHANNEL_4:
			{
				return;
			}
			case TIM_CHANNEL_ALL:
			{
				TIM13->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			default:
				return;
		}
	}else if(TIM_14 == pTIMHandle->TIMx)
	{
		switch(Channel)
		{
			case TIM_CHANNEL_1:
			{
				TIM14->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			case TIM_CHANNEL_2:
			{
				return;
			}
			case TIM_CHANNEL_3:
			{
				return;
			}
			case TIM_CHANNEL_4:
			{
				return;
			}
			case TIM_CHANNEL_ALL:
			{
				TIM14->CCER.bit.cc1e = ChannelStateEnorDi;
				break;
			}
			default:
				return;
		}
	}
}


static void tim_channel_state_set(TIM_Handle_t *pTIMHandle, uint8_t Channel, uint32_t ChannelState)
{
	if(TIM_CHANNEL_1 == Channel)
	{
		pTIMHandle->ChannelState[0] = ChannelState;
	}else if(TIM_CHANNEL_2 == Channel)
	{
		pTIMHandle->ChannelState[1] = ChannelState;
	}else if(TIM_CHANNEL_3 == Channel)
	{
		pTIMHandle->ChannelState[2] = ChannelState;
	}else if(TIM_CHANNEL_4 == Channel)
	{
		pTIMHandle->ChannelState[3] = ChannelState;
	}else if(TIM_CHANNEL_ALL == Channel)
	{
		pTIMHandle->ChannelState[0] = ChannelState;
		pTIMHandle->ChannelState[1] = ChannelState;
		pTIMHandle->ChannelState[2] = ChannelState;
		pTIMHandle->ChannelState[3] = ChannelState;
	}
}

static void tim_channel_n_state_set(TIM_Handle_t *pTIMHandle, uint8_t Channel, uint32_t ChannelNState)
{
	if(TIM_CHANNEL_1 == Channel)
	{
		pTIMHandle->ChannelNState[0] = ChannelNState;
	}else if(TIM_CHANNEL_2 == Channel)
	{
		pTIMHandle->ChannelNState[1] = ChannelNState;
	}else if(TIM_CHANNEL_3 == Channel)
	{
		pTIMHandle->ChannelNState[2] = ChannelNState;
	}else if(TIM_CHANNEL_4 == Channel)
	{
		pTIMHandle->ChannelNState[3] = ChannelNState;
	}else if(TIM_CHANNEL_ALL == Channel)
	{
		pTIMHandle->ChannelNState[0] = ChannelNState;
		pTIMHandle->ChannelNState[1] = ChannelNState;
		pTIMHandle->ChannelNState[2] = ChannelNState;
		pTIMHandle->ChannelNState[3] = ChannelNState;
	}
}

static void tim_it_control(TIM_Handle_t *pTIMHandle, uint8_t Interrupt, uint8_t State)
{
	if(TIM_IT_UPDATE == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.uie = State;
				break;
			}
			case TIM_2:
			{
				TIM2->DIER.bit.uie = State;
				break;
			}
			case TIM_3:
			{
				TIM3->DIER.bit.uie = State;
				break;
			}
			case TIM_4:
			{
				TIM4->DIER.bit.uie = State;
				break;
			}
			case TIM_5:
			{
				TIM5->DIER.bit.uie = State;
				break;
			}
			case TIM_6:
			{
				TIM6->DIER.bit.uie = State;
				break;
			}
			case TIM_7:
			{
				TIM7->DIER.bit.uie = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.uie = State;
				break;
			}
			case TIM_9:
			{
				TIM9->DIER.bit.uie = State;
				break;
			}
			case TIM_10:
			{
				TIM10->DIER.bit.uie = State;
				break;
			}
			case TIM_11:
			{
				TIM11->DIER.bit.uie = State;
				break;
			}
			case TIM_12:
			{
				TIM12->DIER.bit.uie = State;
				break;
			}
			case TIM_13:
			{
				TIM13->DIER.bit.uie = State;
				break;
			}
			case TIM_14:
			{
				TIM14->DIER.bit.uie = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_CC1 == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.cc1ie = State;
				break;
			}
			case TIM_2:
			{
				TIM2->DIER.bit.cc1ie = State;
				break;
			}
			case TIM_3:
			{
				TIM3->DIER.bit.cc1ie = State;
				break;
			}
			case TIM_4:
			{
				TIM4->DIER.bit.cc1ie = State;
				break;
			}
			case TIM_5:
			{
				TIM5->DIER.bit.cc1ie = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.cc1ie = State;
				break;
			}
			case TIM_9:
			{
				TIM9->DIER.bit.cc1ie = State;
				break;
			}
			case TIM_10:
			{
				TIM10->DIER.bit.cc1ie = State;
				break;
			}
			case TIM_11:
			{
				TIM11->DIER.bit.cc1ie = State;
				break;
			}
			case TIM_12:
			{
				TIM12->DIER.bit.cc1ie = State;
				break;
			}
			case TIM_13:
			{
				TIM13->DIER.bit.cc1ie = State;
				break;
			}
			case TIM_14:
			{
				TIM14->DIER.bit.cc1ie = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_CC2 == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.cc2ie = State;
				break;
			}
			case TIM_2:
			{
				TIM2->DIER.bit.cc2ie = State;
				break;
			}
			case TIM_3:
			{
				TIM3->DIER.bit.cc2ie = State;
				break;
			}
			case TIM_4:
			{
				TIM4->DIER.bit.cc2ie = State;
				break;
			}
			case TIM_5:
			{
				TIM5->DIER.bit.cc2ie = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.cc2ie = State;
				break;
			}
			case TIM_9:
			{
				TIM9->DIER.bit.cc2ie = State;
				break;
			}
			case TIM_12:
			{
				TIM12->DIER.bit.cc2ie = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_CC3 == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.cc3ie = State;
				break;
			}
			case TIM_2:
			{
				TIM2->DIER.bit.cc3ie = State;
				break;
			}
			case TIM_3:
			{
				TIM3->DIER.bit.cc3ie = State;
				break;
			}
			case TIM_4:
			{
				TIM4->DIER.bit.cc3ie = State;
				break;
			}
			case TIM_5:
			{
				TIM5->DIER.bit.cc3ie = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.cc3ie = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_CC4 == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.cc4ie = State;
				break;
			}
			case TIM_2:
			{
				TIM2->DIER.bit.cc4ie = State;
				break;
			}
			case TIM_3:
			{
				TIM3->DIER.bit.cc4ie = State;
				break;
			}
			case TIM_4:
			{
				TIM4->DIER.bit.cc4ie = State;
				break;
			}
			case TIM_5:
			{
				TIM5->DIER.bit.cc4ie = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.cc4ie = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_COM == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.comie = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.comie = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_TRIGGER == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.tie = State;
				break;
			}
			case TIM_2:
			{
				TIM2->DIER.bit.tie = State;
				break;
			}
			case TIM_3:
			{
				TIM3->DIER.bit.tie = State;
				break;
			}
			case TIM_4:
			{
				TIM4->DIER.bit.tie = State;
				break;
			}
			case TIM_5:
			{
				TIM5->DIER.bit.tie = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.tie = State;
				break;
			}
			case TIM_9:
			{
				TIM9->DIER.bit.tie = State;
				break;
			}
			case TIM_12:
			{
				TIM12->DIER.bit.tie = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_BREAK == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.bie = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.bie = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_DMA_UPDATE == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.ude = State;
				break;
			}
			case TIM_2:
			{
				TIM2->DIER.bit.ude = State;
				break;
			}
			case TIM_3:
			{
				TIM3->DIER.bit.ude = State;
				break;
			}
			case TIM_4:
			{
				TIM4->DIER.bit.ude = State;
				break;
			}
			case TIM_5:
			{
				TIM5->DIER.bit.ude = State;
				break;
			}
			case TIM_6:
			{
				TIM6->DIER.bit.ude = State;
				break;
			}
			case TIM_7:
			{
				TIM7->DIER.bit.ude = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.ude = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_DMA_CC1 == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.cc1de = State;
				break;
			}
			case TIM_2:
			{
				TIM2->DIER.bit.cc1de = State;
				break;
			}
			case TIM_3:
			{
				TIM3->DIER.bit.cc1de = State;
				break;
			}
			case TIM_4:
			{
				TIM4->DIER.bit.cc1de = State;
				break;
			}
			case TIM_5:
			{
				TIM5->DIER.bit.cc1de = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.cc1de = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_DMA_CC2 == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.cc2de = State;
				break;
			}
			case TIM_2:
			{
				TIM2->DIER.bit.cc2de = State;
				break;
			}
			case TIM_3:
			{
				TIM3->DIER.bit.cc2de = State;
				break;
			}
			case TIM_4:
			{
				TIM4->DIER.bit.cc2de = State;
				break;
			}
			case TIM_5:
			{
				TIM5->DIER.bit.cc2de = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.cc2de = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_DMA_CC3 == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.cc3de = State;
				break;
			}
			case TIM_2:
			{
				TIM2->DIER.bit.cc3de = State;
				break;
			}
			case TIM_3:
			{
				TIM3->DIER.bit.cc3de = State;
				break;
			}
			case TIM_4:
			{
				TIM4->DIER.bit.cc3de = State;
				break;
			}
			case TIM_5:
			{
				TIM5->DIER.bit.cc3de = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.cc3de = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_DMA_CC4 == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.cc4de = State;
				break;
			}
			case TIM_2:
			{
				TIM2->DIER.bit.cc4de = State;
				break;
			}
			case TIM_3:
			{
				TIM3->DIER.bit.cc4de = State;
				break;
			}
			case TIM_4:
			{
				TIM4->DIER.bit.cc4de = State;
				break;
			}
			case TIM_5:
			{
				TIM5->DIER.bit.cc4de = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.cc4de = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_DMA_COM == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.comde = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.comde = State;
				break;
			}
			default:
				break;
		}
	}else if(TIM_IT_DMA_TRIGGER == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->DIER.bit.tde = State;
				break;
			}
			case TIM_2:
			{
				TIM2->DIER.bit.tde = State;
				break;
			}
			case TIM_3:
			{
				TIM3->DIER.bit.tde = State;
				break;
			}
			case TIM_4:
			{
				TIM4->DIER.bit.tde = State;
				break;
			}
			case TIM_5:
			{
				TIM5->DIER.bit.tde = State;
				break;
			}
			case TIM_8:
			{
				TIM8->DIER.bit.tde = State;
				break;
			}
			default:
				break;
		}
	}

		return;
}


uint32_t tim_read_ccmrx(TIM_Handle_t *pTIMHandle, uint8_t TIM_CCMRx)
{
	if(TIM_CCMR1 == TIM_CCMRx)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->CCMR1.reg;
				break;
			}
			case TIM_2:
			{
				return TIM2->CCMR1.reg;
				break;
			}
			case TIM_3:
			{
				return TIM3->CCMR1.reg;
				break;
			}
			case TIM_4:
			{
				return TIM4->CCMR1.reg;
				break;
			}
			case TIM_5:
			{
				return TIM5->CCMR1.reg;
				break;
			}
			case TIM_8:
			{
				return TIM8->CCMR1.reg;
				break;
			}
			case TIM_9:
			{
				return TIM9->CCMR1.reg;
				break;
			}
			case TIM_10:
			{
				return TIM10->CCMR1.reg;
				break;
			}
			case TIM_11:
			{
				return TIM11->CCMR1.reg;
				break;
			}
			case TIM_12:
			{
				return TIM12->CCMR1.reg;
				break;
			}
			case TIM_13:
			{
				return TIM13->CCMR1.reg;
				break;
			}
			case TIM_14:
			{
				return TIM14->CCMR1.reg;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_CCMR2 == TIM_CCMRx)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->CCMR2.reg;
				break;
			}
			case TIM_2:
			{
				return TIM2->CCMR2.reg;
				break;
			}
			case TIM_3:
			{
				return TIM3->CCMR2.reg;
				break;
			}
			case TIM_4:
			{
				return TIM4->CCMR2.reg;
				break;
			}
			case TIM_5:
			{
				return TIM5->CCMR2.reg;
				break;
			}
			case TIM_8:
			{
				return TIM8->CCMR2.reg;
				break;
			}
			default:
				break;
		}
	}

	//wrong input
	return 0x0U;
}


uint32_t tim_read_ccer(TIM_Handle_t *pTIMHandle)
{
	switch(pTIMHandle->TIMx)
	{
		case TIM_1:
		{
			return TIM1->CCER.reg;
			break;
		}
		case TIM_2:
		{
			return TIM2->CCER.reg;
			break;
		}
		case TIM_3:
		{
			return TIM3->CCER.reg;
			break;
		}
		case TIM_4:
		{
			return TIM4->CCER.reg;
			break;
		}
		case TIM_5:
		{
			return TIM5->CCER.reg;
			break;
		}
		case TIM_8:
		{
			return TIM8->CCER.reg;
			break;
		}
		case TIM_9:
		{
			return TIM9->CCER.reg;
			break;
		}
		case TIM_10:
		{
			return TIM10->CCER.reg;
			break;
		}
		case TIM_11:
		{
			return TIM11->CCER.reg;
			break;
		}
		case TIM_12:
		{
			return TIM12->CCER.reg;
			break;
		}
		case TIM_13:
		{
			return TIM13->CCER.reg;
			break;
		}
		case TIM_14:
		{
			return TIM14->CCER.reg;
			break;
		}
		default:
			return 0x0U;
	}
}

uint32_t tim_read_cr2(TIM_Handle_t *pTIMHandle)
{
	switch(pTIMHandle->TIMx)
	{
		case TIM_1:
		{
			return TIM1->CR2.reg;
			break;
		}
		case TIM_2:
		{
			return TIM2->CR2.reg;
			break;
		}
		case TIM_3:
		{
			return TIM3->CR2.reg;
			break;
		}
		case TIM_4:
		{
			return TIM4->CR2.reg;
			break;
		}
		case TIM_5:
		{
			return TIM5->CR2.reg;
			break;
		}
		case TIM_6:
		{
			return TIM6->CR2.reg;
			break;
		}
		case TIM_7:
		{
			return TIM7->CR2.reg;
			break;
		}
		case TIM_8:
		{
			return TIM8->CR2.reg;
			break;
		}
		case TIM_9:
		{
			return TIM9->CR2.reg;
			break;
		}
		case TIM_12:
		{
			return TIM12->CR2.reg;
			break;
		}
		default:
			return 0x0U;
	}
}

uint32_t tim_read_smcr(TIM_Handle_t *pTIMHandle)
{
	switch(pTIMHandle->TIMx)
	{
		case TIM_1:
		{
			return TIM1->SMCR.reg;
			break;
		}
		case TIM_2:
		{
			return TIM2->SMCR.reg;
			break;
		}
		case TIM_3:
		{
			return TIM3->SMCR.reg;
			break;
		}
		case TIM_4:
		{
			return TIM4->SMCR.reg;
			break;
		}
		case TIM_5:
		{
			return TIM5->SMCR.reg;
			break;
		}
		case TIM_8:
		{
			return TIM8->SMCR.reg;
			break;
		}
		case TIM_12:
		{
			return TIM12->SMCR.reg;
			break;
		}
		default:
			break;
	}

	return 0;
}

uint32_t tim_read_ccrx(TIM_Handle_t *pTIMHandle, uint8_t TIM_CCRx)
{
	if(TIM_CCR1 == TIM_CCRx)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->CCR1;
				break;
			}
			case TIM_2:
			{
				return TIM2->CCR1;
				break;
			}
			case TIM_3:
			{
				return TIM3->CCR1;
				break;
			}
			case TIM_4:
			{
				return TIM4->CCR1;
				break;
			}
			case TIM_5:
			{
				return TIM5->CCR1;
				break;
			}
			case TIM_8:
			{
				return TIM8->CCR1;
				break;
			}
			case TIM_9:
			{
				return TIM9->CCR1;
				break;
			}
			case TIM_10:
			{
				return TIM10->CCR1;
				break;
			}
			case TIM_11:
			{
				return TIM11->CCR1;
				break;
			}
			case TIM_12:
			{
				return TIM12->CCR1;
				break;
			}
			case TIM_13:
			{
				return TIM13->CCR1;
				break;
			}
			case TIM_14:
			{
				return TIM14->CCR1;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_CCR2 == TIM_CCRx)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->CCR2;
				break;
			}
			case TIM_2:
			{
				return TIM2->CCR2;
				break;
			}
			case TIM_3:
			{
				return TIM3->CCR2;
				break;
			}
			case TIM_4:
			{
				return TIM4->CCR2;
				break;
			}
			case TIM_5:
			{
				return TIM5->CCR2;
				break;
			}
			case TIM_8:
			{
				return TIM8->CCR2;
				break;
			}
			case TIM_9:
			{
				return TIM9->CCR2;
				break;
			}
			case TIM_12:
			{
				return TIM12->CCR2;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_CCR3 == TIM_CCRx)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->CCR3;
				break;
			}
			case TIM_2:
			{
				return TIM2->CCR3;
				break;
			}
			case TIM_3:
			{
				return TIM3->CCR3;
				break;
			}
			case TIM_4:
			{
				return TIM4->CCR3;
				break;
			}
			case TIM_5:
			{
				return TIM5->CCR3;
				break;
			}
			case TIM_8:
			{
				return TIM8->CCR3;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_CCR4 == TIM_CCRx)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->CCR4;
				break;
			}
			case TIM_2:
			{
				return TIM2->CCR4;
				break;
			}
			case TIM_3:
			{
				return TIM3->CCR4;
				break;
			}
			case TIM_4:
			{
				return TIM4->CCR4;
				break;
			}
			case TIM_5:
			{
				return TIM5->CCR4;
				break;
			}
			case TIM_8:
			{
				return TIM8->CCR4;
				break;
			}
			default:
				break;
		}
	}
	return 0;
}


static void tim_write_to_cr2(TIM_Handle_t *pTIMHandle, uint32_t CR2tempReg)
{
	switch(pTIMHandle->TIMx)
	{
		case TIM_1:
		{
			TIM1->CR2.reg = CR2tempReg;
			break;
		}
		case TIM_2:
		{
			TIM2->CR2.reg = CR2tempReg;
			break;
		}
		case TIM_3:
		{
			TIM3->CR2.reg = CR2tempReg;
			break;
		}
		case TIM_4:
		{
			TIM4->CR2.reg = CR2tempReg;
			break;
		}
		case TIM_5:
		{
			TIM5->CR2.reg = CR2tempReg;
			break;
		}
		case TIM_6:
		{
			TIM6->CR2.reg = CR2tempReg;
			break;
		}
		case TIM_7:
		{
			TIM7->CR2.reg = CR2tempReg;
			break;
		}
		case TIM_8:
		{
			TIM8->CR2.reg = CR2tempReg;
			break;
		}
		case TIM_9:
		{
			TIM9->CR2.reg = CR2tempReg;
			break;
		}
		case TIM_12:
		{
			TIM12->CR2.reg = CR2tempReg;
			break;
		}
		default:
			return;
	}
}

static void tim_write_to_ccer(TIM_Handle_t *pTIMHandle, uint32_t CCERtempReg)
{
	switch(pTIMHandle->TIMx)
	{
		case TIM_1:
		{
			TIM1->CCER.reg = CCERtempReg;
			break;
		}
		case TIM_2:
		{
			TIM2->CCER.reg = CCERtempReg;
			break;
		}
		case TIM_3:
		{
			TIM3->CCER.reg = CCERtempReg;
			break;
		}
		case TIM_4:
		{
			TIM4->CCER.reg = CCERtempReg;
			break;
		}
		case TIM_5:
		{
			TIM5->CCER.reg = CCERtempReg;
			break;
		}
		case TIM_8:
		{
			TIM8->CCER.reg = CCERtempReg;
			break;
		}
		case TIM_9:
		{
			TIM9->CCER.reg = CCERtempReg;
			break;
		}
		case TIM_10:
		{
			TIM10->CCER.reg = CCERtempReg;
			break;
		}
		case TIM_11:
		{
			TIM11->CCER.reg = CCERtempReg;
			break;
		}
		case TIM_12:
		{
			TIM12->CCER.reg = CCERtempReg;
			break;
		}
		default:
			return;
	}
}


static void tim_write_to_ccmrx(TIM_Handle_t *pTIMHandle, uint8_t TIM_CCMRx ,uint32_t CCMRxtempReg)
{
	if(TIM_CCMR1 == TIM_CCMRx)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->CCMR1.reg = CCMRxtempReg;
				break;
			}
			case TIM_2:
			{
				TIM2->CCMR1.reg = CCMRxtempReg;
				break;
			}
			case TIM_3:
			{
				TIM3->CCMR1.reg = CCMRxtempReg;
				break;
			}
			case TIM_4:
			{
				TIM4->CCMR1.reg = CCMRxtempReg;
				break;
			}
			case TIM_5:
			{
				TIM5->CCMR1.reg = CCMRxtempReg;
				break;
			}
			case TIM_8:
			{
				TIM8->CCMR1.reg = CCMRxtempReg;
				break;
			}
			case TIM_9:
			{
				TIM9->CCMR1.reg = CCMRxtempReg;
				break;
			}
			case TIM_10:
			{
				TIM10->CCMR1.reg = CCMRxtempReg;
				break;
			}
			case TIM_11:
			{
				TIM11->CCMR1.reg = CCMRxtempReg;
				break;
			}
			case TIM_12:
			{
				TIM12->CCMR1.reg = CCMRxtempReg;
				break;
			}
			case TIM_13:
			{
				TIM13->CCMR1.reg = CCMRxtempReg;
				break;
			}
			case TIM_14:
			{
				TIM14->CCMR1.reg = CCMRxtempReg;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_CCMR2 == TIM_CCMRx)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->CCMR2.reg = CCMRxtempReg;
				break;
			}
			case TIM_2:
			{
				TIM2->CCMR2.reg = CCMRxtempReg;
				break;
			}
			case TIM_3:
			{
				TIM3->CCMR2.reg = CCMRxtempReg;
				break;
			}
			case TIM_4:
			{
				TIM4->CCMR2.reg = CCMRxtempReg;
				break;
			}
			case TIM_5:
			{
				TIM5->CCMR2.reg = CCMRxtempReg;
				break;
			}
			case TIM_8:
			{
				TIM8->CCMR2.reg = CCMRxtempReg;
				break;
			}
			default:
				break;
		}
	}
}


static void tim_write_to_ccrx(TIM_Handle_t *pTIMHandle, uint8_t TIM_CCRx, uint32_t CCRxtemp)
{
	if(TIM_CCR1 == TIM_CCRx)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->CCR1 = CCRxtemp;
				break;
			}
			case TIM_2:
			{
				TIM2->CCR1 = CCRxtemp;
				break;
			}
			case TIM_3:
			{
				TIM3->CCR1 = CCRxtemp;
				break;
			}
			case TIM_4:
			{
				TIM4->CCR1 = CCRxtemp;
				break;
			}
			case TIM_5:
			{
				TIM5->CCR1 = CCRxtemp;
				break;
			}
			case TIM_8:
			{
				TIM8->CCR1 = CCRxtemp;
				break;
			}
			case TIM_9:
			{
				TIM9->CCR1 = CCRxtemp;
				break;
			}
			case TIM_10:
			{
				TIM10->CCR1 = CCRxtemp;
				break;
			}
			case TIM_11:
			{
				TIM11->CCR1 = CCRxtemp;
				break;
			}
			case TIM_12:
			{
				TIM12->CCR1 = CCRxtemp;
				break;
			}
			case TIM_13:
			{
				TIM13->CCR1 = CCRxtemp;
				break;
			}
			case TIM_14:
			{
				TIM14->CCR1 = CCRxtemp;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_CCR2 == TIM_CCRx)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->CCR2 = CCRxtemp;
				break;
			}
			case TIM_2:
			{
				TIM2->CCR2 = CCRxtemp;
				break;
			}
			case TIM_3:
			{
				TIM3->CCR2 = CCRxtemp;
				break;
			}
			case TIM_4:
			{
				TIM4->CCR2 = CCRxtemp;
				break;
			}
			case TIM_5:
			{
				TIM5->CCR2 = CCRxtemp;
				break;
			}
			case TIM_8:
			{
				TIM8->CCR2 = CCRxtemp;
				break;
			}
			case TIM_9:
			{
				TIM9->CCR2 = CCRxtemp;
				break;
			}
			case TIM_12:
			{
				TIM12->CCR2 = CCRxtemp;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_CCR3 == TIM_CCRx)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->CCR3 = CCRxtemp;
				break;
			}
			case TIM_2:
			{
				TIM2->CCR3 = CCRxtemp;
				break;
			}
			case TIM_3:
			{
				TIM3->CCR3 = CCRxtemp;
				break;
			}
			case TIM_4:
			{
				TIM4->CCR3 = CCRxtemp;
				break;
			}
			case TIM_5:
			{
				TIM5->CCR3 = CCRxtemp;
				break;
			}
			case TIM_8:
			{
				TIM8->CCR3 = CCRxtemp;
				break;
			}
			default:
				break;
		}
	}

	if(TIM_CCR4 == TIM_CCRx)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				TIM1->CCR4 = CCRxtemp;
				break;
			}
			case TIM_2:
			{
				TIM2->CCR4 = CCRxtemp;
				break;
			}
			case TIM_3:
			{
				TIM3->CCR4 = CCRxtemp;
				break;
			}
			case TIM_4:
			{
				TIM4->CCR4 = CCRxtemp;
				break;
			}
			case TIM_5:
			{
				TIM5->CCR4 = CCRxtemp;
				break;
			}
			case TIM_8:
			{
				TIM8->CCR4 = CCRxtemp;
				break;
			}
			default:
				break;
		}
	}
}

static void tim_write_to_smcr(TIM_Handle_t *pTIMHandle, uint32_t SMCRtempReg)
{
	switch(pTIMHandle->TIMx)
	{
		case TIM_1:
		{
			TIM1->SMCR.reg = SMCRtempReg;
			break;
		}
		case TIM_2:
		{
			TIM2->SMCR.reg = SMCRtempReg;
			break;
		}
		case TIM_3:
		{
			TIM3->SMCR.reg = SMCRtempReg;
			break;
		}
		case TIM_4:
		{
			TIM4->SMCR.reg = SMCRtempReg;
			break;
		}
		case TIM_5:
		{
			TIM5->SMCR.reg = SMCRtempReg;
			break;
		}
		case TIM_8:
		{
			TIM8->SMCR.reg = SMCRtempReg;
			break;
		}
		case TIM_12:
		{
			TIM12->SMCR.reg = SMCRtempReg;
			break;
		}
		default:
			break;
	}
}


static void tim_write_to_egr(TIM_Handle_t *pTIMHandle, uint32_t EGRtempReg)
{
	switch(pTIMHandle->TIMx)
	{
		case TIM_1:
		{
			TIM1->EGR.reg = EGRtempReg;
			break;
		}
		case TIM_2:
		{
			TIM2->EGR.reg = EGRtempReg;
			break;
		}
		case TIM_3:
		{
			TIM3->EGR.reg = EGRtempReg;
			break;
		}
		case TIM_4:
		{
			TIM4->EGR.reg = EGRtempReg;
			break;
		}
		case TIM_5:
		{
			TIM5->EGR.reg = EGRtempReg;
			break;
		}
		case TIM_6:
		{
			TIM6->EGR.reg = EGRtempReg;
			break;
		}
		case TIM_7:
		{
			TIM7->EGR.reg = EGRtempReg;
			break;
		}
		case TIM_8:
		{
			TIM8->EGR.reg = EGRtempReg;
			break;
		}
		case TIM_9:
		{
			TIM9->EGR.reg = EGRtempReg;
			break;
		}
		case TIM_10:
		{
			TIM10->EGR.reg = EGRtempReg;
			break;
		}
		case TIM_11:
		{
			TIM11->EGR.reg = EGRtempReg;
			break;
		}
		case TIM_12:
		{
			TIM12->EGR.reg = EGRtempReg;
			break;
		}
		case TIM_13:
		{
			TIM13->EGR.reg = EGRtempReg;
			break;
		}
		case TIM_14:
		{
			TIM14->EGR.reg = EGRtempReg;
			break;
		}
		default:
			break;
	}
}


uint8_t TIM_GetITStatus(TIM_Handle_t *pTIMHandle, uint8_t Interrupt)
{
	if(TIM_IT_UPDATE == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->DIER.bit.uie;
				break;
			}
			case TIM_2:
			{
				return TIM2->DIER.bit.uie;
				break;
			}
			case TIM_3:
			{
				return TIM3->DIER.bit.uie;
				break;
			}
			case TIM_4:
			{
				return TIM4->DIER.bit.uie;
				break;
			}
			case TIM_5:
			{
				return TIM5->DIER.bit.uie;
				break;
			}
			case TIM_6:
			{
				return TIM6->DIER.bit.uie;
				break;
			}
			case TIM_7:
			{
				return TIM7->DIER.bit.uie;
				break;
			}
			case TIM_8:
			{
				return TIM8->DIER.bit.uie;
				break;
			}
			case TIM_9:
			{
				return TIM9->DIER.bit.uie;
				break;
			}
			case TIM_10:
			{
				return TIM10->DIER.bit.uie;
				break;
			}
			case TIM_11:
			{
				return TIM11->DIER.bit.uie;
				break;
			}
			case TIM_12:
			{
				return TIM12->DIER.bit.uie;
				break;
			}
			case TIM_13:
			{
				return TIM13->DIER.bit.uie;
				break;
			}
			case TIM_14:
			{
				return TIM14->DIER.bit.uie;
				break;
			}
			default:
				//wrong input
				return 0x3;
				break;
		}
	}else if(TIM_IT_CC1 == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->DIER.bit.cc1ie;
				break;
			}
			case TIM_2:
			{
				return TIM2->DIER.bit.cc1ie;
				break;
			}
			case TIM_3:
			{
				return TIM3->DIER.bit.cc1ie;
				break;
			}
			case TIM_4:
			{
				return TIM4->DIER.bit.cc1ie;
				break;
			}
			case TIM_5:
			{
				return TIM5->DIER.bit.cc1ie;
				break;
			}
			case TIM_8:
			{
				return TIM8->DIER.bit.cc1ie;
				break;
			}
			case TIM_9:
			{
				return TIM9->DIER.bit.cc1ie;
				break;
			}
			case TIM_10:
			{
				return TIM10->DIER.bit.cc1ie;
				break;
			}
			case TIM_11:
			{
				return TIM11->DIER.bit.cc1ie;
				break;
			}
			case TIM_12:
			{
				return TIM12->DIER.bit.cc1ie;
				break;
			}
			case TIM_13:
			{
				return TIM13->DIER.bit.cc1ie;
				break;
			}
			case TIM_14:
			{
				return TIM14->DIER.bit.cc1ie;
				break;
			}
			default:
				//wrong input
				return 0x3;
				break;
		}
	}else if(TIM_IT_CC2 == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->DIER.bit.cc2ie;
				break;
			}
			case TIM_2:
			{
				return TIM2->DIER.bit.cc2ie;
				break;
			}
			case TIM_3:
			{
				return TIM3->DIER.bit.cc2ie;
				break;
			}
			case TIM_4:
			{
				return TIM4->DIER.bit.cc2ie;
				break;
			}
			case TIM_5:
			{
				return TIM5->DIER.bit.cc2ie;
				break;
			}
			case TIM_8:
			{
				return TIM8->DIER.bit.cc2ie;
				break;
			}
			case TIM_9:
			{
				return TIM9->DIER.bit.cc2ie;
				break;
			}
			case TIM_12:
			{
				return TIM12->DIER.bit.cc2ie;
				break;
			}
			default:
				//wrong input
				return 0x3;
				break;
		}
	}else if(TIM_IT_CC3 == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->DIER.bit.cc3ie;
				break;
			}
			case TIM_2:
			{
				return TIM2->DIER.bit.cc3ie;
				break;
			}
			case TIM_3:
			{
				return TIM3->DIER.bit.cc3ie;
				break;
			}
			case TIM_4:
			{
				return TIM4->DIER.bit.cc3ie;
				break;
			}
			case TIM_5:
			{
				return TIM5->DIER.bit.cc3ie;
				break;
			}
			case TIM_8:
			{
				return TIM8->DIER.bit.cc3ie;
				break;
			}
			default:
				//wrong input
				return 0x3;
				break;
		}
	}else if(TIM_IT_CC4 == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->DIER.bit.cc4ie;
				break;
			}
			case TIM_2:
			{
				return TIM2->DIER.bit.cc4ie;
				break;
			}
			case TIM_3:
			{
				return TIM3->DIER.bit.cc4ie;
				break;
			}
			case TIM_4:
			{
				return TIM4->DIER.bit.cc4ie;
				break;
			}
			case TIM_5:
			{
				return TIM5->DIER.bit.cc4ie;
				break;
			}
			case TIM_8:
			{
				return TIM8->DIER.bit.cc4ie;
				break;
			}
			default:
				//wrong input
				return 0x3;
				break;
		}
	}else if(TIM_IT_COM == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->DIER.bit.comie;
				break;
			}
			case TIM_8:
			{
				return TIM8->DIER.bit.comie;
				break;
			}
			default:
				//wrong input
				return 0x3;
				break;
		}
	}else if(TIM_IT_TRIGGER == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->DIER.bit.tie;
				break;
			}
			case TIM_2:
			{
				return TIM2->DIER.bit.tie;
				break;
			}
			case TIM_3:
			{
				return TIM3->DIER.bit.tie;
				break;
			}
			case TIM_4:
			{
				return TIM4->DIER.bit.tie;
				break;
			}
			case TIM_5:
			{
				return TIM5->DIER.bit.tie;
				break;
			}
			case TIM_8:
			{
				return TIM8->DIER.bit.tie;
				break;
			}
			case TIM_9:
			{
				return TIM9->DIER.bit.tie;
				break;
			}
			case TIM_12:
			{
				return TIM12->DIER.bit.tie;
				break;
			}
			default:
				//wrong input
				return 0x3;
				break;
		}
	}else if(TIM_IT_BREAK == Interrupt)
	{
		switch(pTIMHandle->TIMx)
		{
			case TIM_1:
			{
				return TIM1->DIER.bit.bie;
				break;
			}
			case TIM_8:
			{
				return TIM8->DIER.bit.bie;
				break;
			}
			default:
				//wrong input
				return 0x3;
				break;
		}
	}
	//wrong input
	return 0x3;
}

static void tim_oc1_set_config(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *OC_Config)
{
	TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
	TIM_1_8_CCER_Reg_t CCERtemp = {0};
	TIM_1_8_CR2_Reg_t	CR2temp = {0};
	uint32_t CCR1temp = {0};

	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	// disable Channel 1
	CCERtemp.bit.cc1e = RESET;

	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);

	// get CCER register value
	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	//get CR2 register value
	CR2temp.reg = tim_read_cr2(pTIMHandle);

	// get CCMR1 register value
	CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);

	// reset Output Compare Mode bits
	CCMR1temp.bit.oc1m = RESET;
	CCMR1temp.bit.cc1s = RESET;

	// select the output compare mode
	CCMR1temp.bit.oc1m = OC_Config->OCMode;

	//set the output compare polarity
	CCERtemp.bit.cc1p = OC_Config->OCPolarity;

	if(((TIM_1 == pTIMHandle->TIMx) && ((TIM_CHANNEL_1 == pTIMHandle->Channel) || (TIM_CHANNEL_2 == pTIMHandle->Channel)
	|| (TIM_CHANNEL_2 == pTIMHandle->Channel) || (TIM_CHANNEL_3 == pTIMHandle->Channel)))
	|| ((TIM_8 == pTIMHandle->TIMx) && ((TIM_CHANNEL_1 == pTIMHandle->Channel) || (TIM_CHANNEL_2 == pTIMHandle->Channel)
	|| (TIM_CHANNEL_2 == pTIMHandle->Channel) || (TIM_CHANNEL_3 == pTIMHandle->Channel))))
	{
		// set the output N polarity level
		CCERtemp.bit.cc1np = OC_Config->OCNPolarity;

		// reset output N state
		CCERtemp.bit.cc1ne = RESET;
	}

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		// reset output compare and output compare N IDLE state
		CR2temp.bit.ois1 = RESET;
		CR2temp.bit.ois1n = RESET;

		//set output IDLE state
		CR2temp.bit.ois1 = OC_Config->OCIdleState;
		CR2temp.bit.ois1n = OC_Config->OCNIdleState;
	}

	// write to CR2
	tim_write_to_cr2(pTIMHandle, CR2temp.reg);

	//write to CCMR1
	tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);

	// set capture compare register value
	CCR1temp = OC_Config->Pulse;
	tim_write_to_ccrx(pTIMHandle, TIM_CCR1, CCR1temp);

	//write to CCER
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);
}

static void tim_oc2_set_config(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *OC_Config)
{
	TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
	TIM_1_8_CCER_Reg_t CCERtemp = {0};
	TIM_1_8_CR2_Reg_t	CR2temp = {0};
	uint32_t CCR2temp = {0};

	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	// disable Channel 2
	CCERtemp.bit.cc2e = RESET;

	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);

	// get CCER register value
	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	//get CR2 register value
	CR2temp.reg = tim_read_cr2(pTIMHandle);

	// get CCMR1 register value
	CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);

	// reset Output Compare Mode bits
	CCMR1temp.bit.oc2m = RESET;
	CCMR1temp.bit.cc2s = RESET;

	// select the output compare mode
	CCMR1temp.bit.oc2m = OC_Config->OCMode;

	// set output polarity
	CCERtemp.bit.cc2p = OC_Config->OCPolarity;

	if(((TIM_1 == pTIMHandle->TIMx) && ((TIM_CHANNEL_1 == pTIMHandle->Channel) || (TIM_CHANNEL_2 == pTIMHandle->Channel)
	|| (TIM_CHANNEL_2 == pTIMHandle->Channel) || (TIM_CHANNEL_3 == pTIMHandle->Channel)))
	|| ((TIM_8 == pTIMHandle->TIMx) && ((TIM_CHANNEL_1 == pTIMHandle->Channel) || (TIM_CHANNEL_2 == pTIMHandle->Channel)
	|| (TIM_CHANNEL_2 == pTIMHandle->Channel) || (TIM_CHANNEL_3 == pTIMHandle->Channel))))
	{
		// set the output N polarity level
		CCERtemp.bit.cc2np = OC_Config->OCNPolarity;

		// reset output N state
		CCERtemp.bit.cc2ne = RESET;
	}

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		// reset output compare and output compare N IDLE state
		CR2temp.bit.ois2 = RESET;
		CR2temp.bit.ois2n = RESET;

		//set output IDLE state
		CR2temp.bit.ois2 = OC_Config->OCIdleState;
		CR2temp.bit.ois2n = OC_Config->OCNIdleState;
	}

	// write to CR2
	tim_write_to_cr2(pTIMHandle, CR2temp.reg);

	//write to CCMR1
	tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);

	// set capture compare register value
	CCR2temp = OC_Config->Pulse;
	tim_write_to_ccrx(pTIMHandle, TIM_CCR2, CCR2temp);

	//write to CCER
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);
}


static void tim_oc3_set_config(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *OC_Config)
{
	TIM_1_8_CCMR2_Reg_t CCMR2temp = {0};
	TIM_1_8_CCER_Reg_t CCERtemp = {0};
	TIM_1_8_CR2_Reg_t	CR2temp = {0};
	uint32_t CCR3temp = {0};

	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	// disable Channel 3
	CCERtemp.bit.cc3e = RESET;

	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);

	// get CCER register value
	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	//get CR2 register value
	CR2temp.reg = tim_read_cr2(pTIMHandle);

	// get CCMR2 register value
	CCMR2temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR2);

	// reset Output Compare Mode bits
	CCMR2temp.bit.oc3m = RESET;
	CCMR2temp.bit.cc3s = RESET;

	// select the output compare mode
	CCMR2temp.bit.oc3m = OC_Config->OCMode;

	// set output polarity
	CCERtemp.bit.cc3p = OC_Config->OCPolarity;

	if(((TIM_1 == pTIMHandle->TIMx) && ((TIM_CHANNEL_1 == pTIMHandle->Channel) || (TIM_CHANNEL_2 == pTIMHandle->Channel)
	|| (TIM_CHANNEL_2 == pTIMHandle->Channel) || (TIM_CHANNEL_3 == pTIMHandle->Channel)))
	|| ((TIM_8 == pTIMHandle->TIMx) && ((TIM_CHANNEL_1 == pTIMHandle->Channel) || (TIM_CHANNEL_2 == pTIMHandle->Channel)
	|| (TIM_CHANNEL_2 == pTIMHandle->Channel) || (TIM_CHANNEL_3 == pTIMHandle->Channel))))
	{
		// set the output N polarity level
		CCERtemp.bit.cc3np = OC_Config->OCNPolarity;

		// reset output N state
		CCERtemp.bit.cc3ne = RESET;
	}

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		// reset output compare and output compare N IDLE state
		CR2temp.bit.ois3 = RESET;
		CR2temp.bit.ois3n = RESET;

		//set output IDLE state
		CR2temp.bit.ois3 = OC_Config->OCIdleState;
		CR2temp.bit.ois3n = OC_Config->OCNIdleState;
	}

	// write to CR2
	tim_write_to_cr2(pTIMHandle, CR2temp.reg);

	//write to CCMR1
	tim_write_to_ccmrx(pTIMHandle, TIM_CCMR2, CCMR2temp.reg);

	// set capture compare register value
	CCR3temp = OC_Config->Pulse;
	tim_write_to_ccrx(pTIMHandle, TIM_CCR3, CCR3temp);

	//write to CCER
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);
}


static void tim_oc4_set_config(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *OC_Config)
{
	TIM_1_8_CCMR2_Reg_t CCMR2temp = {0};
	TIM_1_8_CCER_Reg_t CCERtemp = {0};
	TIM_1_8_CR2_Reg_t	CR2temp = {0};
	uint32_t CCR4temp = {0};

	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	// disable Channel 4
	CCERtemp.bit.cc4e = RESET;

	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);

	// get CCER register value
	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	//get CR2 register value
	CR2temp.reg = tim_read_cr2(pTIMHandle);

	// get CCMR2 register value
	CCMR2temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR2);

	// reset Output Compare Mode bits
	CCMR2temp.bit.oc4m = RESET;
	CCMR2temp.bit.cc4s = RESET;

	// select the output compare mode
	CCMR2temp.bit.oc4m = OC_Config->OCMode;

	// set output polarity
	CCERtemp.bit.cc4p = OC_Config->OCPolarity;

	if(((TIM_1 == pTIMHandle->TIMx) && ((TIM_CHANNEL_1 == pTIMHandle->Channel) || (TIM_CHANNEL_2 == pTIMHandle->Channel)
	|| (TIM_CHANNEL_2 == pTIMHandle->Channel) || (TIM_CHANNEL_3 == pTIMHandle->Channel)))
	|| ((TIM_8 == pTIMHandle->TIMx) && ((TIM_CHANNEL_1 == pTIMHandle->Channel) || (TIM_CHANNEL_2 == pTIMHandle->Channel)
	|| (TIM_CHANNEL_2 == pTIMHandle->Channel) || (TIM_CHANNEL_3 == pTIMHandle->Channel))))
	{
		// set the output N polarity level
		CCERtemp.bit.cc4np = OC_Config->OCNPolarity;

	}

	if((TIM_1 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		// reset output compare IDLE state
		CR2temp.bit.ois4 = RESET;

		//set output IDLE state
		CR2temp.bit.ois4 = OC_Config->OCIdleState;
	}

	// write to CR2
	tim_write_to_cr2(pTIMHandle, CR2temp.reg);

	//write to CCMR1
	tim_write_to_ccmrx(pTIMHandle, TIM_CCMR2, CCMR2temp.reg);

	// set capture compare register value
	CCR4temp = OC_Config->Pulse;
	tim_write_to_ccrx(pTIMHandle, TIM_CCR4, CCR4temp);

	//write to CCER
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);
}


static void tim_ti1_set_config(TIM_Handle_t *pTIMHandle, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t TIM_ICFilter)
{
	TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
	TIM_1_8_CCER_Reg_t CCERtemp = {0};

	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	//disable Channel 1
	CCERtemp.bit.cc1e = RESET;
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);

	CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);
	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	// select the input
	if((TIM_1 == pTIMHandle->TIMx) || (TIM_2 == pTIMHandle->TIMx) || (TIM_3 == pTIMHandle->TIMx) || (TIM_4 == pTIMHandle->TIMx)
	  || (TIM_5 == pTIMHandle->TIMx) || (TIM_6 == pTIMHandle->TIMx) || (TIM_7 == pTIMHandle->TIMx) || (TIM_8 == pTIMHandle->TIMx))
	{
		CCMR1temp.bit.cc1s = TIM_ICSelection;
	}
	else
	{
		CCMR1temp.bit.cc1s = 0x1U;
	}

	//set the filter
	CCMR1temp.bit.oc1m = RESET;
	CCMR1temp.bit.oc1ce = RESET;

	CCMR1temp.reg |= (TIM_ICFilter << 4U);

	//select the polarity and set the CC1E bit
	CCERtemp.bit.cc1p = TIM_ICPolarity;
	CCERtemp.bit.cc1np = TIM_ICPolarity;
	CCERtemp.bit.cc1e = SET;

	// write to CCMR1 and CCER registers
	tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);
}


static void tim_ti2_set_config(TIM_Handle_t *pTIMHandle, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t TIM_ICFilter)
{
	TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
	TIM_1_8_CCER_Reg_t CCERtemp = {0};

	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	//disable Channel 2
	CCERtemp.bit.cc2e = RESET;
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);

	CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);
	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	// select the input
	CCMR1temp.bit.cc2s = TIM_ICSelection;

	//set the filter
	CCMR1temp.bit.oc2m = RESET;
	CCMR1temp.bit.oc2ce = RESET;

	CCMR1temp.reg |= (TIM_ICFilter << 12U);

	//select the polarity and set the CC2E bit
	CCERtemp.bit.cc2p = TIM_ICPolarity;
	CCERtemp.bit.cc2np = TIM_ICPolarity;
	CCERtemp.bit.cc2e = SET;

	// write to CCMR1 and CCER registers
	tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);
}


static void tim_ti3_set_config(TIM_Handle_t *pTIMHandle, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t TIM_ICFilter)
{
	TIM_1_8_CCMR2_Reg_t CCMR2temp = {0};
	TIM_1_8_CCER_Reg_t CCERtemp = {0};

	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	//disable Channel 3
	CCERtemp.bit.cc3e = RESET;
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);

	CCMR2temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR2);
	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	// select the input
	CCMR2temp.bit.cc3s = TIM_ICSelection;

	//set the filter
	CCMR2temp.bit.oc3m = RESET;
	CCMR2temp.bit.oc3ce = RESET;

	CCMR2temp.reg |= (TIM_ICFilter << 4U);

	//select the polarity and set the CC3E bit
	CCERtemp.bit.cc3p = TIM_ICPolarity;
	CCERtemp.bit.cc3np = TIM_ICPolarity;
	CCERtemp.bit.cc3e = SET;

	// write to CCMR2 and CCER registers
	tim_write_to_ccmrx(pTIMHandle, TIM_CCMR2, CCMR2temp.reg);
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);
}


static void tim_ti4_set_config(TIM_Handle_t *pTIMHandle, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t TIM_ICFilter)
{
	TIM_1_8_CCMR2_Reg_t CCMR2temp = {0};
	TIM_1_8_CCER_Reg_t CCERtemp = {0};

	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	//disable Channel 4
	CCERtemp.bit.cc4e = RESET;
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);

	CCMR2temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR2);
	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	// select the input
	CCMR2temp.bit.cc4s = TIM_ICSelection;

	//set the filter
	CCMR2temp.bit.oc4m = RESET;
	CCMR2temp.bit.oc4ce = RESET;

	CCMR2temp.reg |= (TIM_ICFilter << 12U);

	//select the polarity and set the CC3E bit
	CCERtemp.bit.cc4p = TIM_ICPolarity;
	CCERtemp.bit.cc4np = TIM_ICPolarity;
	CCERtemp.bit.cc4e = SET;

	// write to CCMR2 and CCER registers
	tim_write_to_ccmrx(pTIMHandle, TIM_CCMR2, CCMR2temp.reg);
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);
}

static void tim_etr_set_config(TIM_Handle_t *pTIMHandle, uint32_t TIM_ExtTRGPrescaler, uint32_t TIM_ExtTRGPolarity,
		uint32_t ExtTRGFilter)
{
	TIM_1_8_SMCR_Reg_t SMCRtemp = {0};

	SMCRtemp.reg = tim_read_smcr(pTIMHandle);

	//reset ETR bits

	SMCRtemp.bit.etf = RESET;
	SMCRtemp.bit.etps = RESET;
	SMCRtemp.bit.ece = RESET;
	SMCRtemp.bit.etp = RESET;

	// set prescaler, filter value and polarity
	SMCRtemp.bit.etf = ExtTRGFilter;
	SMCRtemp.bit.etps = TIM_ExtTRGPrescaler;
	SMCRtemp.bit.etp = TIM_ExtTRGPolarity;

	// write to SMCR
	tim_write_to_smcr(pTIMHandle, SMCRtemp.reg);
}


static void tim_itrx_set_config(TIM_Handle_t *pTIMHandle, uint32_t InputTriggerSource)
{
	TIM_1_8_SMCR_Reg_t SMCRtemp = {0};

	SMCRtemp.reg = tim_read_smcr(pTIMHandle);

	// Reset the TS Bits
	SMCRtemp.bit.ts = RESET;

	// set the Input Trigger source and the slave mode
	SMCRtemp.bit.ts = InputTriggerSource;
	SMCRtemp.bit.sms = 0x7U; //  External Clock Mode 1 : 111

	// write to TIMx SMCR
	tim_write_to_smcr(pTIMHandle, SMCRtemp.reg);
}


static void tim_ti1_config_input_stage(TIM_Handle_t *pTIMHandle, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter)
{
	TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
	TIM_1_8_CCER_Reg_t CCERtemp = {0};
	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	// disable the channel 1: reset the CC2E bit
	CCERtemp.bit.cc1e = RESET;
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);

	CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);
	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	// set the filter
	CCMR1temp.bit.oc1m = RESET;
	CCMR1temp.bit.oc1ce = RESET;

	CCMR1temp.reg |= (TIM_ICFilter << 4U);

	// select the polarity and set CC1E bit
	CCERtemp.bit.cc1p = RESET;
	CCERtemp.bit.cc1np = RESET;

	CCERtemp.reg |= (TIM_ICPolarity << 4U);

	// write to CCMR1 and CCER registers
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);
	tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);
}


static void tim_ti2_config_input_stage(TIM_Handle_t *pTIMHandle, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter)
{
	TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
	TIM_1_8_CCER_Reg_t CCERtemp = {0};
	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	// disable the channel 2: reset the CC2E bit
	CCERtemp.bit.cc2e = RESET;
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);

	CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);
	CCERtemp.reg = tim_read_ccer(pTIMHandle);

	// set the filter
	CCMR1temp.bit.oc2m = RESET;
	CCMR1temp.bit.oc2ce = RESET;

	CCMR1temp.reg |= (TIM_ICFilter << 12U);

	// select the polarity and set CC2E bit
	CCERtemp.bit.cc2p = RESET;
	CCERtemp.bit.cc2np = RESET;

	CCERtemp.reg |= (TIM_ICPolarity << 4U);

	// write to CCMR1 and CCER registers
	tim_write_to_ccer(pTIMHandle, CCERtemp.reg);
	tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);
}


uint8_t tim_slave_timer_set_config(TIM_Handle_t *pTIMHandle,TIM_Slave_Config_t *sSlaveConfig)
{
	uint8_t status = 0;
	TIM_1_8_SMCR_Reg_t SMCRtemp = {0};
	TIM_1_8_CCMR1_Reg_t CCMR1temp = {0};
	TIM_1_8_CCER_Reg_t	CCERtemp = {0};

	// get SMCR value
	SMCRtemp.reg = tim_read_smcr(pTIMHandle);

	// set input trigger source
	SMCRtemp.bit.ts = sSlaveConfig->InputTrigger;

	// set slave mode
	SMCRtemp.bit.sms = sSlaveConfig->SlaveMode;

	// write to SMCR
	tim_write_to_smcr(pTIMHandle, SMCRtemp.reg);

	// configure the trigger prescaler, filter, and polarity
	switch (sSlaveConfig->InputTrigger)
	{
		case TIM_CLOCKSOURCE_ETRMODE1:
		{
			// configure the ETR Trigger source
			tim_etr_set_config(pTIMHandle,
                    sSlaveConfig->TriggerPrescaler,
                    sSlaveConfig->TriggerPolarity,
                    sSlaveConfig->TriggerFilter);
			break;
		}
		case TIM_CLOCKSOURCE_TI1ED:
		{
			if (sSlaveConfig->SlaveMode == TIM_SLAVEMODE_GATED)
			{
				return 0x1U;
			}

			// disable channel 1: reset CC1E bit
			CCERtemp.reg = tim_read_ccer(pTIMHandle);
			CCERtemp.bit.cc1e = RESET;
			tim_write_to_ccer(pTIMHandle, CCERtemp.reg);

			// set the filter
			CCMR1temp.reg = tim_read_ccmrx(pTIMHandle, TIM_CCMR1);
			CCMR1temp.bit.oc1m = RESET;
			CCMR1temp.bit.oc1ce = RESET;
			CCMR1temp.reg |= ((sSlaveConfig->TriggerFilter) << 4U);
			tim_write_to_ccmrx(pTIMHandle, TIM_CCMR1, CCMR1temp.reg);

			break;
		}
		case TIM_CLOCKSOURCE_TI2:
		{
			// configure TI2 filter and polarity
			tim_ti2_config_input_stage(pTIMHandle,
                    sSlaveConfig->TriggerPolarity,
                    sSlaveConfig->TriggerFilter);
			break;
		}
		case TIM_CLOCKSOURCE_ITR0:
		case TIM_CLOCKSOURCE_ITR1:
		case TIM_CLOCKSOURCE_ITR2:
		case TIM_CLOCKSOURCE_ITR3:
		{
			break;
		}
		default:
			status = 0x1U;
			break;
	}
	return status;
}


/****************************************************** End of file *************************************************************/

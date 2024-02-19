/*												@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
 @file     stm32f407xx_usart.h					@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @brief    STM32F407xx Device Peripheral 		@@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
  		   USART HAL driver.					@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
 @author   destrocore							@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
 @version  V1.0									@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
												@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
This file provides firmware functions to manage @@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
the following functionalities of the Inter- 	@@@@@@@@@@@@@@@@@             @@@         %   @@
Integrated Circuit peripheral:					@@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
+ Initialization and de-initialization function @@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@
+ Peripheral Control functions					@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@
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

#ifndef __STM32F407XX_I2C_H_
#define __STM32F407XX_I2C_H_


#include "stm32f407xx.h"

/*
 * Configuration structure for I2Cx peripheral
 */
typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint8_t  I2C_DeviceAddress;
	uint8_t  I2C_AckControl;
	uint8_t  I2C_FMDutyCycle;
}I2C_Config_t;

/*
 * Handle structure for I2Cx peripheral
 */
typedef struct
{
	I2C_RegDef_t 	*pI2Cx;
	I2C_Config_t 	I2C_Config;
	uint8_t 		*pTxBuffer; /* !< To store the app. Tx buffer address > */
	uint8_t 		*pRxBuffer;	/* !< To store the app. Rx buffer address > */
	uint32_t 		TxLen;		/* !< To store Tx len > */
	uint32_t 		RxLen;		/* !< To store Tx len > */
	uint8_t 		TxRxState;	/* !< To store Communication state > */
	uint8_t 		DevAddr;	/* !< To store slave/device address > */
	uint32_t        RxSize;		/* !< To store Rx size  > */
	uint8_t         Sr;			/* !< To store repeated start value  > */
}I2C_Handle_t;


/*
 * I2C application states
 */
typedef enum
{
	I2C_READY,
	I2C_BUSY_IN_RX,
	I2C_BUSY_IN_TX
}I2C_AppState_t;

/*
 * @I2C_SCLSpeed
 */
typedef enum
{
	I2C_SCL_SPEED_STANDARD = 100000,
	I2C_SCL_SPEED_FAST4K   = 400000,
	I2C_SCL_SPEED_FAST2K   = 200000
}I2C_SCL_Speed_t;

/*
 * @I2C_ACK_Control
 */
typedef enum
{
	I2C_ACK_DISABLE,
	I2C_ACK_ENABLE
}ACK_Status_t;


/*
 * @I2C_FastModeDutyCycle
 */
typedef enum
{
	I2C_FM_DUTY_2,
	I2C_FM_DUTY_16_9
}I2C_FmDutyCycle_t;


/*
 * I2C related status flags definitions
 */
typedef enum
{
    I2C_SR1_SB = 0x0U,   			/*!< Parity error 									*/
	I2C_SR1_ADDR = 0x1U,   			/*!< Framing error 									*/
	I2C_SR1_BTF = 0x2U,   			/*!< Noise detected flag 							*/
	I2C_SR1_ADD10 = 0x3U,  			/*!< Overrun error 									*/
	I2C_SR1_STOPF = 0x4U, 			/*!< IDLE line detected 							*/
	I2C_SR1_RxNE = 0x5U, 			/*!< Read data register not empty 					*/
	I2C_SR1_TxE = 0x6U,   			/*!< Transmission complete 							*/
	I2C_SR1_BERR = 0x7U,  			/*!< Transmit data register empty 					*/
	I2C_SR1_ARLO = 0x8U,  			/*!< LIN break detection flag 						*/
	I2C_SR1_AF = 0x9U,  			/*!< CTS flag 										*/
	I2C_SR1_OVR = 0x10U,  			/*!< CTS flag 										*/
	I2C_SR1_PECERR = 0x11U,  		/*!< CTS flag 										*/
	I2C_SR1_TIMEOUT = 0x12U,  		/*!< CTS flag 										*/
	I2C_SR1_SMBALERT = 0x13U,  		/*!< CTS flag 										*/
}I2C_SR1_flag_t;


/*
 * I2C application events
 */
typedef enum{
 I2C_EV_TX_CMPLT,
 I2C_EV_RX_CMPLT,
 I2C_EV_STOP,
 I2C_ERROR_BERR,
 I2C_ERROR_ARLO,
 I2C_ERROR_AF,
 I2C_ERROR_OVR,
 I2C_ERROR_TIMEOUT,
 I2C_EV_DATA_REQ,
 I2C_EV_DATA_RCV
}I2C_AppEvent_t;

/********************************************************************************************/
/*								APIs supported by this driver								*/
/*		 For more information about the APIs check the function definitions					*/
/********************************************************************************************/
/*
 * Peripheral Clock setup
 */
void I2C_PeriphClockControl(I2C_RegDef_t *pI2Cx, uint8_t state);

/*
 * Initialization and De-initialization
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);


/*
 * Data Send and Receive
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);


void I2C_SlaveSendData(I2C_RegDef_t *pI2C,uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C);

/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQConfig(uint8_t IRQNumber, uint32_t IRQPriority, uint8_t state);
void I2C_Event_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_Error_IRQHandling(I2C_Handle_t *pI2CHandle);


/*
 * Other Peripheral Control APIs
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t state);
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t state);
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2Cx,uint8_t state);

/*
 * Application callback
 */
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv);



#endif /* __STM32F407XX_I2C_H_ */

/*												@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
 @file     stm32f407xx_usart.h					@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @brief    STM32F407xx Device Peripheral 		@@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
  		   SPI HAL driver.						@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
 @author   destrocore							@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
 @version  V1.0									@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
												@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
This file provides firmware functions to manage @@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
the following functionalities of the Serial	 	@@@@@@@@@@@@@@@@@             @@@         %   @@
Peripheral Interface:							@@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
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
#ifndef __STM32F407XX_SPI_H_
#define __STM32F407XX_SPI_H_

#include <stm32f407xx.h>

typedef struct
{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
}SPI_Config_t;

/*
 * Handle structure for SPIx peripheral
 */
typedef struct
{
	SPI_RegDef_t *pSPIx;		/*!< This holds the base addresses of SPIx(x:0,1,2) peripheral >*/
	SPI_Config_t SPIConfig;
	uint8_t *TxBuffer;			/*!< To store the application Tx buffer address >*/
	uint8_t *RxBuffer;			/*!< To store the application Rx buffer address >*/
	uint32_t TxLen;				/*!< To store Tx length >*/
	uint32_t RxLen;				/*!< To store Rx length > */
	uint8_t TxState;			/*!< To store Tx state >*/
	uint8_t RxState;			/*!< To store Tx state >*/
}SPI_Handle_t;

/*
 * @SPI_DeviceMode
 */
typedef enum{
	SPI_MODE_SLAVE = 0,
	SPI_MODE_MASTER
}SPI_Mode_t;

/*
 * @SPI_BusConfig
 */
typedef enum
{
	SPI_BUS_FD = 1,
	SPI_BUS_HD,
	SPI_BUS_SIMPLEX_RXONLY
}SPI_BusConfig_t;

/*
 * @SPI_SclkSpeed
 */
typedef enum
{
	SPI_SCLK_SPEED_DIV2 = 0,
	SPI_SCLK_SPEED_DIV4,
	SPI_SCLK_SPEED_DIV8,
	SPI_SCLK_SPEED_DIV16,
	SPI_SCLK_SPEED_DIV32,
	SPI_SCLK_SPEED_DIV64,
	SPI_SCLK_SPEED_DIV128,
	SPI_SCLK_SPEED_DIV256
}SPI_SclkCSpeed_t;

/*
 * @SPI_DFF
 */
typedef enum
{
	SPI_DFF_8BITS = 0,
	SPI_DFF_16BITS

}SPI_DFF_t;

/*
 * @SPI_CPOL
 */
typedef enum
{
	SPI_CPOL_LOW = 0,
	SPI_CPOL_HIGH
}SPI_CPOL_t;

/*
 * @SPI_CPHA
 */
typedef enum
{
	SPI_CPHA_LOW = 0,
	SPI_CPHA_HIGH
}SPI_CPHA_t;

/*
 * @SPI_SSM
 */
typedef enum
{
	SPI_SSM_HW = 0,
	SPI_SSM_SW
}SPI_SSM_t;

/*
 * Possible SPI application events
 */
typedef enum
{
	SPI_EVENT_TX_CMPLT = 1,
	SPI_EVENT_RX_CMPLT,
	SPI_EVENT_OVR_ERR,
	SPI_EVENT_CRC_ERR
}SPI_AppEvent_t;

typedef enum
{
	SPI_READY = 0,
	SPI_BUSY_IN_RX,
	SPI_BUSY_IN_TX
}SPI_IT_flag_t;

/*
 * @SPI flags
 */
typedef enum
{
    SPI_SR_RXNE = 0x0U,   			/*!< Receive buffer not empty						*/
	SPI_SR_TXE = 0x1U,   			/*!< Transmit buffer empty							*/
	SPI_SR_CHSIDE = 0x2U,   		/*!< Channel side 									*/
	SPI_SR_UDR = 0x3U,  			/*!< Underrun flag 								 	*/
	SPI_SR_CRCERR = 0x4U, 			/*!< CRC error flag 								*/
	SPI_SR_MODF = 0x5U, 			/*!< Mode fault 									*/
	SPI_SR_OVR = 0x6U,   			/*!< Overrun flag 									*/
	SPI_SR_BSY = 0x7U,  			/*!< Busy flag 										*/
	SPI_SR_FRE = 0x8U,  			/*!< Frame format error 							*/
}SPI_SR_flag_t;

/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void SPI_PeriphClockControl(SPI_RegDef_t *pSPIx, uint8_t state);

/*
 * Initialization and de-initialization
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*
 * Data Send and Receive
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTXBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRXBuffer, uint32_t Len);

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTXBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRXBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQConfig(uint8_t IRQNumber, uint32_t IRQPriority, uint8_t state);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

/*
 * Other Peripheral Control APIs
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t state);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t state);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t state);
uint8_t SPI_GetFLagStatus(SPI_RegDef_t *pSPIx, SPI_SR_flag_t StatusFlagName);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTx(SPI_Handle_t *pSPIHandle);
void SPI_CloseRx(SPI_Handle_t *pSPIHandle);

/*
 * Application callback
 */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, SPI_AppEvent_t event);

#endif /* __STM32F407XX_SPI_H_ */

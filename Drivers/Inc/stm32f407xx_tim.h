/*												@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
 @file     stm32f407xx_tim.h					@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @brief    STM32F407xx Device Peripheral 		@@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
  		   TIM HAL module driver.				@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
 @author   destrocore							@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
 @version  V1.0									@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
												@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
This file provides firmware functions to manage @@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
the following functionalities of the timer  	@@@@@@@@@@@@@@@@@             @@@         %   @@
(TIM) peripheral:								@@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
+ Initialization and de-initialization function @@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@
+ Peripheral Control functions					@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@
+ Basic timer control functions 			 	@ @@@@@@@@@@@@@@                  @@@@@@@@@@@@@@
												@@  @@@@@@@@@                  @@@@@@@@@@@@@@@@@
- IC and OC modes need updates					@@@@  @@@    @@@@@@@&         .@@@@@@@@@@@@@@@@@
- callback functions also don't work			@@@@@@@#   ###@@@@( @        &@@@@@@@@@@@@@@@@@@
												@@@@@@@@@@@@@@@#     @@     (@     @@@@@@@@@@@@@
												@@@@@@@@@@@@@@     @@@@     @@     @@@@@@@@@@@@@
												@@@@@@@@@@@&     @@@@@@/   @@@@@@    @@@@@@@@@@@
												@@@@@@@@@@@*    @@@@@@@@  @@@@@@@@      @@@@@@@@
												@@@@@@@@@@@      @@@@@@@  @@@@@@@@   %  @@@@@@@@
												@@@@@@@@@@@@       /&@@@  @@@@@@&   @ @@@@@@@@@@
												@@@@@@@@@@@@@@&  ,@@@@@@@@@@@@  @ @@@@@@@@@@@@@@
												@@@@@@@@@@@@@@@@@@  @@@@@@@@@@@%@@@@@@@@@@@@@@@@													*/

#ifndef __STM32F407XX_TIM_H_
#define __STM32F407XX_TIM_H_

#include <stm32f407xx.h>


/*
 * Timer Configuration Structure
 */
typedef struct
{
    uint32_t Prescaler;				/*!< Specifies the prescaler value used to divide the TIM clock. 					>*/
    uint32_t Period;				/*!< Specifies the period value to be loaded into the active ARR at the next update
    																											  event >*/
    uint32_t CounterMode;			/*!< @TIM_Counter_Mode - Specifies the counter mode 								>*/
    uint32_t CenterAlignedMode;		/*!< @TIM_CMS - Specifies the center aligned mode 									>*/
    uint32_t ClockDivision;			/*!< @TIM_ClockDivision - Specifies the clock division 								>*/
    uint32_t RepetitionCounter;		/*!< Specifies the repetition counter value. Each time the RCR downcounter reaches
    									 zero, an update event is generated and counting restarts from the RCR value N. >*/
    uint32_t AutoReloadPreload;		/*!< @TIM_AutoReloadPreload - Specifies the auto-reload preload 					>*/
}TIM_Base_Config_t;


/*
 * Timer Output Compare Configuration Structure
 */
typedef struct
{
    uint32_t OCMode;
    uint32_t Pulse;
    uint32_t OCPolarity;
    uint32_t OCNPolarity;
    uint32_t OCFastMode;
    uint32_t OCIdleState;
    uint32_t OCNIdleState;
}TIM_OC_Config_t;

/*
 * Timer One Pulse Configuration Structure
 */
typedef struct
{
	uint32_t OCMode;
    uint32_t Pulse;
    uint32_t OCPolarity;
    uint32_t OCNPolarity;
    uint32_t OCIdleState;
    uint32_t OCNIdleState;
    uint32_t ICPolarity;
    uint32_t ICSelection;
    uint32_t ICFilter;
}TIM_OnePulse_Config_t;

/*
 * Timer Input Capture Configuration Structure
 */
typedef struct
{
    uint32_t ICPolarity;
    uint32_t ICSelection;
    uint32_t ICPrescaler;
    uint32_t ICFilter;
}TIM_IC_Config_t;

/*
 * Timer Clock Configuration Structure
 */
typedef struct
{
    uint32_t ClockSource;
    uint32_t ClockPolarity;
    uint32_t ClockPrescaler;
    uint32_t ClockFilter;
}TIM_Clock_Config_t;

/*
 * Timer Clear Input Configuration Structure
 */
typedef struct
{
	uint32_t ClearInputState;
	uint32_t ClearInputSource;
    uint32_t ClearInputPolarity;
    uint32_t ClearInputPrescaler;
    uint32_t ClearInputFilter;
}TIM_Clear_Input_Config_t;

/*
 * Timer Master Configuration Structure
 */
typedef struct
{
    uint32_t MasterOutputTrigger;
    uint32_t MasterSlaveMode;
}TIM_Master_Config_t;

/*
 * Timer Slave Configuration Structure
 */
typedef struct
{
	uint32_t  SlaveMode;         /*!< Slave mode selection. This parameter can be a value of @TIM_Slave_Mode 		*/
	uint32_t  InputTrigger;      /*!< Input Trigger source. This parameter can be a value of @TIM_Clock_Source_t 	*/
	uint32_t  TriggerPolarity;   /*!< Input Trigger polarity. This parameter can be a value of @TIM_Trigger_Polarity */
	uint32_t  TriggerPrescaler;  /*!< Input trigger prescaler. This parameter can be a value of @TIM_Trigger_Prescaler */
	uint32_t  TriggerFilter;     /*!< Input trigger filter. This parameter can be a number between Min_Data = 0x0 and Max_Data = 0xF  */
}TIM_Slave_Config_t;

/*
 * @TIM_Number definitions
 */
typedef enum
{
	TIM_1 = 0x1U,
	TIM_2 = 0x2U,
	TIM_3 = 0x3U,
	TIM_4 = 0x4U,
	TIM_5 = 0x5U,
	TIM_6 = 0x6U,
	TIM_7 = 0x7U,
	TIM_8 = 0x8U,
	TIM_9 = 0x9U,
	TIM_10 = 0x10U,
	TIM_11 = 0x11U,
	TIM_12 = 0x12U,
	TIM_13 = 0x13U,
	TIM_14 = 0x14U
}TIM_Number_t;

/*
 * @TIM_State definitions
 */
typedef enum
{
	TIM_STATE_RESET             = 0x0U,    	/*!< Peripheral not yet initialized or disabled  							>*/
	TIM_STATE_READY             = 0x1U,    	/*!< Peripheral Initialized and ready for use    							>*/
	TIM_STATE_BUSY              = 0x2U,    	/*!< An internal process is ongoing              							>*/
	TIM_STATE_TIMEOUT           = 0x3U,    	/*!< Timeout state                               							>*/
	TIM_STATE_ERROR             = 0x4U     	/*!< Reception process is ongoing                							>*/
}TIM_State_t;

/*
 * @TIM_CH_State definitions
 */
typedef enum
{
  TIM_CH_STATE_RESET             = 0x0U,    	/*!< TIM Channel initial state                         					>*/
  TIM_CH_STATE_READY             = 0x1U,    	/*!< TIM Channel ready for use                         					>*/
  TIM_CH_STATE_BUSY              = 0x2U,    	/*!< An internal process is ongoing on the TIM channel 					>*/
}TIM_CH_State_t;

/*
 * @DMA_Burst_State definitions
 */
typedef enum
{
	DMA_BURST_STATE_RESET             = 0x0U,    /*!< DMA Burst initial state 											>*/
	DMA_BURST_STATE_READY             = 0x1U,    /*!< DMA Burst ready for use 											>*/
	DMA_BURST_STATE_BUSY              = 0x2U,    /*!< Ongoing DMA Burst       											>*/
}TIM_DMA_Burst_State_t;


/*
 * @TIM_ActiveChannel definitions
 */
typedef enum
{
	TIM_ACTIVE_CHANNEL_1        = 0x1U,    	/*!< The active channel is 1     											>*/
	TIM_ACTIVE_CHANNEL_2        = 0x2U,    	/*!< The active channel is 2     											>*/
	TIM_ACTIVE_CHANNEL_3        = 0x4U,    	/*!< The active channel is 3     											>*/
	TIM_ACTIVE_CHANNEL_4        = 0x8U,    	/*!< The active channel is 4     											>*/
	TIM_ACTIVE_CHANNEL_CLEARED  = 0x0U     	/*!< All active channels cleared 											>*/
}TIM_ActiveChannel_t;

/*
 * @TIM_Channel definitions
 */
typedef enum
{
	TIM_CHANNEL_1 = 0x0U,
	TIM_CHANNEL_2 = 0x4U,
	TIM_CHANNEL_3 = 0x8U,
	TIM_CHANNEL_4 = 0xCU,
	TIM_CHANNEL_ALL = 0x3CU
}TIM_Channel_t;

/*
 * @TIM_Counter_Mode
 */
typedef enum
{
	TIM_COUNTERMODE_UP              = 0x0U,   		 /*!< Counter used as up-counter   									>*/
	TIM_COUNTERMODE_DOWN            = 0x1U,   		 /*!< Counter used as down-counter 									>*/
}TIM_Counter_Mode_t;

/*
 * @TIM flags
 */
typedef enum
{
	TIM_SR_UPDATE 	= 0x0U,   						/*!< Update interrupt  												>*/
	TIM_SR_CC1 		= 0x1U,   						/*!< Capture/Compare 1 interrupt  									>*/
	TIM_SR_CC2 		= 0x2U,							/*!< Capture/Compare 2 interrupt  									>*/
	TIM_SR_CC3	 	= 0x3U,   						/*!< Capture/Compare 3 interrupt  									>*/
	TIM_SR_CC4	 	= 0x4U,   						/*!< Capture/Compare 4 interrupt  									>*/
	TIM_SR_COM 		= 0x5U,   						/*!< Commutation interrupt  										>*/
	TIM_SR_TRIGGER 	= 0x6U,   						/*!< Trigger interrupt  											>*/
	TIM_SR_BREAK 	= 0x7U,   						/*!< Break interrupt  												>*/
	TIM_SR_CC1OF 	= 0x8U,   						/*!< Capture/Compare 1 overcapture  								>*/
	TIM_SR_CC2OF 	= 0x9U,   						/*!< Capture/Compare 2 overcapture 									>*/
	TIM_SR_CC3OF 	= 0x10U,   						/*!< Capture/Compare 3 overcapture  								>*/
	TIM_SR_CC4OF 	= 0x11U   						/*!< Capture/Compare 4 overcapture  								>*/
}TIM_SR_flag_t;

/*
 * @TIM_CMS
 */
typedef enum
{
	TIM_CMS_EDGEALIGNED	    = 0x0U,					/*!< Edge-aligned mode 	        									>*/
	TIM_CMS_CENTERALIGNED1  = 0x1U, 				/*!< Center-aligned mode 1        									>*/
	TIM_CMS_CENTERALIGNED2  = 0x2U, 				/*!< Center-aligned mode 2        									>*/
	TIM_CMS_CENTERALIGNED3  = 0x3U					/*!< Center-aligned mode 3        									>*/
}TIM_CMS_t;

/*
 * @TIM_CCMR
 */
typedef enum
{
	TIM_CCMR1 = 0x0U,
	TIM_CCMR2 = 0x1U
}TIM_CCMR_t;

/*
 * @TIM_CCR
 */
typedef enum
{
	TIM_CCR1 = 0x0U,
	TIM_CCR2 = 0x1U,
	TIM_CCR3 = 0x2U,
	TIM_CCR4 = 0x3U
}TIM_CCR_t;

/*
 * @TIM_IT
 */
typedef enum
{
	TIM_IT_UPDATE 		= 0x0U,
	TIM_IT_CC1 			= 0x1U,
	TIM_IT_CC2			= 0x2U,
	TIM_IT_CC3			= 0x3U,
	TIM_IT_CC4			= 0x4U,
	TIM_IT_COM			= 0x5U,
	TIM_IT_TRIGGER		= 0x6U,
	TIM_IT_BREAK		= 0x7U,
	TIM_IT_DMA_UPDATE	= 0x8U,
	TIM_IT_DMA_CC1		= 0x9U,
	TIM_IT_DMA_CC2		= 0x10U,
	TIM_IT_DMA_CC3		= 0x11U,
	TIM_IT_DMA_CC4		= 0x12U,
	TIM_IT_DMA_COM		= 0x13U,
	TIM_IT_DMA_TRIGGER	= 0x14U
}TIM_IT_t;

/*
 * @TIM_One_Pulse_Mode
 */
typedef enum
{
	TIM_OPMODE_REPETITIVE = 0x0U,
	TIM_OPMODE_SINGLE = 0x1U
}TIM_One_Pulse_Mode_t;

/*
 * @TIM_Input_Capture_Polarity
 */
typedef enum
{
	TIM_ICPOLARITY_RISING = 0x0U,
	TIM_ICPOLARITY_FALLING = 0x1U,
	TIM_ICPOLARITY_BOTHEDGE = 0x3U
}TIM_Input_Capture_Polarity_t;


/*
 * @TIM_Input_Capture_Prescaler
 */
typedef enum
{
	TIM_ICPSC_DIV1 = 0x0U,
	TIM_ICPSC_DIV2 = 0x1U,
	TIM_ICPSC_DIV4 = 0x2U,
	TIM_ICPSC_DIV8 = 0x3U
}TIM_Input_Capture_Prescaler_t;


/*
 * @TIM_Input_Capture_Selection
 */
typedef enum
{
	TIM_ICSELECTION_DIRECTTI 	= 0x1U,
	TIM_ICSELECTION_INDIRECTTI 	= 0x2U,
	TIM_ICSELECTION_TRC			= 0x3U
}TIM_Input_Capture_Selection_t;


/*
 *	@TIM_Clear_Input_Source
 */
typedef enum
{
	TIM_CLEARINPUTSOURCE_NONE = 0x0U,
	TIM_CLEARINPUTSOURCE_ETR  = 0x1U
}TIM_Clear_Input_Source_t;

/*
 * @TIM_TI1_Selection
 */
typedef enum
{
	TIM_TI1SELECTION_CH1 = 0x0U,
	TIM_TI1SELECTION_XORCOMBINATION = 0x1U
}TIM_TI1_Selection_t;

/*
 * @TIM_Slave_Mode
 */
typedef enum
{
	TIM_SLAVEMODE_DISABLE 	= 0x0U,
	TIM_SLAVEMODE_RESET 	= 0x4U,
	TIM_SLAVEMODE_GATED		= 0x5U,
	TIM_SLAVEMODE_TRIGGER  	= 0x6U,
	TIM_SLAVEMODE_EXTERNAL1	= 0x7U
}TIM_Slave_Mode_t;

/*
 * @TIM_MasterOutputTrigger
 */
typedef enum
{
	TIM_TRGO_RESET 	= 0x0U,
	TIM_TRGO_ENABLE = 0x1U,
	TIM_TRGO_UPDATE = 0x2U,
	TIM_TRGO_OC1	= 0x3U,
	TIM_TRGO_OC1REF = 0x4U,
	TIM_TRGO_OC2REF = 0x5U,
	TIM_TRGO_OC3REF = 0x6U,
	TIM_TRGO_OC4REF = 0x7U
}TIM_MasterOutputTrigger_t;


/*
 * @TIM_ClockDivision
 */
typedef enum
{
	TIM_CLOCKDIV_DIV1 = 0x0U,
	TIM_CLOCKDIV_DIV2 = 0x1U,
	TIM_CLOCKDIV_DIV4 = 0x2U
}TIM_ClockDivision_t;


/*
 * @TIM_AutoReloadPreload
 */
typedef enum
{
	TIM_AUTORELOAD_DISABLE = 0x0U,
	TIM_AUTORELOAD_ENABLE = 0x1U
}TIM_ARP_t;

/*
 * @TIM_Clock_Source
 */
typedef enum
{
	TIM_CLOCKSOURCE_INTERNAL = 0x1U,
	TIM_CLOCKSOURCE_ETRMODE1 = 0x7U,
	TIM_CLOCKSOURCE_ETRMODE2 = 0x2U,
	TIM_CLOCKSOURCE_TI1ED  	 = 0x4U,
	TIM_CLOCKSOURCE_TI1		 = 0x5U,
	TIM_CLOCKSOURCE_TI2      = 0x6U,
}TIM_Clock_Source_t;

/*
 * @TIM_Event_Source
 */
typedef enum
{
	TIM_EVENTSOURCE_UPDATE 	= 0x0U,
	TIM_EVENTSOURCE_CC1		= 0x1U,
	TIM_EVENTSOURCE_CC2		= 0x2U,
	TIM_EVENTSOURCE_CC3		= 0x3U,
	TIM_EVENTSOURCE_CC4		= 0x4U,
	TIM_EVENTSOURCE_COM		= 0x5U,
	TIM_EVENTSOURCE_TRIGGER	= 0x6U,
	TIM_EVENTSOURCE_BREAK	= 0x7U
}TIM_Event_Source_t;

#define TIM_CLOCKSOURCE_ITR0	 0x8U
#define TIM_CLOCKSOURCE_ITR1	 0x9U
#define TIM_CLOCKSOURCE_ITR2	 0x10U
#define TIM_CLOCKSOURCE_ITR3	 0x11U

/*
 * Timer Handle Structure
 */
typedef struct __TIM_Handle_t
{
	 uint8_t TIMx;						 	/*!< @TIM_Number - Number of timer used for the application by user 		>*/
    TIM_Base_Config_t 	TIM_Config;     		/*!< This holds Timer configuration settings 			 					>*/
    		   uint8_t 	Channel;			/*!< @TIM_Channel - Active Channel						 			 		>*/
     volatile uint8_t 	State;             	/*!< @TIM_State - TIM operation state                       				>*/
     volatile uint8_t 	ChannelState[4];   	/*!< @TIM_CH_State - TIM channel operation state            				>*/
     volatile uint8_t 	ChannelNState[4];  	/*!< @TIM_CH_State - TIM complementary channel operation state 				>*/
     volatile uint8_t	DMABurstState; 		/*!< @DMA_Burst_State - TIM DMA operation state 							>*/
}TIM_Handle_t;

/********************************************************************************************/
/*								APIs supported by this driver								*/
/*		 For more information about the APIs check the function definitions					*/
/********************************************************************************************/
/*
 * TIM Peripheral Control
 */
void TIM_PeriphControl(TIM_Handle_t *pTIMHandle, uint8_t state);
/*
 * Time base functions
 */
void TIM_Base_Init(TIM_Handle_t *pTIMHandle);
void TIM_Base_DeInit(TIM_Handle_t *pTIMHandle);
/* Start and Stop blocking APIs*/
void TIM_Base_Start(TIM_Handle_t *pTIMHandle);
void TIM_Base_Stop(TIM_Handle_t *pTIMHandle);
/*Start and Stop interrupt mode*/
void TIM_Base_StartIT(TIM_Handle_t *pTIMHandle);
void TIM_Base_StopIT(TIM_Handle_t *pTIMHandle);
/*Start and Stop DMA mode*/
//TODO: add DMA timer functions
//void TIM_Base_StartDMA(TIM_Handle_t *pTIMHandle, uint32_t *pData, uint16_t Length);
//void TIM_Base_StopDMA(TIM_Handle_t *pTIMHandle);

/*
 * Timer Output Compare functions
 */
void TIM_OC_Init(TIM_Handle_t *pTIMHandle);
void TIM_OC_DeInit(TIM_Handle_t *pTIMHandle);
/* Start and Stop blocking APIs*/
void TIM_OC_Start(TIM_Handle_t *pTIMHandle, uint8_t Channel);
void TIM_OC_Stop(TIM_Handle_t *pTIMHandle, uint8_t Channel);
/*Start and Stop interrupt mode*/
void TIM_OC_StartIT(TIM_Handle_t *pTIMHandle, uint8_t Channel);
void TIM_OC_StopIT(TIM_Handle_t *pTIMHandle, uint8_t Channel);
/*Start and Stop DMA mode*/
//TODO: add DMA timer functions
//void TIM_OC_StartDMA(TIM_Handle_t *pTIMHandle, uint32_t Channel, uint32_t *pData, uint16_t Length);
//void TIM_OC_StopDMA(TIM_Handle_t *pTIMHandle, uint32_t Channel);

/*
 * Timer PWM functions
 */
void TIM_PWM_Init(TIM_Handle_t *pTIMHandle);
void TIM_PWM_DeInit(TIM_Handle_t *pTIMHandle);
/* Start and Stop blocking APIs*/
void TIM_PWM_Start(TIM_Handle_t *pTIMHandle, uint8_t Channel);
void TIM_PWM_Stop(TIM_Handle_t *pTIMHandle, uint8_t Channel);
/*Start and Stop interrupt mode*/
void TIM_PWM_StartIT(TIM_Handle_t *pTIMHandle, uint8_t Channel);
void TIM_PWM_StopIT(TIM_Handle_t *pTIMHandle, uint8_t Channel);
/*Start and Stop DMA mode*/
//TODO: add DMA timer functions
//void TIM_PWM_StartDMA(TIM_Handle_t *pTIMHandle, uint8_t Channel, uint32_t *pData, uint16_t Length);
//void TIM_PWM_StopDMA(TIM_Handle_t *pTIMHandle, uint8_t Channel);

/*
 * Timer Input Capture functions
 */
void TIM_IC_Init(TIM_Handle_t *pTIMHandle);
void TIM_IC_DeInit(TIM_Handle_t *pTIMHandle);
/* Start and Stop blocking APIs*/
void TIM_IC_Start(TIM_Handle_t *pTIMHandle, uint8_t Channel);
void TIM_IC_Stop(TIM_Handle_t *pTIMHandle, uint8_t Channel);
/*Start and Stop interrupt mode*/
void TIM_IC_StartIT(TIM_Handle_t *pTIMHandle, uint8_t Channel);
void TIM_IC_StopIT(TIM_Handle_t *pTIMHandle, uint8_t Channel);
/*Start and Stop DMA mode*/
//TODO: add DMA timer functions
//void TIM_IC_StartDMA(TIM_Handle_t *pTIMHandle, uint32_t Channel, uint32_t *pData, uint16_t Length);
//void TIM_IC_StopDMA(TIM_Handle_t *pTIMHandle, uint32_t Channel);

/*
 * Timer One Pulse functions
 */
void TIM_OnePulse_Init(TIM_Handle_t *pTIMHandle, uint32_t OnePulseMode);
void TIM_OnePulse_DeInit(TIM_Handle_t *pTIMHandle);
/* Start and Stop blocking APIs*/
void TIM_OnePulse_Start(TIM_Handle_t *pTIMHandle);
void TIM_OnePulse_Stop(TIM_Handle_t *pTIMHandle);
/*Start and Stop interrupt mode*/
void TIM_OnePulse_StartIT(TIM_Handle_t *pTIMHandle);
void TIM_OnePulse_StopIT(TIM_Handle_t *pTIMHandle);

/*
 * Timer Interrupt Handler functions
 */
uint8_t TIM_GetFlagStatus(TIM_Handle_t *pTIMHandle, TIM_SR_flag_t StatusFlagName);
void TIM_IRQHandling(TIM_Handle_t *pTIMHandle);


/*
 * Control functions
 */
void TIM_OC_ConfigChannel(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *sConfig, uint8_t Channel);
void TIM_PWM_ConfigChannel(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *sConfig, uint8_t Channel);
void TIM_IC_ConfigChannel(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *sConfig, uint8_t Channel);
void TIM_OnePulse_ConfigChannel(TIM_Handle_t *pTIMHandle, TIM_OnePulse_Config_t *sConfig,
		uint8_t OutputChannel,  uint8_t InputChannel);
void TIM_ConfigOCrefClear(TIM_Handle_t *pTIMHandle, TIM_Clear_Input_Config_t *sClearInputConfig,
		uint8_t Channel);
void TIM_ConfigClockSource(TIM_Handle_t *pTIMHandle, TIM_Clock_Config_t *sClockSourceConfig);
void TIM_ConfigTI1Input(TIM_Handle_t *pTIMHandle, uint32_t TI1_Selection);
void TIM_SlaveConfigSynchro(TIM_Handle_t *pTIMHandle, TIM_Slave_Config_t *sSlaveConfig);
void TIM_SlaveConfigSynchro_IT(TIM_Handle_t *pTIMHandle, TIM_Slave_Config_t *sSlaveConfig);

//TODO: add DMA timer functions
//void TIM_DMABurst_WriteStart(TIM_Handle_t *pTIMHandle, uint32_t BurstBaseAddress,
//                                              uint32_t BurstRequestSrc, uint32_t  *BurstBuffer, uint32_t  BurstLength);
//void TIM_DMABurst_MultiWriteStart(TIM_Handle_t *pTIMHandle, uint32_t BurstBaseAddress,
//                                                   uint32_t BurstRequestSrc, uint32_t *BurstBuffer,
//                                                   uint32_t BurstLength,  uint32_t DataLength);
//void TIM_DMABurst_WriteStop(TIM_Handle_t *pTIMHandle, uint32_t BurstRequestSrc);
//void TIM_DMABurst_ReadStart(TIM_Handle_t *pTIMHandle, uint32_t BurstBaseAddress,
//                                             uint32_t BurstRequestSrc, uint32_t  *BurstBuffer, uint32_t  BurstLength);
//void TIM_DMABurst_MultiReadStart(TIM_Handle_t *pTIMHandle, uint32_t BurstBaseAddress,
//                                                  uint32_t BurstRequestSrc, uint32_t  *BurstBuffer,
//                                                  uint32_t  BurstLength, uint32_t  DataLength);
//void TIM_DMABurst_ReadStop(TIM_Handle_t *pTIMHandle, uint32_t BurstRequestSrc);
void TIM_GenerateEvent(TIM_Handle_t *pTIMHandle, uint32_t EventSource);
uint32_t TIM_ReadCapturedValue(TIM_Handle_t *pTIMHandle, uint32_t Channel);

/*
 * Callback in Interrupt and DMA modes
 */
void TIM_PeriodElapsedCallback(TIM_Handle_t *pTIMHandle);
void TIM_PeriodElapsedHalfCpltCallback(TIM_Handle_t *pTIMHandle);
void TIM_OC_DelayElapsedCallback(TIM_Handle_t *pTIMHandle);
void TIM_IC_CaptureCallback(TIM_Handle_t *pTIMHandle);
void TIM_IC_CaptureHalfCpltCallback(TIM_Handle_t *pTIMHandle);
void TIM_PWM_PulseFinishedCallback(TIM_Handle_t *pTIMHandle);
void TIM_PWM_PulseFinishedHalfCpltCallback(TIM_Handle_t *pTIMHandle);
void TIM_TriggerCallback(TIM_Handle_t *pTIMHandle);
void TIM_TriggerHalfCpltCallback(TIM_Handle_t *pTIMHandle);
void TIM_ErrorCallback(TIM_Handle_t *pTIMHandle);

/*
 * Peripheral State functions
 */
uint8_t TIM_Base_GetState(TIM_Handle_t *pTIMHandle);
uint8_t TIM_OC_GetState(TIM_Handle_t *pTIMHandle);
uint8_t TIM_PWM_GetState(TIM_Handle_t *pTIMHandle);
uint8_t TIM_IC_GetState(TIM_Handle_t *pTIMHandle);
uint8_t TIM_OnePulse_GetState(TIM_Handle_t *pTIMHandle);

/*
 * Channel State functions
 */
TIM_ActiveChannel_t TIM_GetActiveChannel(TIM_Handle_t *pTIMHandle);
uint8_t TIM_GetChannelState(TIM_Handle_t *pTIMHandle,  uint8_t Channel);
//TODO: add DMA timer functions
//uint8_t	TIM_DMABurstState(TIM_Handle_t *pTIMHandle);

//helper API
uint8_t TIM_GetITStatus(TIM_Handle_t *pTIMHandle, uint8_t Interrupt);


#endif /* __STM32F407XX_TIM_H_ */

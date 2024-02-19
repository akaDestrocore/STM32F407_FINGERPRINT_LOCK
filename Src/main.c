/*												@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
 @file     main.c								@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @brief    Main body of fingerprint sensor 	    @@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
		   module R308 based lock system		@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
 @author   destrocore							@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
 @version  V1.0									@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
												@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
This file is an example of fingerprint module   @@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
usage in combination with STM32F4 series MCU.	@@@@@@@@@@@@@@@@@             @@@         %   @@
Components used in the project are:			    @@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
+ R308 optical fingerprint module				@@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@
+ 1.8″ ST7735 TFT display						@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@
+ 4x4 keypad									@ @@@@@@@@@@@@@@                  @@@@@@@@@@@@@@
+ buzzer										@@  @@@@@@@@@                  @@@@@@@@@@@@@@@@@
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
#include <stm32f407xx.h>
#include <string.h>
#include <simple_delay.h>
#include <stdio.h>
#include <r308.h>
#include <4x4_keypad.h>
#include <password.h>
#include <logo.h>

/* Private function prototypes*/
/*
 * System initialization related functions
 */
void SystemClock_Config(void);
void TIM3_Config(void);
void TIM2_Config(void);
void TIM3_IRQHandler(void);
void TIM_PeriodElapsedCallback(TIM_Handle_t *pTIMHandle);

// Fingerprint module control related function prototypes are listed in <password.h> module


/*
 * Private structure definitions
 */
USART_CircBuff_t R308_USART_Handle;
R308_Handle_t R308_Config_Handle;
SPI_Handle_t ST7735Handle;
TIM_Handle_t TIM3Handle;
TIM_Handle_t TIM2Handle;

/*
 * Private variables
 */
char key = KEYPAD_NOT_PRESSED;
uint8_t SystemState = 0;
volatile uint32_t counter_1s = 0;


/********************************************************************************************************/
/* @function name 		- ST7735_Config															 		*/
/*																									 	*/
/* @brief				- This function configures GPIO and SPI registers needed for ST7735 based 		*/
/*						  TFT display functioning														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- Only SLCK and MOSI pins are initialized inside this function. All other pins 	*/
/*						  used by display are initialized inside 'ST7735_Init' function. If you want to */
/*						  use other pins for display you may change them inside <st7735.h> module.		*/
/********************************************************************************************************/
void ST7735_Config(void)
{

	GPIO_Handle_t ST7735_Pins;

	//SLCK and MOSI
	ST7735_Pins.pGPIOx = GPIOA;
	ST7735_Pins.GPIO_Config.PinMode = GPIO_MODE_AF;
	ST7735_Pins.GPIO_Config.PinAltFuncMode = 5;
	ST7735_Pins.GPIO_Config.PinOPType = GPIO_OUTPUT_PP;
	ST7735_Pins.GPIO_Config.PinPuPdControl = GPIO_PIN_NO_PUPD;
	ST7735_Pins.GPIO_Config.PinSpeed = GPIO_SPEED_FAST;

	ST7735_Pins.GPIO_Config.PinNumber = GPIO_PIN_5;		//SLCK pin
	GPIO_Init(&ST7735_Pins);

	ST7735_Pins.GPIO_Config.PinNumber = GPIO_PIN_7;		//MOSI pin
	GPIO_Init(&ST7735_Pins);

	ST7735Handle.pSPIx = SPI1;
	ST7735Handle.SPIConfig.SPI_DeviceMode = SPI_MODE_MASTER;
	ST7735Handle.SPIConfig.SPI_BusConfig = SPI_BUS_FD;
	ST7735Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	ST7735Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	ST7735Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	ST7735Handle.SPIConfig.SPI_SSM = SPI_SSM_SW;
	ST7735Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPI_Init(&ST7735Handle);

	ST7735_Init();
	ST7735_Backlight_On();
	ST7735_SetRotation(3);
}

/********************************************************************************************************/
/* @function name 		- R308_Config															 		*/
/*																									 	*/
/* @brief				- This function configures USART port and baudrate value needed to setup 		*/
/*						  communication with R308 module. Also sends a password verification package to */
/*						  make sure that communication between MCU and R308 is working as intended.		*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- Since ring buffer is used for USART communication between R308 and MCU, a 	*/
/*						  separate USART function  is used for R308 USART initialization.				*/
/********************************************************************************************************/
uint8_t R308_Config(void)
{
	R308_USART_Handle.USART_handle.pUSARTx = USART3;
	R308_USART_Init(&R308_USART_Handle, R308_BAUDRATE_57600);

	uint8_t ack_package;
	R308_Config_Handle.Address = 0xFFFFFFFF;
	R308_Config_Handle.Password = 0;
	ack_package = R308_Init(&R308_Config_Handle);

	return ack_package;
}


/********************************************************************************************************/
/* @function name 		- main															 				*/
/*																									 	*/
/* @brief				- The application entry point.													*/
/*																										*/
/* @return				- int																			*/
/********************************************************************************************************/
int main(void)
{
	SystemClock_Config();

	//timers init
	TIM1_Config();
	TIM2_Config();
	TIM3_Config();
	TIM_Base_StartIT(&TIM3Handle);

	//screen init
	ST7735_Config();
	destrocore();

	//PWM is used for producing buzzer sounds
	TIM_PWM_Start(&TIM2Handle, TIM_CHANNEL_1);

	//keypad init
	Keypad4x4_Init();

	//check if FP sensor is connected
	if(1 == R308_Config())
	{
		BuzzerAlert(BUZZER_INIT_SOUND);
		printf("R308 connected and ready to work!\n");
	}else
	{
		BuzzerAlert(BUZZER_ERROR_SOUND);
		ST7735_FillScreen(ST7735_BLACK);
		ST7735_DrawString(0, 35, "FINGERPRINT    SCANNER IS NOT CONNECTED", Font_11x18, ST7735_RED, ST7735_BLACK);
		while(1)
		{

		}
	}
	WelcomeScreen();
	SystemState = SYS_STATE_BEGIN;

	while(1)
	{
		if(SYS_STATE_BEGIN == SystemState)
		{
			MainMenu();
		}else if(SYS_STATE_READ_PWD == SystemState)
		{
			EnterPassword();
		}else if(SYS_STATE_PWD_CONFIRMATION == SystemState)
		{
			PasswordConfirmation();
		}else if(SYS_STATE_READ_ADMIN_PWD == SystemState)
		{
			EnterPassword();
		}else if(SYS_STATE_ADM_DB_DEL_ID == SystemState)
		{
			Delete_ID();
		}else if(SYS_STATE_ADM_SCREEN == SystemState)
		{
			AdminSettingsMenu();
		}else if(SYS_STATE_ADM_PWD_MENU == SystemState)
		{
			ChangePasswordMenu();
		}else if(SYS_STATE_ADM_DB_MENU == SystemState)
		{
			DatabaseOperationsMenu();
		}else if(SYS_STATE_ADM_DB_EMPTY == SystemState)
		{
			EmptyDBWarning();
		}else if(SYS_STATE_ADM_NEW_USR_PWD == SystemState)
		{
			EnterPassword();
		}else if(SYS_STATE_ADM_NEW_PWD == SystemState)
		{
			EnterPassword();
		}else if(SYS_STATE_ADM_ALARM_MENU == SystemState)
		{
			ChangeAlarmWarning();
		}else if(SYS_STATE_ADM_ALARM_TIME == SystemState)
		{
			SetNewAlarmTime();
		}
	}
}


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
void TIM_PeriodElapsedCallback(TIM_Handle_t *pTIMHandle)
{
	if(TIM_3 == pTIMHandle->TIMx)
	{
		counter_1s++;
		//indicate 1s has passed
	}
}


/********************************************************************************************************/
/* @function name 		- TIM3_IRQHandler																*/
/*																										*/
/* @brief				- TIM3 interrupt handling function												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM3_IRQHandler(void)
{	// if update interrupt flag is set
	if(SET == TIM3->SR.bit.uif)
	{
		TIM3->SR.bit.uif = RESET; // reset the flag
		TIM_PeriodElapsedCallback(&TIM3Handle);
	}
}


/********************************************************************************************************/
/* @function name 		- MainMenu																		*/
/*																										*/
/* @brief				- This function reads user input from keypad and proceeds to authentication		*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- If user enters any key they get promoted to scan their finger. In case user's	*/
/*						  input is a combination of '*' and 'A' they get promoted to enter administrator*/
/*						  password instead.																*/
/********************************************************************************************************/
void MainMenu(void)
{
	key = KEYPAD_NOT_PRESSED;
	key = Keypad4x4_Read();
	if(KEYPAD_NOT_PRESSED != key)
	{
		if('*'== key)
		{
			key = KEYPAD_NOT_PRESSED;
			while(KEYPAD_NOT_PRESSED == key)
			{
				key = KEYPAD_NOT_PRESSED;
				key = Keypad4x4_Read();
				if(KEYPAD_NOT_PRESSED != key)
				{
					if('A' == key)
					{
						SystemState = SYS_STATE_READ_ADMIN_PWD;
						return;
					}
					if(('A' != key) && (KEYPAD_NOT_PRESSED != key))
					{
						SearchDatabase();
						return;
					}
					else
					{
						continue;
					}

				}
			}
		}
		if(KEYPAD_NOT_PRESSED == key)
		{
			SystemState = SYS_STATE_BEGIN;
			return;
		}
		else
		{
			SearchDatabase();
			return;
		}
	}
}


/********************************************************************************************************/
/* @function name 		- GetFreeIndex																	*/
/*																										*/
/* @brief				- This function checks for new fingerprint ID in R308 FLASH memory				*/
/*																										*/
/* @return				- fingerprint ID																*/
/*																										*/
/* @Note				- Firstly, TemplateNum command packet is sent to R308. This return the current 	*/
/*						  count of fingerprints saved in R308 FLASH memory. If there is free space in 	*/
/*						  FLASH memory then the ID of then ID number will be returned for new fingerprint*/
/********************************************************************************************************/
int16_t GetFreeIndex(void)
{
	int16_t fingerID = R308_TemplateNum();
	int16_t template_count = 499 - fingerID;
	if(template_count < 0)
	{
		ST7735_FillScreen(ST7735_WHITE);
		ST7735_DrawString(20, 45, "MEMORY FULL", Font_11x18, ST7735_BLUE, ST7735_WHITE);
		delay_ms(1000);
		return -1;
	}else
	{
		return fingerID;
	}
}


/********************************************************************************************************/
/* @function name 		- EnrollFingerprint																*/
/*																										*/
/* @brief				- This function scans finger,generates new model and stores it inside the 		*/
/*						   specific slot ID in R308 FLASH memory										*/
/*																										*/
/* @parameter[in]		- ID of free slot in FLASH memory												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- Enrollment requires generation of 2 character models for each fingerprint. 	*/
/*						  If both fingerprint models are identical then fingerprint may be stored inside*/
/*						  FLASH memory and new ID will be assigned in case of successful enrollment.    */
/********************************************************************************************************/
void EnrollFingerprint(int16_t fingerID)
{
	int16_t ack_package = -1;
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawString(20, 25, "PLACE YOUR       FINGER", Font_11x18, ST7735_GREEN, ST7735_BLACK);
	delay_ms(500);
	while(R308_SUCCESS != ack_package)
	{
		ack_package = R308_GetImage();
		switch(ack_package)
		{
			case R308_SUCCESS:
			{
				printf("Finger collection success\n");
				break;
			}
			case R308_FINGER_NOT_DETECTED:
			{
				printf("Can't detect finger\n");
				break;
			}
			default:
			{
				printf("An error occurred while collecting the finger\n");
				break;
			}
		}
	}

	ack_package = R308_GenChar(1);
	switch(ack_package)
	{
		case R308_SUCCESS:
		{
			printf("Generating character file 1 complete\n");
			break;
		}
		default:
		{
			printf("Image generation failed\n");
			DatabaseOperationsMenu();
			return;
		}
	}

	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawString(20, 25, "REMOVE YOUR      FINGER", Font_11x18, ST7735_GREEN, ST7735_BLACK);
	ack_package = 0;
	while(R308_FINGER_NOT_DETECTED != ack_package)
	{
		ack_package = R308_GetImage();
		delay_ms(10);
	}

	ack_package = -1;
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawString(0, 25, "  PLACE YOUR    FINGER AGAIN", Font_11x18, ST7735_GREEN, ST7735_BLACK);
	while(R308_SUCCESS != ack_package)
	{
		ack_package = R308_GetImage();
		switch(ack_package)
		{
			case R308_SUCCESS:
			{
				printf("Finger collection success\n");
				break;
			}
			case R308_FINGER_NOT_DETECTED:
			{
				printf("Can't detect finger\n");
				break;
			}
			default:
			{
				printf("An error occurred while collecting the finger\n");
				ST7735_FillScreen(ST7735_BLACK);
				ST7735_DrawString(0, 25, "     ERROR", Font_11x18, ST7735_GREEN, ST7735_BLACK);
				delay_ms(100);
				DatabaseOperationsMenu();
				break;
			}
		}
	}

	ack_package = R308_GenChar(2);
	switch(ack_package)
	{
		case R308_SUCCESS:
		{
			printf("Generating character file 2 complete\n");
			break;
		}
		default:
		{
			printf("Image generation failed\n");
			ST7735_FillScreen(ST7735_BLACK);
			ST7735_DrawString(0, 25, "IMG GENERATION     FAILED", Font_11x18, ST7735_GREEN, ST7735_BLACK);
			delay_ms(100);
			DatabaseOperationsMenu();
			return;
		}
	}

	ack_package = R308_RegModel();
	switch(ack_package)
	{
		case R308_SUCCESS:
		{
			printf("Operation success!\n");
			break;
		}
		default:
		{
			printf("Failed registering model\n");
			ST7735_FillScreen(ST7735_BLACK);
			ST7735_DrawString(0, 25, "FAILED REGISTERING MODEL", Font_11x18, ST7735_GREEN, ST7735_BLACK);
			delay_ms(100);
			DatabaseOperationsMenu();
			break;
		}
	}

	ST7735_FillScreen(ST7735_BLACK);
	ST7735_Print(0, 25, "STORING NEW    FINGERPRINT IN THE SLOT:", fingerID, Font_11x18, ST7735_GREEN, ST7735_BLACK);
	delay_ms(500);

	ack_package = R308_Store(1, fingerID);
	switch(ack_package)
	{
		case R308_SUCCESS:
		{
			printf("Storage success\n");
			DatabaseOperationsMenu();
			break;
		}
		default:
		{
			printf("Error\n");
			ST7735_FillScreen(ST7735_BLACK);
			ST7735_DrawString(0, 25, "     ERROR", Font_11x18, ST7735_GREEN, ST7735_BLACK);
			delay_ms(100);
			DatabaseOperationsMenu();
			break;
		}
	}
	return;
}


/********************************************************************************************************/
/* @function name 		- EnrollmentAttempt																*/
/*																										*/
/* @brief				- This function checks if there is free space in R308 FLASH memory and if so	*/
/*						  calls EnrollFingerprint function												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void EnrollmentAttempt(void)
{
	int16_t fingerID = GetFreeIndex();
	if(fingerID < 0)
	{
		printf("No free slots left!\n");
		//welcome message
		return;
	}else
	{
		EnrollFingerprint(fingerID);
	}
	return;
}


/********************************************************************************************************/
/* @function name 		- SearchDatabase																*/
/*																										*/
/* @brief				- This function scans finger,generates new model and looks for the model in		*/
/*						  FLASH memory of R308															*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- if model is present in the database it proceeds to password screen. In case 	*/
/*						 finger ID belongs to administrator it proceeds to administrator password screen*/
/********************************************************************************************************/
void SearchDatabase(void)
{
	int16_t response = -1;
	ST7735_FillScreen(ST7735_WHITE);
	ST7735_DrawString(20, 25, "PLACE YOUR       FINGER", Font_11x18, ST7735_BLUE, ST7735_WHITE);
	fingerprint_icon();
	delay_ms(500);
	while(R308_SUCCESS != response)
	{
		response  = R308_GetImage();
		switch(response)
		{
			case R308_SUCCESS:
			{
				printf("Image captured successfully\n");
				break;
			}
			case R308_FINGER_NOT_DETECTED:
			{
				printf("Finger not detected\n");
				break;
			}
			default:
			{
				printf("Error\n");
				BuzzerAlert(BUZZER_ERROR_SOUND);
				ST7735_FillScreen(ST7735_BLACK);
				ST7735_Print(15, 50, "ERROR CODE:", response, Font_11x18, ST7735_RED, ST7735_BLACK);
				delay_ms(500);
				WelcomeScreen();
				return;
			}
		}
	}
	response = R308_GenChar(1);
	switch(response)
	{
		case R308_SUCCESS:
		{
			printf("Character 1 generated");
			break;
		}
		default:
		{
			printf("Error generating character");
			BuzzerAlert(BUZZER_ERROR_SOUND);
			ST7735_FillScreen(ST7735_BLACK);
			ST7735_Print(25, 50, "ERROR:", response, Font_11x18, ST7735_RED, ST7735_BLACK);
			delay_ms(500);
			WelcomeScreen();
			return;
		}
	}
	uint16_t fingerID;
	response = R308_SearchDatabase(&fingerID, 1);

	switch(response)
	{
		case R308_SUCCESS:
		{
			printf("Found finger in database!\n");
			break;
		}
		case R308_NOT_FOUND:
		{
			printf("No such finger in database");
			BuzzerAlert(BUZZER_ALARM_SOUND);
			return;
		}
		default:
		{
			printf("Error\n");
			BuzzerAlert(BUZZER_ERROR_SOUND);
			ST7735_FillScreen(ST7735_BLACK);
			ST7735_Print(25, 50, "ERROR:", response, Font_11x18, ST7735_RED, ST7735_BLACK);
			delay_ms(1000);
			WelcomeScreen();
			return;
		}
	}
	if(ADMIN_ID == fingerID)
	{
		SystemState = SYS_STATE_READ_ADMIN_PWD;
	}else
	{
		SecondStepAuth(fingerID);
	}
}


/********************************************************************************************************/
/* @function name 		- TIM2_Config															 		*/
/*																									 	*/
/* @brief				- This function configures GPIO and TIM register for TIM2 in PWM mode			*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM2_Config(void)
{
	RCC->APB1ENR.bit.tim2en = SET;

	TIM_OC_Config_t sConfigOC = {0};

	TIM2Handle.TIMx = TIM_2;
	TIM2Handle.TIM_Config.Prescaler = 1290;
	TIM2Handle.TIM_Config.CounterMode = TIM_COUNTERMODE_UP;
	TIM2Handle.TIM_Config.Period = 254;
	TIM2Handle.TIM_Config.ClockDivision = TIM_CLOCKDIV_DIV1;
	TIM2Handle.TIM_Config.AutoReloadPreload = 0;
	TIM_PWM_Init(&TIM2Handle);

	sConfigOC.OCMode = 0x6;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = 0;
	sConfigOC.OCFastMode = 0;
	TIM_PWM_ConfigChannel(&TIM2Handle, &sConfigOC, TIM_CHANNEL_1);

	GPIO_Handle_t TIM2_GPIO;

	TIM2_GPIO.pGPIOx = GPIOA;
	TIM2_GPIO.GPIO_Config.PinNumber = GPIO_PIN_15;
	TIM2_GPIO.GPIO_Config.PinMode = GPIO_MODE_AF;
	TIM2_GPIO.GPIO_Config.PinAltFuncMode = 1;
	TIM2_GPIO.GPIO_Config.PinOPType = GPIO_OUTPUT_PP;
	TIM2_GPIO.GPIO_Config.PinPuPdControl = GPIO_PIN_NO_PUPD;
	TIM2_GPIO.GPIO_Config.PinSpeed = GPIO_SPEED_LOW;
	GPIO_Init(&TIM2_GPIO);
}


/********************************************************************************************************/
/* @function name 		- TIM3_Config															 		*/
/*																									 	*/
/* @brief				- This function configures GPIO and TIM register for TIM3 in base mode			*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TIM3_Config(void)
{
	TIM_Clock_Config_t sClkSrcConfig = {0};

	RCC->APB1ENR.bit.tim3en = SET;

	TIM3Handle.TIMx = TIM_3;
	TIM3Handle.TIM_Config.Prescaler = 8399;
	TIM3Handle.TIM_Config.CounterMode = TIM_COUNTERMODE_UP;
	TIM3Handle.TIM_Config.Period = 9999;
	TIM3Handle.TIM_Config.ClockDivision = TIM_CLOCKDIV_DIV1;
	TIM3Handle.TIM_Config.AutoReloadPreload = TIM_AUTORELOAD_DISABLE;
	TIM_Base_Init(&TIM3Handle);

	sClkSrcConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	TIM_ConfigClockSource(&TIM3Handle, &sClkSrcConfig);

	NVIC_SetPriority(TIM3_IRQn, 0);
	NVIC_EnableIRQ(TIM3_IRQn);
}


/********************************************************************************************************/
/* @function name 		- SystemClock_Config															*/
/*																									 	*/
/* @brief				- This function configures System Clock for 8MHz HSE input with PLL 		 	*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void SystemClock_Config(void)
{

	RCC_OscInit_t Osc = {0};
	RCC_ClkInit_t Clk = {0};


	//osc init
	Osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	Osc.HSEState = RCC_HSE_ON;
	Osc.PLL.State = RCC_PLL_ON;
	Osc.PLL.Source = RCC_PLLCFGR_PLLSRC_HSE;
	Osc.PLL.M = 4;
	Osc.PLL.N = 168;
	Osc.PLL.P = 0;
	Osc.PLL.Q = 4;
	RCC_OscConfig(&Osc);

	//clk init
	Clk.ClockType = RCC_CLOCKTYPE_HCLK;
	Clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	Clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
	Clk.APB1CLKDivider = RCC_HCLK_DIV4;
	Clk.APB2CLKDivider = RCC_HCLK_DIV2;

	RCC_ClockConfig(&Clk);

	Clk.ClockType = RCC_CLOCKTYPE_SYSCLK;
	RCC_ClockConfig(&Clk);

	Clk.ClockType = RCC_CLOCKTYPE_PCLK1;
	RCC_ClockConfig(&Clk);

	Clk.ClockType = RCC_CLOCKTYPE_PCLK2;
	RCC_ClockConfig(&Clk);

	SysTick_Init();
}

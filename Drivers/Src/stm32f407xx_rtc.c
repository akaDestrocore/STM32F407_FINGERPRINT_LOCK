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

#include <stm32f407xx_rtc.h>

#define HSE_VALUE  8000000U


static uint8_t RTC_ConvertYear(uint16_t year);
static uint8_t RTC_ConvertMonth(uint8_t month);
static uint8_t RTC_ConvertDate(uint8_t date);



/*
 * Peripheral Clock setup
 */
/********************************************************************************************************/
/* @function name 		- RTC_ClockControl																*/
/*																										*/
/* @brief				- This function enables or disables peripheral clock for RTC					*/
/*																										*/
/* @parameter[in]		- ENABLE, DISABLE or RTC_RESET													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void RTC_ClockControl(RTC_Clock_State_t state)
{
	if (state == RTC_ENABLE)
	{
		//enable RTC clock
		RCC->BDCR.bit.rtcen = SET;
	}
	else if (state == RTC_DISABLE)
	{
		//disable RTC clock
		RCC->BDCR.bit.rtcen = RESET;
	}
	else if(state == RTC_RESET)
	{
		//backup domain software reset
		RCC->BDCR.bit.bdrst = SET;
		RCC->BDCR.bit.bdrst = RESET;
	}
}

/*
 * Initialization
 */
/********************************************************************************************************/
/* @function name 		- RTC_Init																		*/
/*																										*/
/* @brief				- This function initializes RTC													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void RTC_Init(RTC_Handle_t *pRTCHandle)
{
	//enable RTC clock
	RTC_ClockControl(RTC_ENABLE);

	//Disable RTC registers write protection
	RTC->WPR.bit.key = 0xCA;
	RTC->WPR.bit.key = 0x53;

	//Enable initialization mode
	RTC->ISR.bit.init = SET;

	while(!(SET == RTC->ISR.bit.initf))
	{
		//Wait till initialization mode is set
	}

	//Set RTC configuration
	RTC->CR.bit.fmt = pRTCHandle->RTC_Config.RTC_HourFormat;

	RTC->PRER.bit.prediv_a = pRTCHandle->RTC_Config.RTC_AsynchPrediv - 1;
	RTC->PRER.bit.prediv_s = pRTCHandle->RTC_Config.RTC_SynchPrediv - 1;

	//Set time
	RTC_SetTime(pRTCHandle);

	//Set date
	RTC_SetDate(pRTCHandle);

	//Disable initialization mode
	RTC->ISR.bit.init = RESET;

	//Enable RTC registers write protection
	RTC->WPR.reg = 0xFF;
}


/********************************************************************************************************/
/* @function name 		- RTC_SetTime																	*/
/*																										*/
/* @brief				- This function sets the time in RTC											*/
/*																										*/
/* @parameter[in]		- pointer to RTC Handle structure												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- 23 hours, 59 minutes, 59 seconds												*/
/********************************************************************************************************/
void RTC_SetTime(RTC_Handle_t *pRTCHandle)
{
	//Disable RTC registers write protection
	RTC->WPR.bit.key = 0xCA;
	RTC->WPR.bit.key = 0x53;

	//Enable initialization mode
	RTC->ISR.bit.init = SET;

	while (!(SET == RTC->ISR.bit.initf))
	{
		//Wait till initialization mode is set
	}

	RTC_TR_Reg_t TR_temp = {0};

	//Set RTC time
	TR_temp.bit.pm  = pRTCHandle->Time.AM_PM;
	TR_temp.bit.ht  = pRTCHandle->Time.hour   / 10;
	TR_temp.bit.hu  = pRTCHandle->Time.hour   % 10;
	TR_temp.bit.mnt = pRTCHandle->Time.minute / 10;
	TR_temp.bit.mnu = pRTCHandle->Time.minute % 10;
	TR_temp.bit.st  = pRTCHandle->Time.second / 10;
	TR_temp.bit.su  = pRTCHandle->Time.second % 10;
	RTC->TR.reg = TR_temp.reg;

	//Disable initialization mode
	RTC->ISR.bit.init = RESET;

	//Enable RTC registers write protection
	RTC->WPR.reg = 0xFF;
}

/********************************************************************************************************/
/* @function name 		- RTC_SetDate																	*/
/*																										*/
/* @brief				- This function sets the date in RTC											*/
/*																										*/
/* @parameter[in]		- pointer to RTC Handle structure												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void RTC_SetDate(RTC_Handle_t * pRTCHandle)
{
	RTC_DR_Reg_t DR_Temp = {0};

	//Disable RTC registers write protection
	RTC->WPR.bit.key = 0xCA;
	RTC->WPR.bit.key = 0x53;

	//Set RTC date
	DR_Temp.bit.yu  = RTC_ConvertYear(pRTCHandle->Date.year);
	DR_Temp.bit.mu  = RTC_ConvertMonth(pRTCHandle->Date.month);
	DR_Temp.bit.du  = RTC_ConvertDate(pRTCHandle->Date.date);
	DR_Temp.bit.wdu = pRTCHandle->Date.weekDay;

	RTC->ISR.bit.rsf = RESET;

	RTC->DR.reg = DR_Temp.reg;

	//Enable initialization mode
	RTC->ISR.bit.init = SET;

	while (!(SET == RTC->ISR.bit.initf))
	{
		//Wait till initialization mode is set
	}

	//Disable initialization mode
	RTC->ISR.bit.init = RESET;

	//Enable RTC registers write protection
	RTC->WPR.reg = 0xFF;
}

/********************************************************************************************************/
/* @function name 		- RTC_GetTime																	*/
/*																										*/
/* @brief				- This function fills time related variables									*/
/*																										*/
/* @parameter[in]		- pointer to RTC Handle structure												*/
/*																										*/
/* @parameter[in]		- pointer to current date handle structure										*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void RTC_GetTime(RTC_Handle_t * pRTCHandle, Current_Date_Handle_t* pCurrentDateHandle)
{
	RTC_TR_Reg_t TR_temp = {0};

	//Read the time values
	TR_temp.reg = RTC->TR.reg;

	while (!(SET == RTC->ISR.bit.rsf))
	{
		//wait till the RTC time is updated
	}

	//read the time
	pCurrentDateHandle->Time.hour        = TR_temp.bit.ht  * 10 + TR_temp.bit.hu;
	pCurrentDateHandle->Time.minute      = TR_temp.bit.mnt * 10 + TR_temp.bit.mnu;
	pCurrentDateHandle->Time.second      = TR_temp.bit.st  * 10 + TR_temp.bit.su;
	pCurrentDateHandle->Time.hour_format = RTC->CR.bit.fmt;
}

/********************************************************************************************************/
/* @function name 		- RTC_GetDate																	*/
/*																										*/
/* @brief				- This function fills time related variables									*/
/*																										*/
/* @parameter[in]		- pointer to RTC Handle structure												*/
/*																										*/
/* @parameter[in]		- pointer to current date handle structure										*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void RTC_GetDate(RTC_Handle_t* pRTCHandle, Current_Date_Handle_t* pCurrentDateHandle)
{
	RTC_DR_Reg_t DR_temp = {0};

	//Read the date values
	DR_temp.reg = RTC->DR.reg;

	pCurrentDateHandle->Date.year = DR_temp.bit.yt * 10;
	pCurrentDateHandle->Date.year += DR_temp.bit.yu;

	pCurrentDateHandle->Date.month = DR_temp.bit.mt * 10;
	pCurrentDateHandle->Date.month += DR_temp.bit.mu;

	pCurrentDateHandle->Date.date = DR_temp.bit.dt * 10;
	pCurrentDateHandle->Date.date += DR_temp.bit.du;

	pCurrentDateHandle->Date.weekDay = DR_temp.bit.wdu;
}


void RTC_SystemClock_Config(uint32_t clk)
{
	RCC_OscInit_t rcc_osc = {0};
	RCC_ClkInit_t rcc_clk = {0};

	//osc init
	rcc_osc.OscillatorType = RCC_OSCILLATORTYPE_LSI;
	rcc_osc.LSIState = RCC_LSI_ON;
	rcc_osc.PLL.State = RCC_PLL_ON;
	rcc_osc.PLL.Source = RCC_PLLCFGR_PLLSRC_HSE; 			// Use HSE as PLL input clock source
	rcc_osc.PLL.M = 4;										// HSE oscillator clock is 8MHz
	rcc_osc.PLL.N = (clk / HSE_VALUE) * rcc_osc.PLL.M * 2;		// Calculate PLL multiplication factor
	rcc_osc.PLL.P = 0;										// Set P to 0 (i.e. divide by 2) to get 84MHz system clock
	rcc_osc.PLL.Q = 4;										// Set Q to 4 to get 84MHz clock for USB OTG and SDIO
	RCC_OscConfig(&rcc_osc);

	//select RTC clock source as LSI
	RCC->BDCR.bit.rtcsel = RCC_BDCR_RTCSEL_LSI;

	rcc_clk.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
	rcc_clk.AHBCLKDivider  = RCC_SYSCLK_DIV1;
	rcc_clk.APB1CLKDivider = RCC_HCLK_DIV4;
	rcc_clk.APB2CLKDivider = RCC_HCLK_DIV2;

	RCC_ClockConfig(&rcc_clk);

	// Set AHB, APB1, and APB2 prescalers
	if(clk <= 36000000)
	{
		rcc_clk.AHBCLKDivider = 0;  // AHB prescaler = 1
		rcc_clk.APB1CLKDivider = 0; // APB1 prescaler = 1
		rcc_clk.APB2CLKDivider = 0; // APB2 prescaler = 1
	}
	else if(clk <= 72000000)
	{
		rcc_clk.AHBCLKDivider  = 9; // AHB prescaler = 1
		rcc_clk.APB1CLKDivider = 5; // APB1 prescaler = 2
		rcc_clk.APB2CLKDivider = 4; // APB2 prescaler = 2
	}
	else if(clk <= 108000000)
	{
		rcc_clk.AHBCLKDivider = 0; // AHB prescaler = 1
		rcc_clk.APB1CLKDivider = 5; // APB1 prescaler = 4
		rcc_clk.APB2CLKDivider = 4; // APB2 prescaler = 2
	}
	else
	{
		rcc_clk.AHBCLKDivider = 0; 	// AHB prescaler = 1
		rcc_clk.APB1CLKDivider = 5; // APB1 prescaler = 4
		rcc_clk.APB2CLKDivider = 4; // APB2 prescaler = 2
	}

}

/*
 * Helper functions
 */
// Convert a 4-digit year value to BCD format
static uint8_t RTC_ConvertYear(uint16_t year)
{
    uint8_t bcd_year = 0;
    bcd_year |= ((year % 100 /10) <<4);
    bcd_year |= (year % 10 <<0);
    return bcd_year;
}

// Function to convert a decimal month to BCD format
static uint8_t RTC_ConvertMonth(uint8_t month)
{
	uint8_t bcd_month = 0;
    if(month > 9)
    {
    	bcd_month |= month/10 << 4;
    }
    bcd_month |= month%10 << 0;
    return bcd_month;
}

// Function to convert a decimal date to BCD format
static uint8_t RTC_ConvertDate(uint8_t date)
{
    uint8_t bcd_date = 0;
    if(date > 9)
	{
    	bcd_date |= date/10 << 4;
	}
	bcd_date |= date%10 << 0;
	return bcd_date;
}


/****************************************************** End of file ******************************************************/

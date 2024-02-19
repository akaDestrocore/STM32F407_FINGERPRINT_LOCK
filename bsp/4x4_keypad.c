#include <4x4_keypad.h>
#include <stdint.h>

/*
 * Keypad Button Characters
 */
const char Keypad_Button_Chars[4][4] =  {  	  {'1', '2', '3', 'A'},
	                                          {'4', '5', '6', 'B'},
	                                          {'7', '8', '9', 'C'},
	                                          {'*', '0', '#', 'D'},
                                         };


static void set_keypad_row(uint8_t row);
static char read_keypad_column(uint8_t row);


/*
 * Initialization and de-initialization
 */
/********************************************************************************************************/
/* @function name 		- Keypad4x4_Init																*/
/*																										*/
/* @brief				- This function initializes given GPIO ports that are used for keypad			*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void Keypad4x4_Init(void)
{
	GPIO_Handle_t Keypad4x4h = {0};

	Keypad4x4h.pGPIOx = KEYPAD_ROW_1_PORT;
	Keypad4x4h.GPIO_Config.PinNumber = KEYPAD_ROW_1_PIN;
	Keypad4x4h.GPIO_Config.PinMode = GPIO_MODE_OUTPUT;
	Keypad4x4h.GPIO_Config.PinOPType = GPIO_OUTPUT_PP;
	Keypad4x4h.GPIO_Config.PinPuPdControl = GPIO_PIN_NO_PUPD;
	Keypad4x4h.GPIO_Config.PinSpeed = GPIO_SPEED_LOW;
	GPIO_Init(&Keypad4x4h);

	Keypad4x4h.pGPIOx = KEYPAD_ROW_2_PORT;
	Keypad4x4h.GPIO_Config.PinNumber = KEYPAD_ROW_2_PIN;
	GPIO_Init(&Keypad4x4h);

	Keypad4x4h.pGPIOx = KEYPAD_ROW_3_PORT;
	Keypad4x4h.GPIO_Config.PinNumber = KEYPAD_ROW_3_PIN;
	GPIO_Init(&Keypad4x4h);

	Keypad4x4h.pGPIOx = KEYPAD_ROW_4_PORT;
	Keypad4x4h.GPIO_Config.PinNumber = KEYPAD_ROW_4_PIN;
	GPIO_Init(&Keypad4x4h);

	Keypad4x4h.pGPIOx = KEYPAD_COLUMN_1_PORT;
	Keypad4x4h.GPIO_Config.PinNumber = KEYPAD_COLUMN_1_PIN;
	Keypad4x4h.GPIO_Config.PinMode = GPIO_MODE_INPUT;
	Keypad4x4h.GPIO_Config.PinPuPdControl = GPIO_PIN_PULL_UP;
	GPIO_Init(&Keypad4x4h);

	Keypad4x4h.pGPIOx = KEYPAD_COLUMN_2_PORT;
	Keypad4x4h.GPIO_Config.PinNumber = KEYPAD_COLUMN_2_PIN;
	GPIO_Init(&Keypad4x4h);

	Keypad4x4h.pGPIOx = KEYPAD_COLUMN_3_PORT;
	Keypad4x4h.GPIO_Config.PinNumber = KEYPAD_COLUMN_3_PIN;
	GPIO_Init(&Keypad4x4h);

	Keypad4x4h.pGPIOx = KEYPAD_COLUMN_4_PORT;
	Keypad4x4h.GPIO_Config.PinNumber = KEYPAD_COLUMN_4_PIN;
	GPIO_Init(&Keypad4x4h);
}


/*
 * Keypad read state function
 */
/********************************************************************************************************/
/* @function name 		- Keypad4x4_Read																*/
/*																										*/
/* @brief				- This function returns the value received by given keypad pin					*/
/*																										*/
/* @return				- return the character value that corresponds to the given pin					*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
char Keypad4x4_Read(void)
{
	char val;
	set_keypad_row(1);
	val = read_keypad_column(1);
	if(val)
	{
		return val;
	}

	set_keypad_row(2);
	val = read_keypad_column(2);
	if(val)
	{
		return val;
	}

	set_keypad_row(3);
	val = read_keypad_column(3);
	if(val)
	{
		return val;
	}

	set_keypad_row(4);
	val = read_keypad_column(4);
	if(val)
	{
		return val;
	}

	return KEYPAD_NOT_PRESSED;
}


static void set_keypad_row(uint8_t row)
{
	GPIO_WritePin(KEYPAD_ROW_1_PORT, KEYPAD_ROW_1_PIN, SET);
	GPIO_WritePin(KEYPAD_ROW_2_PORT, KEYPAD_ROW_2_PIN, SET);
	GPIO_WritePin(KEYPAD_ROW_3_PORT, KEYPAD_ROW_3_PIN, SET);
	GPIO_WritePin(KEYPAD_ROW_4_PORT, KEYPAD_ROW_4_PIN, SET);

	if(1 == row)
	{
		GPIO_WritePin(KEYPAD_ROW_1_PORT, KEYPAD_ROW_1_PIN, RESET);
	}
	if(2 == row)
	{
		GPIO_WritePin(KEYPAD_ROW_2_PORT, KEYPAD_ROW_2_PIN, RESET);
	}
	if(3 == row)
	{
		GPIO_WritePin(KEYPAD_ROW_3_PORT, KEYPAD_ROW_3_PIN, RESET);
	}
	if(4 == row)
	{
		GPIO_WritePin(KEYPAD_ROW_4_PORT, KEYPAD_ROW_4_PIN, RESET);
	}
}

static char read_keypad_column(uint8_t row)
{
	if(RESET == GPIO_ReadPin(KEYPAD_COLUMN_1_PORT, KEYPAD_COLUMN_1_PIN))
	{
		while(RESET == GPIO_ReadPin(KEYPAD_COLUMN_1_PORT, KEYPAD_COLUMN_1_PIN))
		{

		}
		return Keypad_Button_Chars[row-1][0];
	}

	if(RESET == GPIO_ReadPin(KEYPAD_COLUMN_2_PORT, KEYPAD_COLUMN_2_PIN))
	{
		while(RESET == GPIO_ReadPin(KEYPAD_COLUMN_2_PORT, KEYPAD_COLUMN_2_PIN))
		{

		}
		return Keypad_Button_Chars[row-1][1];
	}

	if(RESET == GPIO_ReadPin(KEYPAD_COLUMN_3_PORT, KEYPAD_COLUMN_3_PIN))
	{
		while(RESET == GPIO_ReadPin(KEYPAD_COLUMN_3_PORT, KEYPAD_COLUMN_3_PIN))
		{

		}
		return Keypad_Button_Chars[row-1][2];
	}

	if(RESET == GPIO_ReadPin(KEYPAD_COLUMN_4_PORT, KEYPAD_COLUMN_4_PIN))
	{
		while(RESET == GPIO_ReadPin(KEYPAD_COLUMN_4_PORT, KEYPAD_COLUMN_4_PIN))
		{

		}
		return Keypad_Button_Chars[row-1][3];
	}

	return KEYPAD_NOT_PRESSED;
}

/****************************************************** End of file ******************************************************/

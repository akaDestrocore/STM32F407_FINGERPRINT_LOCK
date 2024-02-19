#ifndef _4X4_KEYPAD_H_
#define _4X4_KEYPAD_H_

#include <stm32f407xx.h>

/********************************************************************************************/
/*		 						Define your GPIO pins here									*/
/********************************************************************************************/

/**
  Keypad		PIN	 			 Description      	 I/O

     R1			PB15				Row 1           Output
     R2			PB13				Row 2           Output
     R3			PB11				Row 3           Output
     R4			PE15				Row 4           Output

     C1			PE13				Column 1        Input
     C2			PE11				Column 2        Input
     C3			PE9					Column 3        Input
     C4			PE7					Column 4        Input
**/


#define KEYPAD_ROW_1_PORT		GPIOB
#define KEYPAD_ROW_2_PORT		GPIOB
#define KEYPAD_ROW_3_PORT		GPIOB
#define KEYPAD_ROW_4_PORT		GPIOE

#define KEYPAD_COLUMN_1_PORT 	GPIOE
#define KEYPAD_COLUMN_2_PORT	GPIOE
#define KEYPAD_COLUMN_3_PORT	GPIOE
#define KEYPAD_COLUMN_4_PORT	GPIOE


typedef enum
{
	KEYPAD_ROW_1_PIN = GPIO_PIN_15,
	KEYPAD_ROW_2_PIN = GPIO_PIN_13,
	KEYPAD_ROW_3_PIN = GPIO_PIN_11,
	KEYPAD_ROW_4_PIN = GPIO_PIN_15,
	KEYPAD_COLUMN_1_PIN = GPIO_PIN_13,
	KEYPAD_COLUMN_2_PIN = GPIO_PIN_11,
	KEYPAD_COLUMN_3_PIN = GPIO_PIN_9,
	KEYPAD_COLUMN_4_PIN = GPIO_PIN_7
}Keypad_Pin_t;


#define NULL_CHARACTER              '\0'
#define KEYPAD_NOT_PRESSED			NULL_CHARACTER

//Initialization
void Keypad4x4_Init(void);

//Specific GPIO state read function
char Keypad4x4_Read(void);

#endif /* _4X4_KEYPAD_H_ */

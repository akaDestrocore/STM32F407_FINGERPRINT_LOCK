#include <password.h>

extern volatile uint32_t counter_1s;			//needed for password entering timeout
volatile uint32_t alarm_time = 15;				//default alarm working time is 15 sec
char keypad_input[20];
char admin_password[6] = "555555";
char input_password[6];							//array for entered password
char user_password[4] = "1234";
char alarm_time_char[3];						//array for alarm working time
extern char key;
extern uint8_t SystemState;



/********************************************************************************************************/
/* @function name 		- BuzzerAlert																	*/
/*																										*/
/* @brief				- function to produce desired sound and write the corresponding message to the	*/
/*						  screen																		*/
/*																										*/
/* @parameter[in]		- sound value - @Buzzer_Sound 													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void BuzzerAlert(uint8_t sound)
{
	uint8_t i = 0;
	switch(sound)
	{
		case BUZZER_ALARM_SOUND:
		{
			ST7735_FillScreen(ST7735_RED);
			ST7735_DrawString(0, 40, "      EITHER YOUR                             FINGERPRINT NOT FOUND                           OR THE PASSWORD IS                                  WRONG!", Font_7x10, ST7735_WHITE, ST7735_RED);
			alert_icon();
			while(i < alarm_time)
			{
				for(int k = 0; k < 255; k++)
				{
					TIM2->CCR1 = k;
					delay_ms(2);
				}
				ST7735_FillScreen(ST7735_BLUE);
				ST7735_DrawString(0, 40, "                            EITHER YOUR                             FINGERPRINT NOT FOUND                           OR THE PASSWORD IS                                  WRONG!", Font_7x10, ST7735_WHITE, ST7735_BLUE);
				alert_icon();
				for(int k = 100; k > 0; k--)
				{
					TIM2->CCR1 = k;
					delay_ms(2);
				}
				ST7735_FillScreen(ST7735_RED);
				ST7735_DrawString(0, 40, "                            EITHER YOUR                             FINGERPRINT NOT FOUND                           OR THE PASSWORD IS                                  WRONG!", Font_7x10, ST7735_WHITE, ST7735_RED);
				alert_icon();
				i++;
			}
			TIM2->CCR1 = 0;
			WelcomeScreen();
			break;
		}
		case BUZZER_ACCESS_GRANTED_SOUND:
		{
			while(i < 10)
			{
				for(int k = 0; k < 10; k++)
				{
					TIM2->CCR1 = k;
					delay_ms(4);
				}

				for(int k=10; k>0; k--)
				{
					TIM2->CCR1 = k;
					delay_ms(4);
				}
				i++;
			}
			TIM2->CCR1 = 0;
			break;
		}
		case BUZZER_ERROR_SOUND:
		{
			for(int k = 5; k > 0; k--)
			{
				TIM2->CCR1 = k;
				delay_ms(50);
			}
			TIM2->CCR1 = 0;
			delay_ms(20);
			for(int k = 5; k > 0; k--)
			{
				TIM2->CCR1 = k;
				delay_ms(40);
			}
			TIM2->CCR1 = 0;
			break;
		}
		case BUZZER_INIT_SOUND:
		{
			for(int k = 5; k > 0; k--)
			{
				TIM2->CCR1 = k;
				delay_ms(30);
			}
			TIM2->CCR1 = 0;
			delay_ms(90);
			for(int k = 5; k > 0; k--)
			{
				TIM2->CCR1 = k;
			}
			TIM2->CCR1 = 0;
			delay_ms(60);
			for(int k = 5; k > 0; k--)
			{
				TIM2->CCR1 = k;
				delay_ms(70);
			}
			TIM2->CCR1 = 0;
			delay_ms(3);
			for(int k = 5; k > 0; k--)
			{
				TIM2->CCR1 = k;
				delay_ms(25);
			}
			TIM2->CCR1 = 0;
			break;
		}
	}
}


/********************************************************************************************************/
/* @function name 		- WelcomeScreen																	*/
/*																										*/
/* @brief				- function to switch system state and write corresponding message to screen		*/
/*																										*/
/* @parameter[in]		- user fingerprint ID that was found in R308 FLASH memory						*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- 'EnterPassword()' will be called after switching the SystemState				*/
/********************************************************************************************************/
void SecondStepAuth(uint16_t fingerID)
{
	ST7735_FillScreen(ST7735_GREEN);
	ST7735_Print(0, 30, "    FOUND        FINGERPRINT      ID =", fingerID, Font_11x18, ST7735_WHITE, ST7735_GREEN);
	SystemState = SYS_STATE_READ_PWD;
	delay_ms(400);
}


/********************************************************************************************************/
/* @function name 		- WelcomeScreen																	*/
/*																										*/
/* @brief				- function to print user instruction to the screen								*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void WelcomeScreen(void)
{
	ST7735_FillScreen(ST7735_WHITE);
	ST7735_DrawString(10, 30, "PLEASE PRESS                    ANY KEY", Font_11x18, ST7735_BLUE, ST7735_WHITE);
	return;
}

/********************************************************************************************************/
/* @function name 		- EnterPassword																	*/
/*																										*/
/* @brief				- function to capture password input from keypad within the given time			*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- this function is called for any password entrance related operations			*/
/********************************************************************************************************/
void EnterPassword(void)
{
	counter_1s = 0;
	int	digit_number = 0;
	switch(SystemState)
	{
		case SYS_STATE_READ_PWD:
		{
			ST7735_FillScreen(ST7735_WHITE);
			ST7735_DrawString(0, 40, "ENTER PASSWORD", Font_11x18, ST7735_BLUE, ST7735_WHITE);
			break;
		}
		case SYS_STATE_ADM_NEW_USR_PWD:
		{
			ST7735_FillScreen(ST7735_BLACK);
			ST7735_DrawString(0, 10, "PLEASE ENTER     NEW USER       PASSWORD:", Font_11x18, ST7735_GREEN, ST7735_BLACK);
			break;
		}
		case SYS_STATE_READ_ADMIN_PWD:
		{
			ST7735_FillScreen(ST7735_BLACK);
			ST7735_DrawString(0, 40, "ENTER ADMIN PASSWORD:", Font_7x10, ST7735_GREEN, ST7735_BLACK);
			break;
		}
		case SYS_STATE_ADM_NEW_PWD:
		{
			ST7735_FillScreen(ST7735_BLACK);
			ST7735_DrawString(0, 10, "   ENTER NEW  ADMIN PASSWORD", Font_11x18, ST7735_GREEN, ST7735_BLACK);
			break;
		}
		default:
			break;
	}

	if((SYS_STATE_READ_PWD == SystemState) || (SYS_STATE_ADM_NEW_USR_PWD == SystemState))
	{
		while(digit_number < 4)
		{
			if(counter_1s < 10)
			{
				key = KEYPAD_NOT_PRESSED;
				key = Keypad4x4_Read();
				if(key != KEYPAD_NOT_PRESSED)
				{
					if(key == '0'){input_password[digit_number] = '0'; digit_number++; ST7735_DrawString(4+digit_number*27.5, 100, "*", Font_16x26, ST7735_BLUE, ST7735_WHITE); delay_ms(70); continue;}
					if(key == '1'){input_password[digit_number] = '1'; digit_number++; ST7735_DrawString(4+digit_number*27.5, 100, "*", Font_16x26, ST7735_BLUE, ST7735_WHITE); delay_ms(70); continue;}
					if(key == '2'){input_password[digit_number] = '2'; digit_number++; ST7735_DrawString(4+digit_number*27.5, 100, "*", Font_16x26, ST7735_BLUE, ST7735_WHITE); delay_ms(70); continue;}
					if(key == '3'){input_password[digit_number] = '3'; digit_number++; ST7735_DrawString(4+digit_number*27.5, 100, "*", Font_16x26, ST7735_BLUE, ST7735_WHITE); delay_ms(70); continue;}
					if(key == '4'){input_password[digit_number] = '4'; digit_number++; ST7735_DrawString(4+digit_number*27.5, 100, "*", Font_16x26, ST7735_BLUE, ST7735_WHITE); delay_ms(70); continue;}
					if(key == '5'){input_password[digit_number] = '5'; digit_number++; ST7735_DrawString(4+digit_number*27.5, 100, "*", Font_16x26, ST7735_BLUE, ST7735_WHITE); delay_ms(70); continue;}
					if(key == '6'){input_password[digit_number] = '6'; digit_number++; ST7735_DrawString(4+digit_number*27.5, 100, "*", Font_16x26, ST7735_BLUE, ST7735_WHITE); delay_ms(70); continue;}
					if(key == '7'){input_password[digit_number] = '7'; digit_number++; ST7735_DrawString(4+digit_number*27.5, 100, "*", Font_16x26, ST7735_BLUE, ST7735_WHITE); delay_ms(70); continue;}
					if(key == '8'){input_password[digit_number] = '8'; digit_number++; ST7735_DrawString(4+digit_number*27.5, 100, "*", Font_16x26, ST7735_BLUE, ST7735_WHITE); delay_ms(70); continue;}
					if(key == '9'){input_password[digit_number] = '9'; digit_number++; ST7735_DrawString(4+digit_number*27.5, 100, "*", Font_16x26, ST7735_BLUE, ST7735_WHITE); delay_ms(70); continue;}
					if(key == 'A'){delay_ms(70); continue;}
					if(key == 'B'){delay_ms(70); continue;}
					if(key == 'C'){ST7735_DrawString(4+(digit_number)*27.5, 100, " ", Font_16x26, ST7735_BLUE, ST7735_WHITE); digit_number--; delay_ms(70); continue;}
					if(key == 'D'){delay_ms(70); continue;}
					if(key == '*'){delay_ms(70); continue;}
					if(key == '#'){delay_ms(70); continue;}
				}
			}else
			{
				TimeoutMessage();
				return;
			}
		}
		if((4 == digit_number) && (SYS_STATE_READ_PWD == SystemState))
		{//user entered their password
			digit_number = 0;
			SystemState = SYS_STATE_PWD_CONFIRMATION;
			return;
		}
		if((4 == digit_number) && (SYS_STATE_ADM_NEW_USR_PWD == SystemState))
		{
			user_password[0] = input_password[0];
			user_password[1] = input_password[1];
			user_password[2] = input_password[2];
			user_password[3] = input_password[3];

			input_password[0] = admin_password[0];
			input_password[1] = admin_password[1];
			input_password[2] = admin_password[2];
			input_password[3] = admin_password[3];
			digit_number = 0;
			SystemState = SYS_STATE_ADM_PWD_MENU;
		}
	}
	if((SYS_STATE_READ_ADMIN_PWD == SystemState) || (SYS_STATE_ADM_NEW_PWD == SystemState))
	{
		while(digit_number < 6)
		{
			if(counter_1s < 20)
			{
				key = KEYPAD_NOT_PRESSED;
				key = Keypad4x4_Read();
				if(key != KEYPAD_NOT_PRESSED)
				{
					if(key == '0'){input_password[digit_number] = '0'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == '1'){input_password[digit_number] = '1'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == '2'){input_password[digit_number] = '2'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == '3'){input_password[digit_number] = '3'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == '4'){input_password[digit_number] = '4'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == '5'){input_password[digit_number] = '5'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == '6'){input_password[digit_number] = '6'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == '7'){input_password[digit_number] = '7'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == '8'){input_password[digit_number] = '8'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == '9'){input_password[digit_number] = '9'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == 'A'){input_password[digit_number] = 'A'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == 'B'){input_password[digit_number] = 'B'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == 'C'){input_password[digit_number] = 'C'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == 'D'){input_password[digit_number] = 'D'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == '*'){input_password[digit_number] = '*'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
					if(key == '#'){input_password[digit_number] = '#'; digit_number++; ST7735_DrawString(4+digit_number*18, 100, "*", Font_16x26, ST7735_GREEN, ST7735_BLACK); delay_ms(70); continue;}
				}
			}else
			{
				TimeoutMessage();
				return;
			}
		}
		if((6 == digit_number) && (SYS_STATE_READ_ADMIN_PWD == SystemState))
		{//user has entered admin password
			SystemState = SYS_STATE_ADM_SCREEN;
			digit_number = 0;
			return;
		}
		if((6 == digit_number) && (SYS_STATE_ADM_NEW_PWD == SystemState))
		{//admin has changed admin password
			admin_password[0] = input_password[0];
			admin_password[1] = input_password[1];
			admin_password[2] = input_password[2];
			admin_password[3] = input_password[3];
			admin_password[4] = input_password[4];
			admin_password[5] = input_password[5];
			digit_number = 0;
			SystemState = SYS_STATE_ADM_PWD_MENU;
		}
	}
}


/********************************************************************************************************/
/* @function name 		- ChangePasswordMenu															*/
/*																										*/
/* @brief				- menu interface to pick from available password related operations				*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void ChangePasswordMenu(void)
{
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawString(0, 0, "SELECT THE PASSWORD    YOU WANT TO CHANGE", Font_7x10, ST7735_GREEN, ST7735_BLACK);
	ST7735_DrawString(5, 30, "A-USER                                         B-ADMINISTRATOR                               D-BACK", Font_7x10, ST7735_GREEN, ST7735_BLACK);
	key = KEYPAD_NOT_PRESSED;
	while(KEYPAD_NOT_PRESSED == key)
	{
		key = Keypad4x4_Read();
		if(KEYPAD_NOT_PRESSED != key)
		{
			switch(key)
			{
				case 'A':
				{
					ChangeUserPassword();
					return;
				}
				case 'B':
				{
					ChangeAdminPassword();
					return;
				}
				case 'D':
				{
					SystemState = SYS_STATE_ADM_SCREEN;
					return;
				}
				case '#':
				{
					SystemState = SYS_STATE_BEGIN;
					input_password[0] = '\0';
					input_password[1] = '\0';
					input_password[2] = '\0';
					input_password[3] = '\0';
					ST7735_FillScreen(ST7735_BLACK);
					ST7735_DrawString(60, 50, "BYE!", Font_11x18, ST7735_GREEN, ST7735_BLACK);
					delay_ms(2000);
					WelcomeScreen();
					return;
				}
				default:
					ChangePasswordMenu();
			}
		}
	}
}


/********************************************************************************************************/
/* @function name 		- DatabaseOperationsMenu														*/
/*																										*/
/* @brief				- menu interface to pick from available fingerprint database related operations	*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void DatabaseOperationsMenu(void)
{
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawString(0, 0, "SELECT DATABASE        OPERATION", Font_7x10, ST7735_GREEN, ST7735_BLACK);
	ST7735_DrawString(5, 40, "A-ENROLLMENT                                   B-DELETE ID                                   C-EMPTY DATABASE                              D-BACK", Font_7x10, ST7735_GREEN, ST7735_BLACK);
	key = KEYPAD_NOT_PRESSED;
	while(KEYPAD_NOT_PRESSED == key)
	{
		key = Keypad4x4_Read();
		if(KEYPAD_NOT_PRESSED != key)
		{
			switch(key)
			{
				case 'A':
				{
					EnrollmentAttempt();
					return;
				}
				case 'B':
				{
					SystemState = SYS_STATE_ADM_DB_DEL_ID;
					return;
				}
				case 'C':
				{
					SystemState = SYS_STATE_ADM_DB_EMPTY;
					return;
				}
				case 'D':
				{
					SystemState = SYS_STATE_ADM_SCREEN;
					return;
				}
				case '#':
				{
					SystemState = SYS_STATE_BEGIN;
					input_password[0] = '\0';
					input_password[1] = '\0';
					input_password[2] = '\0';
					input_password[3] = '\0';
					ST7735_FillScreen(ST7735_BLACK);
					ST7735_DrawString(60, 50, "BYE!", Font_11x18, ST7735_GREEN, ST7735_BLACK);
					delay_ms(2000);
					WelcomeScreen();
					return;
				}
				default:
					DatabaseOperationsMenu();
			}
		}
	}
}


/********************************************************************************************************/
/* @function name 		- EmptyDBWarning																*/
/*																										*/
/* @brief				- ask permission before emptying fingerprint library of R308					*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void EmptyDBWarning(void)
{
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawString(0, 0, "ARE YOU SURE YOU WANT TO EMPTY DATABASE?", Font_7x10, ST7735_GREEN, ST7735_BLACK);
	ST7735_DrawString(20, 80, "A-YES D-NO", Font_11x18, ST7735_GREEN, ST7735_BLACK);
	key = KEYPAD_NOT_PRESSED;
	while(KEYPAD_NOT_PRESSED == key)
	{
		key = Keypad4x4_Read();
		if(KEYPAD_NOT_PRESSED != key)
		{
			switch(key)
			{
				case 'A':
				{
					EmptyDB();
					return;
				}
				case 'D':
				{
					SystemState = SYS_STATE_ADM_DB_MENU;
					return;
				}
				case '#':
				{
					SystemState = SYS_STATE_BEGIN;
					input_password[0] = '\0';
					input_password[1] = '\0';
					input_password[2] = '\0';
					input_password[3] = '\0';
					ST7735_FillScreen(ST7735_BLACK);
					ST7735_DrawString(60, 50, "BYE!", Font_11x18, ST7735_GREEN, ST7735_BLACK);
					delay_ms(2000);
					WelcomeScreen();
					return;
				}
				default:
					EmptyDBWarning();
			}
		}
	}
}


/********************************************************************************************************/
/* @function name 		- ChangeAdminPassword															*/
/*																										*/
/* @brief				- ask permission before proceeding to changing administrator password			*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void ChangeAdminPassword(void)
{
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawString(0, 0, "ARE YOU SURE YOU WANT TO CHANGE ADMIN PASSWORD?", Font_7x10, ST7735_GREEN, ST7735_BLACK);
	ST7735_DrawString(20, 80, "A-YES D-NO", Font_11x18, ST7735_GREEN, ST7735_BLACK);
	key = KEYPAD_NOT_PRESSED;
	while(KEYPAD_NOT_PRESSED == key)
	{
		key = Keypad4x4_Read();
		if(KEYPAD_NOT_PRESSED != key)
		{
			switch(key)
			{
				case 'A':
				{
					SystemState = SYS_STATE_ADM_NEW_PWD;
					return;
				}
				case 'D':
				{
					return;
				}
				case '#':
				{
					SystemState = SYS_STATE_BEGIN;
					input_password[0] = '\0';
					input_password[1] = '\0';
					input_password[2] = '\0';
					input_password[3] = '\0';
					ST7735_FillScreen(ST7735_BLACK);
					ST7735_DrawString(60, 50, "BYE!", Font_11x18, ST7735_GREEN, ST7735_BLACK);
					delay_ms(2000);
					WelcomeScreen();
					return;
				}
				default:
					ChangeAdminPassword();
			}
		}
	}
}


/********************************************************************************************************/
/* @function name 		- ChangeUserPassword															*/
/*																										*/
/* @brief				- ask permission before proceeding to changing user password					*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void ChangeUserPassword(void)
{
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawString(0, 0, "ARE YOU SURE YOU WANT TO CHANGE USER PASSWORD?", Font_7x10, ST7735_GREEN, ST7735_BLACK);
	ST7735_DrawString(20, 80, "A-YES D-NO", Font_11x18, ST7735_GREEN, ST7735_BLACK);
	key = KEYPAD_NOT_PRESSED;
	while(KEYPAD_NOT_PRESSED == key)
	{
		key = Keypad4x4_Read();
		if(KEYPAD_NOT_PRESSED != key)
		{
			switch(key)
			{
				case 'A':
				{
					SystemState = SYS_STATE_ADM_NEW_USR_PWD;
					return;
				}
				case 'D':
				{
					return;
				}
				case '#':
				{
					SystemState = SYS_STATE_BEGIN;
					input_password[0] = '\0';
					input_password[1] = '\0';
					input_password[2] = '\0';
					input_password[3] = '\0';
					ST7735_FillScreen(ST7735_BLACK);
					ST7735_DrawString(60, 50, "BYE!", Font_11x18, ST7735_GREEN, ST7735_BLACK);
					delay_ms(2000);
					WelcomeScreen();
					return;
				}
				default:
					ChangeUserPassword();
			}
		}
	}
}


/********************************************************************************************************/
/* @function name 		- ChangeAlarmWarning															*/
/*																										*/
/* @brief				- ask permission before proceeding to changing alarm duration time				*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void ChangeAlarmWarning(void)
{
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawString(0, 0, "ARE YOU SURE YOU WANT TO MODIFY ALARM TIME?", Font_7x10, ST7735_GREEN, ST7735_BLACK);
	ST7735_DrawString(20, 80, "A-YES D-NO", Font_11x18, ST7735_GREEN, ST7735_BLACK);
	key = KEYPAD_NOT_PRESSED;
	while(KEYPAD_NOT_PRESSED == key)
	{
		key = Keypad4x4_Read();
		if(KEYPAD_NOT_PRESSED != key)
		{
			switch(key)
			{
				case 'A':
				{
					SystemState = SYS_STATE_ADM_ALARM_TIME;
					ST7735_FillScreen(ST7735_BLACK);
					return;
				}
				case 'D':
				{
					AdminSettingsMenu();
					return;
				}
				case '#':
				{
					SystemState = SYS_STATE_BEGIN;
					input_password[0] = '\0';
					input_password[1] = '\0';
					input_password[2] = '\0';
					input_password[3] = '\0';
					ST7735_FillScreen(ST7735_BLACK);
					ST7735_DrawString(60, 50, "BYE!", Font_11x18, ST7735_GREEN, ST7735_BLACK);
					delay_ms(2000);
					WelcomeScreen();
					return;
				}
				default:
					ChangeAlarmWarning();
			}
		}
	}
}


/********************************************************************************************************/
/* @function name 		- SetNewAlarmTime																*/
/*																										*/
/* @brief				- set new alarm duration time													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void SetNewAlarmTime(void)
{
    int digit_number = 0;
    int save = 0;
    ST7735_InvertColors(false);
    ST7735_DrawString(0, 0, "PLEASE ENTER NEW ALARM TIME[DEFAULT=15](PRESS 'D' TO SAVE)", Font_7x10, ST7735_GREEN, ST7735_BLACK);

    while ((digit_number < 3) && (SET != save))
    {
        key = KEYPAD_NOT_PRESSED;
        key = Keypad4x4_Read();

        if (KEYPAD_NOT_PRESSED != key)
        {
            if ((key >= '0' && key <= '9') || ('D' == key))
            {
                keypad_input[digit_number] = key;

                char temp_string[2];
                temp_string[0] = key;
                temp_string[1] = '\0';

                ST7735_DrawString(30 + digit_number * 27.5, 100, temp_string, Font_16x26, ST7735_GREEN, ST7735_BLACK);
                delay_ms(70);

                if (key >= '0' && key <= '9')
                {
                    digit_number++;
                }
                else if ('D' == key)
                {
                    save++;
                    delay_ms(300);
                }
            }
            else if (('C' == key) && (digit_number > 0))
            {
                ST7735_DrawString(30 + digit_number * 27.5, 100, " ", Font_16x26, ST7735_GREEN, ST7735_BLACK);
                digit_number--;
            }
        }
    }

    if ((SET == save) && (digit_number > 0))
    {
        int alarm_value = 0;
        for (int i = 0; i < digit_number; i++)
        {
            alarm_value = alarm_value * 10 + (keypad_input[i] - '0');
        }

        alarm_time = alarm_value;
        SystemState = SYS_STATE_ADM_SCREEN;
    }
}

/********************************************************************************************************/
/* @function name 		- TimeoutMessage																*/
/*																										*/
/* @brief				- print timeout message on the screen											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void TimeoutMessage(void)
{
	ST7735_FillScreen(ST7735_WHITE);
	ST7735_DrawString(0, 30, "    TIMEOUT.                   REAUTHORIZE                      PLEASE", Font_11x18, ST7735_BLUE, ST7735_WHITE);
	BuzzerAlert(BUZZER_ERROR_SOUND);
	delay_ms(500);
	SystemState = SYS_STATE_BEGIN;
	WelcomeScreen();
	return;
}


/********************************************************************************************************/
/* @function name 		- PasswordConfirmation															*/
/*																										*/
/* @brief				- unlocks the lock if user entered correct password	within given time			*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void PasswordConfirmation(void)
{
	counter_1s = 0;
	if(counter_1s < 20)
	{
		if((user_password[0] == input_password[0]) && (user_password[1] == input_password[1])
		&& (user_password[2] == input_password[2]) && (user_password[3] == input_password[3]))
		{
			ST7735_FillScreen(ST7735_GREEN);
			ST7735_DrawString(0, 50, "   CORRECT        PASSWORD", Font_11x18, ST7735_WHITE, ST7735_GREEN);
			delay_ms(100);
			input_password[0] = '\0';
			input_password[1] = '\0';
			input_password[2] = '\0';
			input_password[3] = '\0';
			Unlock();
		}else
		{
			SystemState = SYS_STATE_BEGIN;
			input_password[0] = '\0';
			input_password[1] = '\0';
			input_password[2] = '\0';
			input_password[3] = '\0';
			BuzzerAlert(BUZZER_ALARM_SOUND);
			WelcomeScreen();
			return;
		}
	}
	else
	{
		TimeoutMessage();
		return;
	}
}


/********************************************************************************************************/
/* @function name 		- AdminSettingsMenu																*/
/*																										*/
/* @brief				- menu interface for some of the system configuration settings					*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- '#' key is used to exit administrator settings								*/
/********************************************************************************************************/
void AdminSettingsMenu(void)
{
	counter_1s = 0;
	if((admin_password[0] == input_password[0]) && (admin_password[1] == input_password[1]) && (admin_password[2] == input_password[2])
	&& (admin_password[3] == input_password[3]) && (admin_password[4] == input_password[4]) && (admin_password[5] == input_password[5]))
	{
		ST7735_FillScreen(ST7735_BLACK);
		ST7735_DrawString(5, 0, "OPTIONS MENU", Font_11x18, ST7735_GREEN, ST7735_BLACK);
		ST7735_DrawString(0, 30, "A-FINGERPRINT DATABASE OPTIONS                                      B-PASSWORD OPTIONS                           C-ALARM OPTIONS                              D-RESET", Font_7x10, ST7735_GREEN, ST7735_BLACK);
		key = KEYPAD_NOT_PRESSED;
		while(KEYPAD_NOT_PRESSED == key)
		{
			key = Keypad4x4_Read();
			if(KEYPAD_NOT_PRESSED != key)
			{
				switch(key)
				{
					case 'A':
					{
						SystemState = SYS_STATE_ADM_DB_MENU;
						return;
					}
					case 'B':
					{
						SystemState = SYS_STATE_ADM_PWD_MENU;
						return;
					}
					case 'C':
					{
						SystemState = SYS_STATE_ADM_ALARM_MENU;
						return;
					}
					case 'D':
					{
						NVIC_SystemReset();  //reboot
					}
					case '#':
					{
						SystemState = SYS_STATE_BEGIN;
						ST7735_FillScreen(ST7735_BLACK);
						ST7735_DrawString(60, 50, "BYE!", Font_11x18, ST7735_GREEN, ST7735_BLACK);
						delay_ms(2000);
						WelcomeScreen();
						return;
					}
					default:
						AdminSettingsMenu();
				}
			}
		}
	}
	else
	{
		SystemState = SYS_STATE_BEGIN;
		input_password[0] = '\0';
		input_password[1] = '\0';
		input_password[2] = '\0';
		input_password[3] = '\0';
		BuzzerAlert(BUZZER_ALARM_SOUND);
		WelcomeScreen();
		return;
	}
}


/********************************************************************************************************/
/* @function name 		- Unlock																		*/
/*																										*/
/* @brief				- function to unlock the lock i.e relay with a magnetic lock connected to it for*/
/*						  10 seconds and then lock it bringing SystemState to initial state				*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void Unlock(void)
{
	//unlock relay
	GPIO_WritePin(GPIOD, GPIO_PIN_3, RESET);
	BuzzerAlert(BUZZER_ACCESS_GRANTED_SOUND);
	ST7735_FillScreen(ST7735_WHITE);
	ST7735_DrawString(0, 45, "10 SECONDS    LEFT TO PASS", Font_11x18, ST7735_BLUE, ST7735_WHITE);
	delay_ms(1000);
	ST7735_DrawString(0, 45, "9 SECONDS LEFT     TO PASS ", Font_11x18, ST7735_BLUE, ST7735_WHITE);
	delay_ms(1000);
	ST7735_DrawString(0, 45, "8 SECONDS LEFT     TO PASS ", Font_11x18, ST7735_BLUE, ST7735_WHITE);
	delay_ms(1000);
	ST7735_DrawString(0, 45, "7 SECONDS LEFT     TO PASS ", Font_11x18, ST7735_BLUE, ST7735_WHITE);
	delay_ms(1000);
	ST7735_DrawString(0, 45, "6 SECONDS LEFT     TO PASS ", Font_11x18, ST7735_BLUE, ST7735_WHITE);
	delay_ms(1000);
	ST7735_DrawString(0, 45, "5 SECONDS LEFT     TO PASS ", Font_11x18, ST7735_BLUE, ST7735_WHITE);
	delay_ms(1000);
	ST7735_DrawString(0, 45, "4 SECONDS LEFT     TO PASS ", Font_11x18, ST7735_BLUE, ST7735_WHITE);
	delay_ms(1000);
	ST7735_FillScreen(ST7735_CYAN);
	ST7735_DrawString(0, 45, "3 SECONDS LEFT   TO PASS!!", Font_11x18, ST7735_WHITE, ST7735_CYAN);
	delay_ms(500);
	ST7735_FillScreen(ST7735_WHITE);
	ST7735_DrawString(0, 45, "3 SECONDS LEFT   TO PASS!!", Font_11x18, ST7735_CYAN, ST7735_WHITE);
	delay_ms(500);
	ST7735_FillScreen(ST7735_GREEN);
	ST7735_DrawString(0, 45, "2 SECONDS LEFT   TO PASS!!", Font_11x18, ST7735_WHITE, ST7735_GREEN);
	delay_ms(500);
	ST7735_FillScreen(ST7735_WHITE);
	ST7735_DrawString(0, 45, "2 SECONDS LEFT   TO PASS!!", Font_11x18, ST7735_GREEN, ST7735_WHITE);
	delay_ms(500);
	ST7735_FillScreen(ST7735_MAGENTA);
	ST7735_DrawString(0, 45, "1 SECOND LEFT    TO PASS! ", Font_11x18, ST7735_WHITE, ST7735_MAGENTA);
	delay_ms(500);
	ST7735_FillScreen(ST7735_WHITE);
	ST7735_DrawString(0, 45, "1 SECONDS LEFT   TO PASS! ", Font_11x18, ST7735_MAGENTA, ST7735_WHITE);
	delay_ms(1000);
	ST7735_FillScreen(ST7735_WHITE);
	BuzzerAlert(BUZZER_ACCESS_GRANTED_SOUND);
	//lock relay
	GPIO_WritePin(GPIOD,GPIO_PIN_3, SET);
	WelcomeScreen();
	SystemState = SYS_STATE_BEGIN;
	input_password[0] = '\0';
	input_password[1] = '\0';
	input_password[2] = '\0';
	input_password[3] = '\0';
	return;
}


/********************************************************************************************************/
/* @function name 		- EmptyDB																		*/
/*																										*/
/* @brief				- this function calls a low level API to empty database and notify admin in case*/
/*						  of successful empty operation													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void EmptyDB(void)
{
	uint8_t empty_status = R308_Empty();
	if(empty_status == 0)
	{
		ST7735_FillScreen(ST7735_BLACK);
		ST7735_DrawString(35, 40, "DATABASE        EPMTIED", Font_11x18, ST7735_GREEN, ST7735_BLACK);
		delay_ms(1000);
		EnrollmentAttempt();
		DatabaseOperationsMenu();
	}else
	{
		ST7735_FillScreen(ST7735_BLACK);
		ST7735_DrawString(25, 50, "ERROR", Font_11x18, ST7735_RED, ST7735_BLACK);
		delay_ms(500);
		AdminSettingsMenu();
	}
}


/********************************************************************************************************/
/* @function name 		- Delete_ID																		*/
/*																										*/
/* @brief				- this function deletes the stored data of desired ID in R308 FLASH memory		*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void Delete_ID(void)
{
	int digit_number = 0;
	int save = 0;
	int16_t fingerprintID = 0;
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawString(0, 0, "PLEASE ENTER ID TO DELETE (PRESS 'D' TO SAVE)", Font_7x10, ST7735_GREEN, ST7735_BLACK);

	while ((digit_number < 3) && (SET != save))
	{
		key = KEYPAD_NOT_PRESSED;
		key = Keypad4x4_Read();

		if (KEYPAD_NOT_PRESSED != key)
		{
			if ((key >= '0' && key <= '9') || ('D' == key))
			{
				keypad_input[digit_number] = key;

				char temp_string[2];
				temp_string[0] = key;
				temp_string[1] = '\0';

				ST7735_DrawString(30 + digit_number * 27.5, 100, temp_string, Font_16x26, ST7735_GREEN, ST7735_BLACK);
				delay_ms(70);

				if (key >= '0' && key <= '9')
				{
					digit_number++;
				}
				else if ('D' == key)
				{
					save++;
					delay_ms(300);
				}
			}
			else if (('C' == key) && (digit_number > 0))
			{
				ST7735_DrawString(30 + digit_number * 27.5, 100, " ", Font_16x26, ST7735_GREEN, ST7735_BLACK);
				digit_number--;
			}
		}
	}

	if ((SET == save) && (digit_number > 0))
	{
		int16_t id_to_delete = 0;
		for (int i = 0; i < digit_number; i++)
		{
			id_to_delete = id_to_delete * 10 + (keypad_input[i] - '0');
		}

		fingerprintID = id_to_delete;

		R308_DeletChar(fingerprintID, 1);

		SystemState = SYS_STATE_ADM_SCREEN;
	}
}

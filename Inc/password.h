#ifndef PASSWORD_H_
#define PASSWORD_H_

#include <stm32f407xx.h>
#include <st7735.h>
#include <4x4_keypad.h>
#include <logo.h>
#include <r308.h>

typedef enum
{
	SYS_STATE_BEGIN,
	SYS_STATE_READ_PWD,
	SYS_STATE_PWD_CONFIRMATION,
	SYS_STATE_READ_ADMIN_PWD,
	SYS_STATE_ADM_SCREEN,
	SYS_STATE_ADM_PWD_MENU,
	SYS_STATE_ADM_DB_MENU,
	SYS_STATE_ADM_DB_EMPTY,
	SYS_STATE_ADM_DB_DEL_ID,
	SYS_STATE_ADM_NEW_USR_PWD,
	SYS_STATE_ADM_NEW_PWD,
	SYS_STATE_ADM_ALARM_MENU,
	SYS_STATE_ADM_ALARM_TIME
}SysState_t;


/*
 * @Buzzer_Sound
 */
typedef enum
{
	BUZZER_ALARM_SOUND,
	BUZZER_ACCESS_GRANTED_SOUND,
	BUZZER_ERROR_SOUND,
	BUZZER_INIT_SOUND
}Buzzer_Sound_t;


typedef enum
{
	ADMIN_ID = 0
}ID_t;

void MainMenu(void);
void SearchDatabase(void);
int16_t GetFreeIndex(void);
void EnrollFingerprint(int16_t fingerID);
void EnrollmentAttempt(void);


void BuzzerAlert(uint8_t sound);
void WelcomeScreen(void);
void EnterPassword(void);
void SecondStepAuth(uint16_t fingerID);
void ChangePasswordMenu(void);
void DatabaseOperationsMenu(void);
void EmptyDBWarning(void);
void ChangeAdminPassword(void);
void ChangeUserPassword(void);
void ChangeAlarmWarning(void);
void SetNewAlarmTime(void);
void TimeoutMessage(void);
void PasswordConfirmation(void);
void AdminSettingsMenu(void);
void Unlock(void);
void EmptyDB(void);
void Delete_ID(void);

#endif /* PASSWORD_H_ */

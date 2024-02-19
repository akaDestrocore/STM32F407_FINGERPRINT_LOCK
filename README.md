
# STM32F407 BASED SMART FINGERPRINT LOCK SYSTEM

This project is a fingerprint module based smart lock build over STM32F407. The system is designed for security and ease of use, allowing users to open the lock using their unique fingerprint and password. Some system parameters may be modified using administrator settings.


## System functions

- **Fingerprint Reading:** The system uses the R308 optical fingerprint module to scan fingerprints. The fingerprint is scanned and converted into a digital template, which is then stored in the module's memory.
- **Data Processing:** The STM32F407 microcontroller processes the data from the keypad input. It compares received key combination with the stored password.
- **Lock Control:** If the fingerprint matches one of the stored patterns and entered key combination matches the password, the microcontroller sends a signal to the lock control device, which opens the lock.
- **Alarm:** If either fingerprint or password is wrong, microcontoller generates alarm sound using buzzer over PWM modulation.
- **System settings:** Allows administrator to enter system settings and modify some fingerprint database and password options. Also lets them change the alarm duration and doing hard reset of the MCU.  
## How to use

A 4x4 keypad and TFT LCD is used to interact with the user in this project. At first launch system will check if R308 module is operating as intended by sending VfyPwd command packet(for details check [R308 optical fingerprint module user manual](https://cxem.net/guard/files/3-85_R308-fingerprint-module-user-manual.pdf)). If there are no issues with that then program flow will proceed to the initial state of system state machine.

![](https://github.com/akaDestrocore/STM32F407_FINGERPRINT_LOCK/blob/main/readme_gifs/gif_1.gif)

After this system asks user for a fingerprint. If fingerprint is found in optical module's FLASH memory then user will be promoted to enter user password. In case user completes both steps successfully the lock will be unlocked for 10 seconds.

![](https://github.com/akaDestrocore/STM32F407_FINGERPRINT_LOCK/blob/main/readme_gifs/gif_2.gif)

In case either fingerprint or password doesn't match those in database, buzzer will begin the alarm sound effect and corresponding message will be print on the screen.

![](https://github.com/akaDestrocore/STM32F407_FINGERPRINT_LOCK/blob/main/readme_gifs/gif_3.gif)


### Administartor panel setting

After scanning fingerprint in case its ID is marked as Administartor ID system will prompt to ask for administrator password. Administrator password consists of 6 characters instead of 4 and the timeout is also longer than regular user's. If the password matches system will proceed to Options menu.

![](https://github.com/akaDestrocore/STM32F407_FINGERPRINT_LOCK/blob/main/readme_gifs/gif_4.gif)

Here you can do some fingerprint operations(enrolling new fingerprint to free ID slot, deleting specific ID or emptying R308 database), change passwords or modify alarm duration.'#' key is used for exiting options menu. 

![](https://github.com/akaDestrocore/STM32F407_FINGERPRINT_LOCK/blob/main/readme_gifs/gif_5.gif)


## Related sources

1.[**RM0090 Reference manual**](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)

2.[**R308 optical fingerprint module user manual**](https://cxem.net/guard/files/3-85_R308-fingerprint-module-user-manual.pdf)




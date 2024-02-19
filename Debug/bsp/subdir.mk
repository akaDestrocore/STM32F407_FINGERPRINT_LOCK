################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/4x4_keypad.c \
../bsp/logo.c \
../bsp/r308.c \
../bsp/st7735.c \
../bsp/st7735_fonts.c 

OBJS += \
./bsp/4x4_keypad.o \
./bsp/logo.o \
./bsp/r308.o \
./bsp/st7735.o \
./bsp/st7735_fonts.o 

C_DEPS += \
./bsp/4x4_keypad.d \
./bsp/logo.d \
./bsp/r308.d \
./bsp/st7735.d \
./bsp/st7735_fonts.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/%.o bsp/%.su bsp/%.cyclo: ../bsp/%.c bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/Drivers/CMSIS" -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/Drivers/Inc" -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/Drivers/my libs" -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/Drivers/Src" -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mthumb -o "$@"

clean: clean-bsp

clean-bsp:
	-$(RM) ./bsp/4x4_keypad.cyclo ./bsp/4x4_keypad.d ./bsp/4x4_keypad.o ./bsp/4x4_keypad.su ./bsp/logo.cyclo ./bsp/logo.d ./bsp/logo.o ./bsp/logo.su ./bsp/r308.cyclo ./bsp/r308.d ./bsp/r308.o ./bsp/r308.su ./bsp/st7735.cyclo ./bsp/st7735.d ./bsp/st7735.o ./bsp/st7735.su ./bsp/st7735_fonts.cyclo ./bsp/st7735_fonts.d ./bsp/st7735_fonts.o ./bsp/st7735_fonts.su

.PHONY: clean-bsp


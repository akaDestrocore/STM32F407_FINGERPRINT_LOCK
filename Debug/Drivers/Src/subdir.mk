################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Src/stm32f407xx_flash.c \
../Drivers/Src/stm32f407xx_gpio.c \
../Drivers/Src/stm32f407xx_i2c.c \
../Drivers/Src/stm32f407xx_rcc.c \
../Drivers/Src/stm32f407xx_rtc.c \
../Drivers/Src/stm32f407xx_spi.c \
../Drivers/Src/stm32f407xx_system.c \
../Drivers/Src/stm32f407xx_tim.c \
../Drivers/Src/stm32f407xx_usart.c 

OBJS += \
./Drivers/Src/stm32f407xx_flash.o \
./Drivers/Src/stm32f407xx_gpio.o \
./Drivers/Src/stm32f407xx_i2c.o \
./Drivers/Src/stm32f407xx_rcc.o \
./Drivers/Src/stm32f407xx_rtc.o \
./Drivers/Src/stm32f407xx_spi.o \
./Drivers/Src/stm32f407xx_system.o \
./Drivers/Src/stm32f407xx_tim.o \
./Drivers/Src/stm32f407xx_usart.o 

C_DEPS += \
./Drivers/Src/stm32f407xx_flash.d \
./Drivers/Src/stm32f407xx_gpio.d \
./Drivers/Src/stm32f407xx_i2c.d \
./Drivers/Src/stm32f407xx_rcc.d \
./Drivers/Src/stm32f407xx_rtc.d \
./Drivers/Src/stm32f407xx_spi.d \
./Drivers/Src/stm32f407xx_system.d \
./Drivers/Src/stm32f407xx_tim.d \
./Drivers/Src/stm32f407xx_usart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Src/%.o Drivers/Src/%.su Drivers/Src/%.cyclo: ../Drivers/Src/%.c Drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/Drivers/CMSIS" -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/Drivers/Inc" -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/Drivers/my libs" -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/Drivers/Src" -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Src

clean-Drivers-2f-Src:
	-$(RM) ./Drivers/Src/stm32f407xx_flash.cyclo ./Drivers/Src/stm32f407xx_flash.d ./Drivers/Src/stm32f407xx_flash.o ./Drivers/Src/stm32f407xx_flash.su ./Drivers/Src/stm32f407xx_gpio.cyclo ./Drivers/Src/stm32f407xx_gpio.d ./Drivers/Src/stm32f407xx_gpio.o ./Drivers/Src/stm32f407xx_gpio.su ./Drivers/Src/stm32f407xx_i2c.cyclo ./Drivers/Src/stm32f407xx_i2c.d ./Drivers/Src/stm32f407xx_i2c.o ./Drivers/Src/stm32f407xx_i2c.su ./Drivers/Src/stm32f407xx_rcc.cyclo ./Drivers/Src/stm32f407xx_rcc.d ./Drivers/Src/stm32f407xx_rcc.o ./Drivers/Src/stm32f407xx_rcc.su ./Drivers/Src/stm32f407xx_rtc.cyclo ./Drivers/Src/stm32f407xx_rtc.d ./Drivers/Src/stm32f407xx_rtc.o ./Drivers/Src/stm32f407xx_rtc.su ./Drivers/Src/stm32f407xx_spi.cyclo ./Drivers/Src/stm32f407xx_spi.d ./Drivers/Src/stm32f407xx_spi.o ./Drivers/Src/stm32f407xx_spi.su ./Drivers/Src/stm32f407xx_system.cyclo ./Drivers/Src/stm32f407xx_system.d ./Drivers/Src/stm32f407xx_system.o ./Drivers/Src/stm32f407xx_system.su ./Drivers/Src/stm32f407xx_tim.cyclo ./Drivers/Src/stm32f407xx_tim.d ./Drivers/Src/stm32f407xx_tim.o ./Drivers/Src/stm32f407xx_tim.su ./Drivers/Src/stm32f407xx_usart.cyclo ./Drivers/Src/stm32f407xx_usart.d ./Drivers/Src/stm32f407xx_usart.o ./Drivers/Src/stm32f407xx_usart.su

.PHONY: clean-Drivers-2f-Src


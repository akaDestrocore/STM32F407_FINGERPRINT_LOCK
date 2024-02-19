################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/my\ libs/simple_delay.c 

OBJS += \
./Drivers/my\ libs/simple_delay.o 

C_DEPS += \
./Drivers/my\ libs/simple_delay.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/my\ libs/simple_delay.o: ../Drivers/my\ libs/simple_delay.c Drivers/my\ libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/Drivers/CMSIS" -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/Drivers/Inc" -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/Drivers/my libs" -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/Drivers/Src" -I"D:/Users/nabla/STM32CubeIDE/TI_like_workspace/FingerprintLock/bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Drivers/my libs/simple_delay.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-my-20-libs

clean-Drivers-2f-my-20-libs:
	-$(RM) ./Drivers/my\ libs/simple_delay.cyclo ./Drivers/my\ libs/simple_delay.d ./Drivers/my\ libs/simple_delay.o ./Drivers/my\ libs/simple_delay.su

.PHONY: clean-Drivers-2f-my-20-libs


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/l3gd20/Gyro_kit9.c \
../Drivers/l3gd20/l3gd20.c 

C_DEPS += \
./Drivers/l3gd20/Gyro_kit9.d \
./Drivers/l3gd20/l3gd20.d 

OBJS += \
./Drivers/l3gd20/Gyro_kit9.o \
./Drivers/l3gd20/l3gd20.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/l3gd20/%.o Drivers/l3gd20/%.su: ../Drivers/l3gd20/%.c Drivers/l3gd20/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Core/Inc -I../Drivers/l3gd20 -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-l3gd20

clean-Drivers-2f-l3gd20:
	-$(RM) ./Drivers/l3gd20/Gyro_kit9.d ./Drivers/l3gd20/Gyro_kit9.o ./Drivers/l3gd20/Gyro_kit9.su ./Drivers/l3gd20/l3gd20.d ./Drivers/l3gd20/l3gd20.o ./Drivers/l3gd20/l3gd20.su

.PHONY: clean-Drivers-2f-l3gd20


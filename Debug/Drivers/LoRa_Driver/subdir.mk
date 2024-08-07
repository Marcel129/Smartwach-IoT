################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LoRa_Driver/SX1278.c \
../Drivers/LoRa_Driver/SX1278_hw.c 

OBJS += \
./Drivers/LoRa_Driver/SX1278.o \
./Drivers/LoRa_Driver/SX1278_hw.o 

C_DEPS += \
./Drivers/LoRa_Driver/SX1278.d \
./Drivers/LoRa_Driver/SX1278_hw.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LoRa_Driver/%.o Drivers/LoRa_Driver/%.su: ../Drivers/LoRa_Driver/%.c Drivers/LoRa_Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/Marcel/STM32CubeIDE/workspace_1.11.2/lora_F103/Drivers/LoRa_Driver" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Marcel/STM32CubeIDE/workspace_1.11.2/lora_F103/Drivers/OLED_Drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-LoRa_Driver

clean-Drivers-2f-LoRa_Driver:
	-$(RM) ./Drivers/LoRa_Driver/SX1278.d ./Drivers/LoRa_Driver/SX1278.o ./Drivers/LoRa_Driver/SX1278.su ./Drivers/LoRa_Driver/SX1278_hw.d ./Drivers/LoRa_Driver/SX1278_hw.o ./Drivers/LoRa_Driver/SX1278_hw.su

.PHONY: clean-Drivers-2f-LoRa_Driver


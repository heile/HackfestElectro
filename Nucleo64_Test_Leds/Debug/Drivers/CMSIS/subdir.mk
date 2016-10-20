################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/system_stm32f0xx.c 

OBJS += \
./Drivers/CMSIS/system_stm32f0xx.o 

C_DEPS += \
./Drivers/CMSIS/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/system_stm32f0xx.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/system_stm32f0xx.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



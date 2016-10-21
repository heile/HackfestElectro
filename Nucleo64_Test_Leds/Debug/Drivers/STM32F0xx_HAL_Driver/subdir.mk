################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c \
/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c 

OBJS += \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal.o \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_cortex.o \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_dma.o \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_flash.o \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_flash_ex.o \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_gpio.o \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_i2c.o \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_i2c_ex.o \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_pwr.o \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_pwr_ex.o \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_rcc.o \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_rcc_ex.o \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_tim.o \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_tim_ex.o 

C_DEPS += \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal.d \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_cortex.d \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_dma.d \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_flash.d \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_flash_ex.d \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_gpio.d \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_i2c.d \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_i2c_ex.d \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_pwr.d \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_pwr_ex.d \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_rcc.d \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_rcc_ex.d \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_tim.d \
./Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_tim_ex.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_cortex.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_dma.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_flash.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_flash_ex.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_gpio.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_i2c.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_i2c_ex.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_pwr.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_pwr_ex.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_rcc.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_rcc_ex.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_tim.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F0xx_HAL_Driver/stm32f0xx_hal_tim_ex.o: /home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -I"/home/mdube/share/git/HackfestElectro/Nucleo64_Test_Leds/Inc" -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/mdube/STM32Cube/Repository/STM32Cube_FW_F0_V1.6.0/Drivers/CMSIS/Include  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



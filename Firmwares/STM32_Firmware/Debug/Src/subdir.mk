################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/bin.c \
../Src/main.c \
../Src/servo.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/timer.c \
../Src/uart.c \
../Src/ultrasonic.c 

OBJS += \
./Src/bin.o \
./Src/main.o \
./Src/servo.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/timer.o \
./Src/uart.o \
./Src/ultrasonic.o 

C_DEPS += \
./Src/bin.d \
./Src/main.d \
./Src/servo.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/timer.d \
./Src/uart.d \
./Src/ultrasonic.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F401RETx -c -I../Inc -I"/home/praveenrajrs/Desktop/IoT_Bin/Firmwares/STM32_Firmware/Third_Party/CMSIS/Include" -I"/home/praveenrajrs/Desktop/IoT_Bin/Firmwares/STM32_Firmware/Third_Party/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/bin.cyclo ./Src/bin.d ./Src/bin.o ./Src/bin.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/servo.cyclo ./Src/servo.d ./Src/servo.o ./Src/servo.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/timer.cyclo ./Src/timer.d ./Src/timer.o ./Src/timer.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su ./Src/ultrasonic.cyclo ./Src/ultrasonic.d ./Src/ultrasonic.o ./Src/ultrasonic.su

.PHONY: clean-Src


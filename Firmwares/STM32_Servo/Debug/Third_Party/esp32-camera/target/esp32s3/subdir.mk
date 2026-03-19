################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third_Party/esp32-camera/target/esp32s3/ll_cam.c 

OBJS += \
./Third_Party/esp32-camera/target/esp32s3/ll_cam.o 

C_DEPS += \
./Third_Party/esp32-camera/target/esp32s3/ll_cam.d 


# Each subdirectory must supply rules for building sources it contributes
Third_Party/esp32-camera/target/esp32s3/%.o Third_Party/esp32-camera/target/esp32s3/%.su Third_Party/esp32-camera/target/esp32s3/%.cyclo: ../Third_Party/esp32-camera/target/esp32s3/%.c Third_Party/esp32-camera/target/esp32s3/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F401RETx -c -I../Inc -I"/home/praveenrajrs/Desktop/IoT_Bin/Firmwares/STM32_Servo/Third_Party/CMSIS/Include" -I"/home/praveenrajrs/Desktop/IoT_Bin/Firmwares/STM32_Servo/Third_Party/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/praveenrajrs/Desktop/IoT_Bin/Firmwares/STM32_Servo/Third_Party/WS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Third_Party-2f-esp32-2d-camera-2f-target-2f-esp32s3

clean-Third_Party-2f-esp32-2d-camera-2f-target-2f-esp32s3:
	-$(RM) ./Third_Party/esp32-camera/target/esp32s3/ll_cam.cyclo ./Third_Party/esp32-camera/target/esp32s3/ll_cam.d ./Third_Party/esp32-camera/target/esp32s3/ll_cam.o ./Third_Party/esp32-camera/target/esp32s3/ll_cam.su

.PHONY: clean-Third_Party-2f-esp32-2d-camera-2f-target-2f-esp32s3


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third_Party/esp32-camera/driver/cam_hal.c \
../Third_Party/esp32-camera/driver/esp_camera.c \
../Third_Party/esp32-camera/driver/esp_camera_af.c \
../Third_Party/esp32-camera/driver/sccb-ng.c \
../Third_Party/esp32-camera/driver/sccb.c \
../Third_Party/esp32-camera/driver/sensor.c 

OBJS += \
./Third_Party/esp32-camera/driver/cam_hal.o \
./Third_Party/esp32-camera/driver/esp_camera.o \
./Third_Party/esp32-camera/driver/esp_camera_af.o \
./Third_Party/esp32-camera/driver/sccb-ng.o \
./Third_Party/esp32-camera/driver/sccb.o \
./Third_Party/esp32-camera/driver/sensor.o 

C_DEPS += \
./Third_Party/esp32-camera/driver/cam_hal.d \
./Third_Party/esp32-camera/driver/esp_camera.d \
./Third_Party/esp32-camera/driver/esp_camera_af.d \
./Third_Party/esp32-camera/driver/sccb-ng.d \
./Third_Party/esp32-camera/driver/sccb.d \
./Third_Party/esp32-camera/driver/sensor.d 


# Each subdirectory must supply rules for building sources it contributes
Third_Party/esp32-camera/driver/%.o Third_Party/esp32-camera/driver/%.su Third_Party/esp32-camera/driver/%.cyclo: ../Third_Party/esp32-camera/driver/%.c Third_Party/esp32-camera/driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F401RETx -c -I../Inc -I"/home/praveenrajrs/Desktop/IoT_Bin/Firmwares/STM32_Servo/Third_Party/CMSIS/Include" -I"/home/praveenrajrs/Desktop/IoT_Bin/Firmwares/STM32_Servo/Third_Party/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/praveenrajrs/Desktop/IoT_Bin/Firmwares/STM32_Servo/Third_Party/WS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Third_Party-2f-esp32-2d-camera-2f-driver

clean-Third_Party-2f-esp32-2d-camera-2f-driver:
	-$(RM) ./Third_Party/esp32-camera/driver/cam_hal.cyclo ./Third_Party/esp32-camera/driver/cam_hal.d ./Third_Party/esp32-camera/driver/cam_hal.o ./Third_Party/esp32-camera/driver/cam_hal.su ./Third_Party/esp32-camera/driver/esp_camera.cyclo ./Third_Party/esp32-camera/driver/esp_camera.d ./Third_Party/esp32-camera/driver/esp_camera.o ./Third_Party/esp32-camera/driver/esp_camera.su ./Third_Party/esp32-camera/driver/esp_camera_af.cyclo ./Third_Party/esp32-camera/driver/esp_camera_af.d ./Third_Party/esp32-camera/driver/esp_camera_af.o ./Third_Party/esp32-camera/driver/esp_camera_af.su ./Third_Party/esp32-camera/driver/sccb-ng.cyclo ./Third_Party/esp32-camera/driver/sccb-ng.d ./Third_Party/esp32-camera/driver/sccb-ng.o ./Third_Party/esp32-camera/driver/sccb-ng.su ./Third_Party/esp32-camera/driver/sccb.cyclo ./Third_Party/esp32-camera/driver/sccb.d ./Third_Party/esp32-camera/driver/sccb.o ./Third_Party/esp32-camera/driver/sccb.su ./Third_Party/esp32-camera/driver/sensor.cyclo ./Third_Party/esp32-camera/driver/sensor.d ./Third_Party/esp32-camera/driver/sensor.o ./Third_Party/esp32-camera/driver/sensor.su

.PHONY: clean-Third_Party-2f-esp32-2d-camera-2f-driver


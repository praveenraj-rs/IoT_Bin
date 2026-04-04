################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third_Party/esp32-camera/sensors/bf20a6.c \
../Third_Party/esp32-camera/sensors/bf3005.c \
../Third_Party/esp32-camera/sensors/gc0308.c \
../Third_Party/esp32-camera/sensors/gc032a.c \
../Third_Party/esp32-camera/sensors/gc2145.c \
../Third_Party/esp32-camera/sensors/hm0360.c \
../Third_Party/esp32-camera/sensors/hm1055.c \
../Third_Party/esp32-camera/sensors/mega_ccm.c \
../Third_Party/esp32-camera/sensors/nt99141.c \
../Third_Party/esp32-camera/sensors/ov2640.c \
../Third_Party/esp32-camera/sensors/ov3660.c \
../Third_Party/esp32-camera/sensors/ov5640.c \
../Third_Party/esp32-camera/sensors/ov5640_af.c \
../Third_Party/esp32-camera/sensors/ov7670.c \
../Third_Party/esp32-camera/sensors/ov7725.c \
../Third_Party/esp32-camera/sensors/sc030iot.c \
../Third_Party/esp32-camera/sensors/sc031gs.c \
../Third_Party/esp32-camera/sensors/sc101iot.c 

OBJS += \
./Third_Party/esp32-camera/sensors/bf20a6.o \
./Third_Party/esp32-camera/sensors/bf3005.o \
./Third_Party/esp32-camera/sensors/gc0308.o \
./Third_Party/esp32-camera/sensors/gc032a.o \
./Third_Party/esp32-camera/sensors/gc2145.o \
./Third_Party/esp32-camera/sensors/hm0360.o \
./Third_Party/esp32-camera/sensors/hm1055.o \
./Third_Party/esp32-camera/sensors/mega_ccm.o \
./Third_Party/esp32-camera/sensors/nt99141.o \
./Third_Party/esp32-camera/sensors/ov2640.o \
./Third_Party/esp32-camera/sensors/ov3660.o \
./Third_Party/esp32-camera/sensors/ov5640.o \
./Third_Party/esp32-camera/sensors/ov5640_af.o \
./Third_Party/esp32-camera/sensors/ov7670.o \
./Third_Party/esp32-camera/sensors/ov7725.o \
./Third_Party/esp32-camera/sensors/sc030iot.o \
./Third_Party/esp32-camera/sensors/sc031gs.o \
./Third_Party/esp32-camera/sensors/sc101iot.o 

C_DEPS += \
./Third_Party/esp32-camera/sensors/bf20a6.d \
./Third_Party/esp32-camera/sensors/bf3005.d \
./Third_Party/esp32-camera/sensors/gc0308.d \
./Third_Party/esp32-camera/sensors/gc032a.d \
./Third_Party/esp32-camera/sensors/gc2145.d \
./Third_Party/esp32-camera/sensors/hm0360.d \
./Third_Party/esp32-camera/sensors/hm1055.d \
./Third_Party/esp32-camera/sensors/mega_ccm.d \
./Third_Party/esp32-camera/sensors/nt99141.d \
./Third_Party/esp32-camera/sensors/ov2640.d \
./Third_Party/esp32-camera/sensors/ov3660.d \
./Third_Party/esp32-camera/sensors/ov5640.d \
./Third_Party/esp32-camera/sensors/ov5640_af.d \
./Third_Party/esp32-camera/sensors/ov7670.d \
./Third_Party/esp32-camera/sensors/ov7725.d \
./Third_Party/esp32-camera/sensors/sc030iot.d \
./Third_Party/esp32-camera/sensors/sc031gs.d \
./Third_Party/esp32-camera/sensors/sc101iot.d 


# Each subdirectory must supply rules for building sources it contributes
Third_Party/esp32-camera/sensors/%.o Third_Party/esp32-camera/sensors/%.su Third_Party/esp32-camera/sensors/%.cyclo: ../Third_Party/esp32-camera/sensors/%.c Third_Party/esp32-camera/sensors/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F401RETx -c -I../Inc -I"/home/praveenrajrs/Desktop/IoT_Bin/Firmwares/STM32_Servo/Third_Party/CMSIS/Include" -I"/home/praveenrajrs/Desktop/IoT_Bin/Firmwares/STM32_Servo/Third_Party/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/praveenrajrs/Desktop/IoT_Bin/Firmwares/STM32_Servo/Third_Party/WS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Third_Party-2f-esp32-2d-camera-2f-sensors

clean-Third_Party-2f-esp32-2d-camera-2f-sensors:
	-$(RM) ./Third_Party/esp32-camera/sensors/bf20a6.cyclo ./Third_Party/esp32-camera/sensors/bf20a6.d ./Third_Party/esp32-camera/sensors/bf20a6.o ./Third_Party/esp32-camera/sensors/bf20a6.su ./Third_Party/esp32-camera/sensors/bf3005.cyclo ./Third_Party/esp32-camera/sensors/bf3005.d ./Third_Party/esp32-camera/sensors/bf3005.o ./Third_Party/esp32-camera/sensors/bf3005.su ./Third_Party/esp32-camera/sensors/gc0308.cyclo ./Third_Party/esp32-camera/sensors/gc0308.d ./Third_Party/esp32-camera/sensors/gc0308.o ./Third_Party/esp32-camera/sensors/gc0308.su ./Third_Party/esp32-camera/sensors/gc032a.cyclo ./Third_Party/esp32-camera/sensors/gc032a.d ./Third_Party/esp32-camera/sensors/gc032a.o ./Third_Party/esp32-camera/sensors/gc032a.su ./Third_Party/esp32-camera/sensors/gc2145.cyclo ./Third_Party/esp32-camera/sensors/gc2145.d ./Third_Party/esp32-camera/sensors/gc2145.o ./Third_Party/esp32-camera/sensors/gc2145.su ./Third_Party/esp32-camera/sensors/hm0360.cyclo ./Third_Party/esp32-camera/sensors/hm0360.d ./Third_Party/esp32-camera/sensors/hm0360.o ./Third_Party/esp32-camera/sensors/hm0360.su ./Third_Party/esp32-camera/sensors/hm1055.cyclo ./Third_Party/esp32-camera/sensors/hm1055.d ./Third_Party/esp32-camera/sensors/hm1055.o ./Third_Party/esp32-camera/sensors/hm1055.su ./Third_Party/esp32-camera/sensors/mega_ccm.cyclo ./Third_Party/esp32-camera/sensors/mega_ccm.d ./Third_Party/esp32-camera/sensors/mega_ccm.o ./Third_Party/esp32-camera/sensors/mega_ccm.su ./Third_Party/esp32-camera/sensors/nt99141.cyclo ./Third_Party/esp32-camera/sensors/nt99141.d ./Third_Party/esp32-camera/sensors/nt99141.o ./Third_Party/esp32-camera/sensors/nt99141.su ./Third_Party/esp32-camera/sensors/ov2640.cyclo ./Third_Party/esp32-camera/sensors/ov2640.d ./Third_Party/esp32-camera/sensors/ov2640.o ./Third_Party/esp32-camera/sensors/ov2640.su ./Third_Party/esp32-camera/sensors/ov3660.cyclo ./Third_Party/esp32-camera/sensors/ov3660.d ./Third_Party/esp32-camera/sensors/ov3660.o ./Third_Party/esp32-camera/sensors/ov3660.su ./Third_Party/esp32-camera/sensors/ov5640.cyclo ./Third_Party/esp32-camera/sensors/ov5640.d ./Third_Party/esp32-camera/sensors/ov5640.o ./Third_Party/esp32-camera/sensors/ov5640.su ./Third_Party/esp32-camera/sensors/ov5640_af.cyclo ./Third_Party/esp32-camera/sensors/ov5640_af.d ./Third_Party/esp32-camera/sensors/ov5640_af.o ./Third_Party/esp32-camera/sensors/ov5640_af.su ./Third_Party/esp32-camera/sensors/ov7670.cyclo ./Third_Party/esp32-camera/sensors/ov7670.d ./Third_Party/esp32-camera/sensors/ov7670.o ./Third_Party/esp32-camera/sensors/ov7670.su ./Third_Party/esp32-camera/sensors/ov7725.cyclo ./Third_Party/esp32-camera/sensors/ov7725.d ./Third_Party/esp32-camera/sensors/ov7725.o ./Third_Party/esp32-camera/sensors/ov7725.su ./Third_Party/esp32-camera/sensors/sc030iot.cyclo ./Third_Party/esp32-camera/sensors/sc030iot.d ./Third_Party/esp32-camera/sensors/sc030iot.o ./Third_Party/esp32-camera/sensors/sc030iot.su ./Third_Party/esp32-camera/sensors/sc031gs.cyclo ./Third_Party/esp32-camera/sensors/sc031gs.d ./Third_Party/esp32-camera/sensors/sc031gs.o ./Third_Party/esp32-camera/sensors/sc031gs.su ./Third_Party/esp32-camera/sensors/sc101iot.cyclo ./Third_Party/esp32-camera/sensors/sc101iot.d ./Third_Party/esp32-camera/sensors/sc101iot.o ./Third_Party/esp32-camera/sensors/sc101iot.su

.PHONY: clean-Third_Party-2f-esp32-2d-camera-2f-sensors


// Ultrasonic Sensor Distance and Fill Level

#include <stm32f4xx.h>
#include <stdint.h>

#include "uart.h"
#include "timer.h"
#include "ultrasonic.h"

#define BIN_HEIGHT_CM 40

int main(void)
{
    TIM3_Delay_Init();
    UART2_Init();

    TIM2_Ultrasonic_Init();
    Ultrasonic_Init();

    int8_t distance = 0;
    uint8_t fill = 0;

    while (1)
    {
        distance = Ultrasonic_Read_cm(1);
        fill = Bin_GetFillPercent(distance, BIN_HEIGHT_CM);

        UART2_SendString("1 Distance: ");
        UART2_SendInt(distance);

        UART2_SendString(" cm | Fill: ");
        UART2_SendInt(fill);
        UART2_SendString(" %\n");


        distance = Ultrasonic_Read_cm(2);
        fill = Bin_GetFillPercent(distance, BIN_HEIGHT_CM);

        UART2_SendString("2 Distance: ");
        UART2_SendInt(distance);

        UART2_SendString(" cm | Fill: ");
        UART2_SendInt(fill);
        UART2_SendString(" %\n");

        distance = Ultrasonic_Read_cm(3);
        fill = Bin_GetFillPercent(distance, BIN_HEIGHT_CM);

        UART2_SendString("3 Distance: ");
        UART2_SendInt(distance);

        UART2_SendString(" cm | Fill: ");
        UART2_SendInt(fill);
        UART2_SendString(" %\n");

        UART2_SendString("\n");

        TIM3_Delay_ms(1000);
    }

}

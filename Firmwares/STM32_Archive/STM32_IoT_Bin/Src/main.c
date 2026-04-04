// --- PIN CONFIGURATION ---

// UART PINS

// UART_2 Tx -> PA2
// UART_2 Rx -> PA3

// SERVO PINS

// Servo1 -> TIM1_CH1 -> PA8
// Servo2 -> TIM1_CH2 -> PA9
// Servo3 -> TIM1_CH3 -> PA10

// ULTRASONIC PINS

// Trigger Pin -> PB12
// Echo Pins
// Ultrasonic_1 -> PB13
// Ultrasonic_2 -> PB14
// Ultrasonic_3 -> PB15

#include <stm32f4xx.h>
#include <string.h>
#include <stdio.h>

#include "config.h"
#include "timer.h"
#include "uart.h"
#include "servo.h"
#include "ultrasonic.h"


#define BIN_SERVO_ANGLE_OPEN 90		// 90 degree
#define BIN_SERVO_ANGLE_CLOSE 0		// 0 degree
#define BIN_SERVO_DELAY 5000 		// 10sec
#define BIN_HEIGHT_CM 50

#define BUFFER_SIZE 50
static char cmd[BUFFER_SIZE];
static uint8_t index_val = 0;

// --- UART COMMAND PARSER ---
uint8_t UART_ReadCommand(char **out_cmd)
{
    while (UART2_Available())
    {
        char c = UART2_Read();

        if (c == '\r' || c == '\n')
        {
            if (index_val > 0)
            {
                cmd[index_val] = '\0';
                *out_cmd = cmd;
                index_val = 0;
                return 1;
            }
        }
        else
        {
            if (index_val < BUFFER_SIZE - 1)
                cmd[index_val++] = c;
            else
            {
                index_val = 0;
                UART2_SendString("ERR\n");
            }
        }
    }
    return 0;
}

// --- SERVO ACTION ---
void Bin_Operate(uint8_t bin)
{
    uint8_t servo_channel = bin;

    // ACK
    char ack[10];
    sprintf(ack, "ACK%d\n", bin);
    UART2_SendString(ack);

    // OPEN (BIN_SERVO_ANGLE_CLOSE → BIN_SERVO_ANGLE_OPEN)
    for (int angle = BIN_SERVO_ANGLE_CLOSE; angle <= BIN_SERVO_ANGLE_OPEN; angle += 5)
    {
        Servo_SetAngle(servo_channel, angle);
        TIM3_Delay_ms(20);
    }

    // Wait BIN_SERVO_DELAY seconds
    TIM3_Delay_ms(BIN_SERVO_DELAY);

    // CLOSE (BIN_SERVO_ANGLE → BIN_SERVO_ANGLE_CLOSE)
    for (int angle = BIN_SERVO_ANGLE_OPEN; angle >= BIN_SERVO_ANGLE_CLOSE; angle -= 5)
    {
        Servo_SetAngle(servo_channel, angle);
        TIM3_Delay_ms(20);
    }

    // Measure fill level
    int8_t distance = Ultrasonic_Read_cm(bin);
    uint8_t fill = Bin_GetFillPercent(distance, BIN_HEIGHT_CM);

    // Send result
    char msg[20];
    sprintf(msg, "FILL%d:%d\n", bin, fill);
    UART2_SendString(msg);
}

// --- MAIN ---
int main(void)
{
    UART2_Init();
    TIM3_Delay_Init();

    Servo_TIM1_Init();
    TIM2_Ultrasonic_Init();
    Ultrasonic_Init();

    UART2_SendString("READY\n");

    char *received_cmd;

    while (1)
    {
        if (UART_ReadCommand(&received_cmd))
        {
            if (strcmp(received_cmd, "B1") == 0)
                Bin_Operate(1);

            else if (strcmp(received_cmd, "B2") == 0)
                Bin_Operate(2);

            else if (strcmp(received_cmd, "B3") == 0)
                Bin_Operate(3);

            else
                UART2_SendString("ERR\n");
        }
    }
}

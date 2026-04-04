#include <stm32f4xx.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "bin.h"
#include "timer.h"
#include "uart.h"
#include "servo.h"
#include "ultrasonic.h"

// --- BIN OPERATION ---

// BIN INTILISATION
void Bin_Init(void)
{
	UART1_Init();
	TIM3_Delay_Init();

	Servo_TIM1_Init();
	Ultrasonic_Init();
	TIM2_Ultrasonic_Init();
}

// --- USER DISTANCE ---
long UserToBin_Distance(void)
{
    long total = 0;

    for (uint8_t i = 0; i < ULTRASONIC_SAMPLES; i++)
    {
        total += Ultrasonic_Read_cm(0);
        TIM3_Delay_ms(50);
    }

    return total / ULTRASONIC_SAMPLES;
}

// --- PROCESS COMMAND ---
void Process_Command(char *cmd)
{
    // DEBUG
//    UART1_SendString("RX:[");
//    UART1_SendString(cmd);
//    UART1_SendString("]\n");

    if      (strncmp(cmd, "B1", 2) == 0) Bin_Operate(1);
    else if (strncmp(cmd, "B2", 2) == 0) Bin_Operate(2);
    else if (strncmp(cmd, "B3", 2) == 0) Bin_Operate(3);
    else    UART1_SendString(MSG_ERR);
}

// BIN OPERATE
void Bin_Operate(uint8_t bin)
{
    char msg[32];

    snprintf(msg, sizeof(msg), "ACK%d\n", bin);
    UART1_SendString(msg);

    Servo_Open(bin);
    TIM3_Delay_ms(BIN_SERVO_DELAY_MS);
    Servo_Close(bin);

    int8_t dist = Ultrasonic_Read_cm(bin);
    uint8_t fill = Bin_GetFillPercent(dist, BIN_HEIGHT_CM);

    snprintf(msg, sizeof(msg), "FILL%d:%d\n", bin, fill);
    UART1_SendString(msg);
}



// --- SERVO CONTROL ---
void Servo_Open(uint8_t channel)
{
    for (int a = BIN_SERVO_ANGLE_CLOSE; a <= BIN_SERVO_ANGLE_OPEN; a += 5)
    {
        Servo_SetAngle(channel, a);
        TIM3_Delay_ms(20);
    }
}

void Servo_Close(uint8_t channel)
{
    for (int a = BIN_SERVO_ANGLE_OPEN; a >= BIN_SERVO_ANGLE_CLOSE; a -= 5)
    {
        Servo_SetAngle(channel, a);
        TIM3_Delay_ms(20);
    }
}

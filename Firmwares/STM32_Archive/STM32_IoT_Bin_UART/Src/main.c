// --- PIN CONFIGURATION ---

// PIR       -> PA0

// UART_1 Tx -> PB6  (ESP32-CAM comms)
// UART_1 Rx -> PB7

// Servo1    -> TIM1_CH1 -> PA8
// Servo2    -> TIM1_CH2 -> PA9
// Servo3    -> TIM1_CH3 -> PA10

// Trigger   -> PB8
// Ultrasonic_Mov -> PB12
// Ultrasonic_1   -> PB13
// Ultrasonic_2   -> PB14
// Ultrasonic_3   -> PB15

#include <stm32f4xx.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "config.h"
#include "timer.h"
#include "uart.h"
#include "servo.h"
#include "ultrasonic.h"

// --- CONSTANTS ---
#define BIN_SERVO_ANGLE_OPEN    90
#define BIN_SERVO_ANGLE_CLOSE   0
#define BIN_SERVO_DELAY_MS      5000
#define BIN_HEIGHT_CM           50

#define USER_DISTANCE_THRESHOLD 25
#define ULTRASONIC_SAMPLES      5
#define BUFFER_SIZE             64

#define MSG_PIR_DETECT  "PIR_DETECT\n"
#define MSG_READY       "READY\n"
#define MSG_ERR         "ERR\n"

// --- GLOBALS ---
volatile uint8_t pir_motion = 0;

char rx_buf[BUFFER_SIZE];
static uint8_t rx_index = 0;

volatile uint8_t ESP32_RES_FLAG = 1;

// --- PIR INTERRUPT ---
void PIR_EXTI0_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    GPIOA->MODER &= ~(3U << (0 * 2));
    GPIOA->PUPDR &= ~(3U << (0 * 2));
    GPIOA->PUPDR |=  (2U << (0 * 2));

    SYSCFG->EXTICR[0] &= ~(0xF << 0);

    EXTI->IMR  |= (1U << 0);
    EXTI->RTSR |= (1U << 0);

    NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & (1U << 0))
    {
        EXTI->PR |= (1U << 0);
        pir_motion = 1;
    }
}

// --- SERVO CONTROL ---
void Servo_Open(uint8_t channel)
{
    for (int a = 0; a <= 90; a += 5)
    {
        Servo_SetAngle(channel, a);
        TIM3_Delay_ms(20);
    }
}

void Servo_Close(uint8_t channel)
{
    for (int a = 90; a >= 0; a -= 5)
    {
        Servo_SetAngle(channel, a);
        TIM3_Delay_ms(20);
    }
}

// --- UART1 READER ---
uint8_t UART1_ReadCommand(char **out_cmd)
{
	//rx_buf[0] = '\0';
    while (UART1_Available())
    {
        char c = UART1_Read();
       // UART1_SendChar(c);

        if (c == '\r' || c == '\n')
        {
            if (rx_index > 0)
            {
                rx_buf[rx_index] = '\0';
                *out_cmd = rx_buf;
                rx_index = 0;
                return 1;
            }
        }
        else
        {
            if (rx_index < BUFFER_SIZE - 1)
                rx_buf[rx_index++] = c;
            else
            {
                rx_index = 0;
                UART1_SendString(MSG_ERR);
            }
        }
    }
    return 0;
}

// --- USER DISTANCE ---
static long Measure_UserDistance(void)
{
    long total = 0;

    for (uint8_t i = 0; i < ULTRASONIC_SAMPLES; i++)
    {
        total += Ultrasonic_Read_cm(0);
        TIM3_Delay_ms(50);
    }

    return total / ULTRASONIC_SAMPLES;
}

// --- BIN OPERATION ---
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

// --- PROCESS COMMAND ---
void Process_Command(char *cmd)
{
    trim_string(cmd);

    // DEBUG
//    UART1_SendString("RX:[");
//    UART1_SendString(cmd);
//    UART1_SendString("]\n");

    if      (strncmp(cmd, "B1", 2) == 0) Bin_Operate(1);
    else if (strncmp(cmd, "B2", 2) == 0) Bin_Operate(2);
    else if (strncmp(cmd, "B3", 2) == 0) Bin_Operate(3);
    else    UART1_SendString(MSG_ERR);
}

// --- MAIN ---
int main(void)
{
    PIR_EXTI0_Init();

    UART1_Init();
    TIM3_Delay_Init();

    Servo_TIM1_Init();
    Ultrasonic_Init();
    TIM2_Ultrasonic_Init();

    UART1_SendString(MSG_READY);

    char *cmd;

    while (1)
    {
        // READ UART
    	if (!ESP32_RES_FLAG)
    	{
    	    if (UART1_ReadCommand(&cmd))
    	    {
    	        Process_Command(cmd);
    	        ESP32_RES_FLAG = 1;
    	    }
    	}

        // PIR FLOW
        if (pir_motion && ESP32_RES_FLAG)
        {
            pir_motion = 0;

            long dist = Measure_UserDistance();

            if (dist <= USER_DISTANCE_THRESHOLD)
            {
                UART1_SendString(MSG_PIR_DETECT);
                ESP32_RES_FLAG = 0;
            }
        }
    }
}

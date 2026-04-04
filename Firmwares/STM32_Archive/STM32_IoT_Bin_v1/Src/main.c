// --- PIN CONFIGURATION ---

// UART_2 Tx -> PA2  (debug / ESP32-CAM comms)
// UART_2 Rx -> PA3

// Servo1    -> TIM1_CH1 -> PA8
// Servo2    -> TIM1_CH2 -> PA9
// Servo3    -> TIM1_CH3 -> PA10

// Trigger   -> PB8
// Ultrasonic_Mov -> PB12
// Ultrasonic_1   -> PB13
// Ultrasonic_2   -> PB14
// Ultrasonic_3   -> PB15

// PIR       -> PA0

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
#define BIN_SERVO_DELAY_MS      5000 // 5sec
#define BIN_HEIGHT_CM           50

#define USER_DISTANCE_THRESHOLD 25
#define ULTRASONIC_SAMPLES      5
#define BUFFER_SIZE             50

// ESP32-CAM response timeout in milliseconds
// Camera needs time to capture, run inference, send reply
#define ESP_RESPONSE_TIMEOUT_MS 10000

// --- PROTOCOL TOKENS (sent TO ESP32-CAM) ---
#define MSG_PIR_DETECT  "PIR_DETECT\n"
#define MSG_READY       "READY\n"
#define MSG_ERR         "ERR\n"

// --- GLOBAL FLAGS ---
volatile uint8_t pir_motion = 0;

static char rx_buf[BUFFER_SIZE];
static uint8_t rx_index = 0;

// ---------------------------------------------------------------
// PIR INTERRUPT INIT  (PA0, EXTI0, rising edge)
// ---------------------------------------------------------------
void PIR_EXTI0_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // PA0 as input with pull-down (PIR idle = LOW)
    GPIOA->MODER &= ~(3U << (0 * 2));
    GPIOA->PUPDR &= ~(3U << (0 * 2));
    GPIOA->PUPDR |=  (2U << (0 * 2));

    SYSCFG->EXTICR[0] &= ~(0xF << 0);  // EXTI0 → PA0
    EXTI->IMR  |=  (1U << 0);          // unmask
    EXTI->RTSR |=  (1U << 0);          // rising edge only
    EXTI->FTSR &= ~(1U << 0);

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

// ---------------------------------------------------------------
// UART COMMAND READER
// Returns 1 and fills *out_cmd when a full \n-terminated line
// arrives.  Non-blocking — call repeatedly from the main loop.
// ---------------------------------------------------------------
uint8_t UART_ReadCommand(char **out_cmd)
{
    while (UART2_Available())
    {
        char c = UART2_Read();

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
                rx_index = 0;          // overflow — discard
                UART2_SendString(MSG_ERR);
            }
        }
    }
    return 0;
}

// ---------------------------------------------------------------
// BLOCKING WAIT FOR ESP32-CAM RESPONSE
// Waits up to `timeout_ms` for a complete command line.
// Returns 1 on success; 0 on timeout.
// Uses TIM3 tick to avoid hanging the MCU indefinitely.
// ---------------------------------------------------------------
uint8_t ESP_WaitResponse(char **out_cmd, uint32_t timeout_ms)
{
    uint32_t elapsed = 0;

    while (elapsed < timeout_ms)
    {
        if (UART_ReadCommand(out_cmd))
            return 1;

        TIM3_Delay_ms(10);
        elapsed += 10;
    }

    return 0;   // timed out
}

// ---------------------------------------------------------------
// MEASURE USER DISTANCE  (averaged over N samples)
// Returns distance in cm.
// ---------------------------------------------------------------
static long Measure_UserDistance(void)
{
    long total = 0;
    for (uint8_t i = 0; i < ULTRASONIC_SAMPLES; i++)
    {
        total += Ultrasonic_Read_cm(0);   // channel 0 = Ultrasonic_Mov
        TIM3_Delay_ms(50);
    }
    return total / ULTRASONIC_SAMPLES;
}

// ---------------------------------------------------------------
// SERVO SWEEP  (non-blocking step approach for smooth motion)
// ---------------------------------------------------------------
static void Servo_Open(uint8_t channel)
{
    for (int angle = BIN_SERVO_ANGLE_CLOSE; angle <= BIN_SERVO_ANGLE_OPEN; angle += 5)
    {
        Servo_SetAngle(channel, angle);
        TIM3_Delay_ms(20);
    }
}

static void Servo_Close(uint8_t channel)
{
    for (int angle = BIN_SERVO_ANGLE_OPEN; angle >= BIN_SERVO_ANGLE_CLOSE; angle -= 5)
    {
        Servo_SetAngle(channel, angle);
        TIM3_Delay_ms(20);
    }
}

// ---------------------------------------------------------------
// BIN OPERATION
// Called when STM32 already knows which bin to open (1-3).
//
//  1. Send ACK to ESP32-CAM
//  2. Open servo
//  3. Wait BIN_SERVO_DELAY_MS
//  4. Close servo
//  5. Measure fill level
//  6. Send FILL result to ESP32-CAM (it will HTTP-POST upstream)
// ---------------------------------------------------------------
void Bin_Operate(uint8_t bin)
{
    char msg[32];

    // --- ACK ---
    snprintf(msg, sizeof(msg), "ACK%d\n", bin);
    UART2_SendString(msg);

    // --- OPEN ---
    Servo_Open(bin);

    // --- HOLD ---
    TIM3_Delay_ms(BIN_SERVO_DELAY_MS);

    // --- CLOSE ---
    Servo_Close(bin);

    // --- MEASURE ---
    int8_t  distance_cm = Ultrasonic_Read_cm(bin);
    uint8_t fill_pct    = Bin_GetFillPercent(distance_cm, BIN_HEIGHT_CM);

    // --- REPORT to ESP32-CAM ---
    snprintf(msg, sizeof(msg), "FILL%d:%d\n", bin, fill_pct);
    UART2_SendString(msg);
}

// ---------------------------------------------------------------
// MAIN
// ---------------------------------------------------------------
int main(void)
{
    PIR_EXTI0_Init();

    UART2_Init();
    TIM3_Delay_Init();

    Servo_TIM1_Init();
    TIM2_Ultrasonic_Init();
    Ultrasonic_Init();

    UART2_SendString(MSG_READY);

    char *received_cmd = NULL;

    while (1)
    {
        // =======================================================
        // PATH A: PIR triggered — confirm with ultrasonic,
        //         then notify ESP32-CAM and wait for bin decision
        // =======================================================
        if (pir_motion)
        {
            pir_motion = 0;   // clear flag immediately (re-entrant safe)

            long user_dist = Measure_UserDistance();

            // Optional: forward distance for diagnostics
            char dbg[32];
            snprintf(dbg, sizeof(dbg), "DIST:%ld\n", user_dist);
            UART2_SendString(dbg);

            if (user_dist <= USER_DISTANCE_THRESHOLD)
            {
                // Tell ESP32-CAM: a user is present, take a photo
                UART2_SendString(MSG_PIR_DETECT);

                // Wait for ESP32-CAM to classify and respond
                if (ESP_WaitResponse(&received_cmd, ESP_RESPONSE_TIMEOUT_MS))
                {
                    if      (strcmp(received_cmd, "B1") == 0) Bin_Operate(1);
                    else if (strcmp(received_cmd, "B2") == 0) Bin_Operate(2);
                    else if (strcmp(received_cmd, "B3") == 0) Bin_Operate(3);
                    else    UART2_SendString(MSG_ERR);
                }
                else
                {
                    // ESP32-CAM did not respond in time
                    UART2_SendString("TIMEOUT\n");
                }
            }
        }

        // =======================================================
        // PATH B: Manual override from ESP32-CAM (or host system)
        //         B1 / B2 / B3 sent directly without PIR trigger.
        //         Useful for remote open commands.
        // =======================================================
        if (UART_ReadCommand(&received_cmd))
        {
            if      (strcmp(received_cmd, "B1") == 0) Bin_Operate(1);
            else if (strcmp(received_cmd, "B2") == 0) Bin_Operate(2);
            else if (strcmp(received_cmd, "B3") == 0) Bin_Operate(3);
            else    UART2_SendString(MSG_ERR);
        }
    }
}

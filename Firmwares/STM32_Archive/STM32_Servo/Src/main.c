// Servo Control with Timer1 Channels

// Servo1 -> TIM1_CH1 -> PA8
// Servo2 -> TIM1_CH2 -> PA9
// Servo3 -> TIM1_CH3 -> PA10

#include <stm32f4xx.h>
#include <stdint.h>

#include "config.h"
#include "timer.h"

#define SERVO_MIN 544
#define SERVO_MAX 2400

/* ================= SERVO INIT ================= */
void Servo_TIM1_Init(void)
{
    /* Enable Clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;   // GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;    // TIM1 (APB2!)

    /* ================= GPIO CONFIG ================= */

    // PA8, PA9, PA10 → AF1 (TIM1)
    GPIOA->MODER &= ~((3U<<(8*2)) | (3U<<(9*2)) | (3U<<(10*2)));
    GPIOA->MODER |=  ((2U<<(8*2)) | (2U<<(9*2)) | (2U<<(10*2)));

    GPIOA->AFR[1] &= ~((0xF<<(0*4)) | (0xF<<(1*4)) | (0xF<<(2*4)));
    GPIOA->AFR[1] |=  ((1U<<(0*4)) | (1U<<(1*4)) | (1U<<(2*4))); // AF1

    /* ================= TIMER CONFIG ================= */

    TIM1->PSC = (SysClk / 1000000) - 1;   // 1 MHz
    TIM1->ARR = 20000 - 1;                // 20 ms (50 Hz)

    /* ================= PWM MODE ================= */

    // CH1
    TIM1->CCMR1 &= ~(7U << 4);
    TIM1->CCMR1 |= (6U << 4);
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE;

    // CH2
    TIM1->CCMR1 &= ~(7U << 12);
    TIM1->CCMR1 |= (6U << 12);
    TIM1->CCMR1 |= TIM_CCMR1_OC2PE;

    // CH3
    TIM1->CCMR2 &= ~(7U << 4);
    TIM1->CCMR2 |= (6U << 4);
    TIM1->CCMR2 |= TIM_CCMR2_OC3PE;

    /* Enable Channels */
    TIM1->CCER |= TIM_CCER_CC1E;
    TIM1->CCER |= TIM_CCER_CC2E;
    TIM1->CCER |= TIM_CCER_CC3E;

    /* Default 90° */
    TIM1->CCR1 = 1500;
    TIM1->CCR2 = 1500;
    TIM1->CCR3 = 1500;

    /* IMPORTANT for TIM1 */
    TIM1->BDTR |= TIM_BDTR_MOE;   // Main Output Enable

    /* Start Timer */
    TIM1->CR1 |= TIM_CR1_ARPE;
    TIM1->EGR |= TIM_EGR_UG;
    TIM1->CR1 |= TIM_CR1_CEN;
}

/* ================= SET ANGLE ================= */

void Servo_SetAngle(uint8_t channel, uint8_t angle)
{
    if (angle > 180) angle = 180;

    uint32_t pulse = SERVO_MIN + ((SERVO_MAX - SERVO_MIN) * angle) / 180;

    switch(channel)
    {
        case 1: TIM1->CCR1 = pulse; break;
        case 2: TIM1->CCR2 = pulse; break;
        case 3: TIM1->CCR3 = pulse; break;
        default: break;
    }
}

/* ================= MAIN ================= */

int main(void)
{
    TIM3_Delay_Init();
    Servo_TIM1_Init();

    while (1)
    {
        // Sweep all 3 servos together
        for (int angle = 0; angle <= 180; angle += 30)
        {
            Servo_SetAngle(1, angle);
            Servo_SetAngle(2, angle);
            Servo_SetAngle(3, angle);

            TIM3_Delay_ms(100);
        }

        TIM3_Delay_ms(1000);

        for (int angle = 180; angle >= 0; angle -= 30)
        {
            Servo_SetAngle(1, angle);
            Servo_SetAngle(2, angle);
            Servo_SetAngle(3, angle);

            TIM3_Delay_ms(100);
        }

        TIM3_Delay_ms(1000);
    }
}

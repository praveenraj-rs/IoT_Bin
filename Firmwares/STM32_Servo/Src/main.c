#include "stm32f4xx.h"
#include <stdint.h>
#include "config.h"
#include "timer.h"

#define SERVO_MIN 544
#define SERVO_MAX 2400

/* ================= INIT ================= */
/* ================= SERVO INIT ================= */
void Servo_TIM2_Init(void)
{
    /* Enable Clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;   // 🔥 GPIOB enabled
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    /* ================= GPIO CONFIG ================= */

    /* PA0, PA1 → TIM2_CH1, CH2 */
    GPIOA->MODER &= ~((3U<<(0*2)) | (3U<<(1*2)));
    GPIOA->MODER |=  ((2U<<(0*2)) | (2U<<(1*2)));

    GPIOA->AFR[0] &= ~((0xF<<(0*4)) | (0xF<<(1*4)));
    GPIOA->AFR[0] |=  ((1U<<(0*4)) | (1U<<(1*4)));   // AF1

    /* PB10 → TIM2_CH3 */
    GPIOB->MODER &= ~(3U << (10 * 2));
    GPIOB->MODER |=  (2U << (10 * 2));              // AF mode

    GPIOB->AFR[1] &= ~(0xF << ((10 - 8) * 4));
    GPIOB->AFR[1] |=  (1U << ((10 - 8) * 4));       // AF1 (TIM2)

    /* ================= TIMER CONFIG ================= */

    TIM2->PSC = (SysClk / 1000000) - 1;   // 1 MHz
    TIM2->ARR = 20000 - 1;                // 20 ms (50 Hz)

    /* ================= PWM MODE ================= */

    // CH1
    TIM2->CCMR1 &= ~(7U << 4);
    TIM2->CCMR1 |= (6U << 4);
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;

    // CH2
    TIM2->CCMR1 &= ~(7U << 12);
    TIM2->CCMR1 |= (6U << 12);
    TIM2->CCMR1 |= TIM_CCMR1_OC2PE;

    // CH3
    TIM2->CCMR2 &= ~(7U << 4);
    TIM2->CCMR2 |= (6U << 4);
    TIM2->CCMR2 |= TIM_CCMR2_OC3PE;

    /* Enable Channels */
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CCER |= TIM_CCER_CC2E;
    TIM2->CCER |= TIM_CCER_CC3E;

    /* Default 90° position */
    TIM2->CCR1 = 1500;
    TIM2->CCR2 = 1500;
    TIM2->CCR3 = 1500;

    /* Start Timer */
    TIM2->CR1 |= TIM_CR1_ARPE;
    TIM2->EGR |= TIM_EGR_UG;   // 🔥 Important
    TIM2->CR1 |= TIM_CR1_CEN;
}

/* ================= SET ANGLE ================= */

void Servo_SetAngle(uint8_t channel, uint8_t angle)
{
    if (angle > 180) angle = 180;

    uint32_t pulse = SERVO_MIN + ((SERVO_MAX - SERVO_MIN) * angle) / 180;

    switch(channel)
    {
        case 1: TIM2->CCR1 = pulse; break;
        case 2: TIM2->CCR2 = pulse; break;
        case 3: TIM2->CCR3 = pulse; break;
        default: break;
    }
}

/* ================= MAIN ================= */

int main(void)
{
    TIM3_Delay_Init();
    Servo_TIM2_Init();

    while (1)
    {
        // Sweep all 3 servos together
        for (int angle = 0; angle <= 180; angle += 30)
        {
            Servo_SetAngle(1, angle);
            Servo_SetAngle(2, angle);
            Servo_SetAngle(3, angle);

            TIM3_Delay_ms(200);
        }

        for (int angle = 180; angle >= 0; angle -= 30)
        {
            Servo_SetAngle(1, angle);
            Servo_SetAngle(2, angle);
            Servo_SetAngle(3, angle);

            TIM3_Delay_ms(200);
        }
    }
}

// Servo Control with Timer1 Channels

// Servo1 -> TIM1_CH1 -> PA8
// Servo2 -> TIM1_CH2 -> PA9
// Servo3 -> TIM1_CH3 -> PA10

#include "config.h"
#include "servo.h"

// Servo PWM 50Hz (20ms)

#define SERVO_MIN 544	// 0° -> 0.544 ms
#define SERVO_MAX 2400	// 180° -> 2.4 ms

// --- SERVO INIT ---
void Servo_TIM1_Init(void)
{
    // Enable Clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;   // GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;    // TIM1 (APB2!)

    // GPIO Config
    // PA8, PA9, PA10 → AF1 (TIM1)
    GPIOA->MODER &= ~((3U<<(8*2)) | (3U<<(9*2)) | (3U<<(10*2)));
    GPIOA->MODER |=  ((2U<<(8*2)) | (2U<<(9*2)) | (2U<<(10*2)));

    GPIOA->AFR[1] &= ~((0xF<<(0*4)) | (0xF<<(1*4)) | (0xF<<(2*4)));
    GPIOA->AFR[1] |=  ((1U<<(0*4)) | (1U<<(1*4)) | (1U<<(2*4))); // AF1

    // Timer Config
    TIM1->PSC = (SysClk / 1000000) - 1;   // 1 MHz
    TIM1->ARR = 20000 - 1;                // 20 ms (50 Hz)

    // PWM Mode
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

    // Enable Channels
    TIM1->CCER |= TIM_CCER_CC1E;
    TIM1->CCER |= TIM_CCER_CC2E;
    TIM1->CCER |= TIM_CCER_CC3E;

    // Default 0°
    TIM1->CCR1 = 544;
    TIM1->CCR2 = 544;
    TIM1->CCR3 = 544;

    // TIM1 Main Output Enable
    TIM1->BDTR |= TIM_BDTR_MOE;   // MOE

    // Start Timer
    TIM1->CR1 |= TIM_CR1_ARPE;
    TIM1->EGR |= TIM_EGR_UG;
    TIM1->CR1 |= TIM_CR1_CEN;
}

// --- SET ANGLE ---
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

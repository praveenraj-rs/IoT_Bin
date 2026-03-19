#include "ultrasonic.h"

/* ================= CONFIG ================= */

#define SysClk 16000000U

#define TRIG_PIN     12 //PB12
#define ECHO_PIN_B1  13 //PB13
#define ECHO_PIN_B2  14 //PB14
#define ECHO_PIN_B3  15 //PB15

/* ================= TIMER INIT ================= */

void TIM2_Ultrasonic_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = (SysClk / 1000000) - 1; // 1 MHz → 1 µs
    TIM2->ARR = 0xFFFFFFFF;
    TIM2->CNT = 0;

    TIM2->CR1 |= TIM_CR1_CEN;
}

/* ================= DELAY ================= */

static void TIM2_Delay_us(uint32_t us)
{
    TIM2->CNT = 0;
    while (TIM2->CNT < us);
}

/* ================= GPIO INIT ================= */

void Ultrasonic_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // TRIG → Output
    GPIOB->MODER &= ~(3U << (TRIG_PIN * 2));
    GPIOB->MODER |=  (1U << (TRIG_PIN * 2));

    // ECHO → Input + Pull-down
    uint8_t echo_pins[] = {ECHO_PIN_B1, ECHO_PIN_B2, ECHO_PIN_B3};

    for (int i = 0; i < 3; i++)
    {
        GPIOB->MODER &= ~(3U << (echo_pins[i] * 2));
        GPIOB->PUPDR &= ~(3U << (echo_pins[i] * 2));
        GPIOB->PUPDR |=  (2U << (echo_pins[i] * 2));
    }
}

/* ================= READ CM ================= */

uint32_t Ultrasonic_Read_cm(uint8_t bin)
{
    uint32_t timeout = 30000;
    uint16_t echo_pin;

    if (bin == 1)      echo_pin = ECHO_PIN_B1;
    else if (bin == 2) echo_pin = ECHO_PIN_B2;
    else               echo_pin = ECHO_PIN_B3;

    /* Trigger */
    GPIOB->BSRR = (1U << (TRIG_PIN + 16));
    TIM2_Delay_us(2);

    GPIOB->BSRR = (1U << TRIG_PIN);
    TIM2_Delay_us(10);

    GPIOB->BSRR = (1U << (TRIG_PIN + 16));

    /* Wait rising */
    while (!(GPIOB->IDR & (1U << echo_pin)))
    {
        if (--timeout == 0) return 0;
    }

    /* Measure */
    TIM2->CNT = 0;

    while (GPIOB->IDR & (1U << echo_pin))
    {
        if (TIM2->CNT > 40000) break;
    }

    uint32_t echo_time = TIM2->CNT;

    if (echo_time == 0) return 0;

    return echo_time / ULTRASONIC_DIVISOR;
}

/* ================= FILL ================= */

uint8_t Bin_GetFillPercent(uint32_t distance, uint32_t bin_height)
{
    if (distance == 0 || distance > bin_height)
        return 0;

    uint32_t filled = bin_height - distance;

    return (filled * 100) / bin_height;
}

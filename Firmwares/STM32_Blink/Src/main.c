// Blink LED (User LED - Nucleo STM32F401RE)

#include <stm32f4xx.h>

#include "config.h"
#include "timer.h"

#define LED 5U               // PA5 User LED

void LED_GPIO_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;   // Enable GPIOA clock

    GPIOA->MODER &= ~(3U << (LED * 2));    // Clear mode bits
    GPIOA->MODER |=  (1U << (LED * 2));    // 01: Output mode

    GPIOA->OTYPER &= ~(1U << LED);         // Push-pull

    GPIOA->PUPDR &= ~(3U << (LED * 2));    // No pull-up/pull-down

    GPIOA->ODR &= ~(1U << LED);            // LED OFF initially
}

int main(void)
{
    LED_GPIO_Init();        // Initialize PA5 LED
    TIM3_Delay_Init();      // Initialize TIM3 delay

    while (1)
    {
        GPIOA->ODR ^= (1 << LED);   // Toggle LED
        TIM3_Delay_ms(500);         // 500 ms delay
    }
}

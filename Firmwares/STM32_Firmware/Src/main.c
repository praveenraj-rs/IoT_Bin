#include <stm32f4xx.h>

#include "config.h"
#include "uart.h"
#include "bin.h"

// --- GLOBALS ---
volatile uint8_t PIR_MOTION_FLAG = 0;
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
        PIR_MOTION_FLAG = 1;
    }
}

// --- MAIN ---
int main(void)
{
    PIR_EXTI0_Init();
    Bin_Init();

    UART1_SendString(MSG_READY);

    char rx_buffer[BUFFER_SIZE];
    uint16_t rx_index = 0;

    while (1)
    {
        // READ UART
        if (!ESP32_RES_FLAG)
        {
            if (UART1_ReadString(rx_buffer, BUFFER_SIZE, &rx_index))
            {
                Process_Command(rx_buffer);
                ESP32_RES_FLAG = 1;
            }
        }

        // PIR FLOW
        if (PIR_MOTION_FLAG && ESP32_RES_FLAG)
        {
            PIR_MOTION_FLAG = 0;

            long dist = UserToBin_Distance();

            if (dist <= USER_TO_BIN_THRESHOLD)
            {
                UART1_SendString(MSG_PIR_DETECT);
                ESP32_RES_FLAG = 0;
            }
        }
    }
}

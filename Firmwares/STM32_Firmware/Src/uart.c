// UART_1 Serial Communication Nucleo

// UART1 Tx -> PB6
// UART1 Rx -> PB7

#include <stm32f4xx.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "config.h"
#include "uart.h"

// --- Initialize UART1 ---
void UART1_Init(void)
{
    // Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;     // Enable GPIOB clock
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;    // Enable USART1 clock

    // PB6 -> TX, PB7 -> RX (Alternate Function mode)
    GPIOB->MODER &= ~((3<<(6*2)) | (3<<(7*2)));
    GPIOB->MODER |=  ((2<<(6*2)) | (2<<(7*2)));

    // Set AF7 (USART1) for PB6 and PB7
    GPIOB->AFR[0] &= ~((0xF<<(6*4)) | (0xF<<(7*4)));
    GPIOB->AFR[0] |=  ((7<<(6*4)) | (7<<(7*4)));

    // Optional (recommended): Set output type & speed
    GPIOB->OSPEEDR |= (3<<(6*2)) | (3<<(7*2)); // High speed
    GPIOB->OTYPER &= ~((1<<6) | (1<<7));       // Push-pull
    GPIOB->PUPDR &= ~((3<<(6*2)) | (3<<(7*2)));
    GPIOB->PUPDR |=  ((1<<(6*2)) | (1<<(7*2))); // Pull-up

    // Set baud rate
    USART1->BRR = UART1_BAUD_RATE;

    // Enable TX, RX and USART
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE;
    USART1->CR1 |= USART_CR1_UE;
}

// --- SEND CHAR ---
void UART1_SendChar(char c)
{
    while(!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

// --- SEND STRING ---
void UART1_SendString(char *s)
{
    while(*s)
    {
        UART1_SendChar(*s++);
    }
}

// --- SEND INT ---
void UART1_SendInt(int num)
{
    char buffer[20];
    sprintf(buffer, "%d", num);
    UART1_SendString(buffer);
}

// --- CHECK UART1 RX ---
int UART1_Available(void)
{
    return (USART1->SR & USART_SR_RXNE);
}

// --- READ CHAR ---
char UART1_Read(void)
{
    return (char)(USART1->DR);
}

// --- READ STRING ---
uint8_t UART1_ReadString(char *buffer, uint16_t buf_size, uint16_t *index)
{
    while (UART1_Available())
    {
        char c = UART1_Read();

        // End of line detected
        if (c == '\r' || c == '\n')
        {
            if (*index > 0)
            {
                buffer[*index] = '\0';

                *index = 0;
                return 1;   // String ready
            }
        }
        else
        {
            if (*index < buf_size - 1)
            {
                buffer[(*index)++] = c;
            }
            else
            {
                // Buffer overflow protection
                *index = 0;
                buffer[0] = '\0';
                UART1_SendString(MSG_ERR);
            }
        }
    }

    return 0; // Not complete yet
}

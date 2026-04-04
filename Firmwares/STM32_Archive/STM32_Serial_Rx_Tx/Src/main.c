#include "stm32f4xx.h"
#include <stdint.h>

#define BUFFER_SIZE 64

// ================= UART INIT =================
void UART1_Init(void)
{
    // Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;     // GPIOB
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;    // USART1

    // PB6 -> TX, PB7 -> RX (Alternate Function)
    GPIOB->MODER &= ~((3<<(6*2)) | (3<<(7*2)));
    GPIOB->MODER |=  ((2<<(6*2)) | (2<<(7*2)));

    // AF7 = USART1
    GPIOB->AFR[0] &= ~((0xF<<(6*4)) | (0xF<<(7*4)));
    GPIOB->AFR[0] |=  ((7<<(6*4)) | (7<<(7*4)));

    // High speed
    GPIOB->OSPEEDR |= (3<<(6*2)) | (3<<(7*2));

    // Pull-up (important for RX stability)
    GPIOB->PUPDR &= ~((3<<(6*2)) | (3<<(7*2)));
    GPIOB->PUPDR |=  ((1<<(6*2)) | (1<<(7*2)));

    // ================= BAUD RATE =================
    // Assuming 16 MHz clock → 9600 baud
    USART1->BRR = 0x0683;

    // Enable TX, RX, UART
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE;
    USART1->CR1 |= USART_CR1_UE;
}

// ================= UART FUNCTIONS =================
void UART1_SendChar(char c)
{
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

void UART1_SendString(char *s)
{
    while (*s)
    {
        UART1_SendChar(*s++);
    }
}

int UART1_Available(void)
{
    return (USART1->SR & USART_SR_RXNE);
}

char UART1_Read(void)
{
    return (char)(USART1->DR);
}

// ================= MAIN =================
int main(void)
{
    UART1_Init();

    char buffer[BUFFER_SIZE];
    uint8_t index = 0;

    UART1_SendString("UART Ready\r\n");

    while (1)
    {
        if (UART1_Available())
        {
            char c = UART1_Read();

            // Echo character immediately (debug)
            UART1_SendChar(c);

            // If Enter pressed → process string
            if (c == '\r' || c == '\n')
            {
                if (index > 0)
                {
                    buffer[index] = '\0';   // terminate string

                    UART1_SendString("\r\nReceived: ");
                    UART1_SendString(buffer);
                    UART1_SendString("\r\n");

                    index = 0; // reset buffer
                }
            }
            else
            {
                if (index < BUFFER_SIZE - 1)
                {
                    buffer[index++] = c;
                }
                else
                {
                    // Overflow protection
                    index = 0;
                    UART1_SendString("\r\nError: Overflow\r\n");
                }
            }
        }
    }
}

//// Serial Communication with UART2
//
//// UART_2 Serial Communication Nucleo
//// UART_2 Tx -> PA2
//// UART_2 Rx -> PA3
//
//
//#include <stm32f4xx.h>
//
//#include "config.h"
//#include "uart.h"
//#include "timer.h"
//
//#define BUFFER_SIZE 50
//
//static char cmd[BUFFER_SIZE];
//static uint8_t index = 0;
//
///*
// * Returns:
// * 1 → command ready
// * 0 → still receiving
// */
//uint8_t UART_ReadCommand(char **out_cmd)
//{
//    while (UART1_Available())
//    {
//        char c = UART1_Read();
//
//        if (c == '\n')
//        {
//            if (index > 0)
//            {
//                cmd[index] = '\0';   // terminate string
//                *out_cmd = cmd;
//
//                index = 0;           // reset for next command
//                return 1;            // command ready
//            }
//        }
//        else
//        {
//            if (index < BUFFER_SIZE - 1)
//            {
//                cmd[index++] = c;
//                //UART1_SendChar(c);   // optional echo
//            }
//            else
//            {
//                // overflow protection
//                index = 0;
//                UART1_SendString("\nError: Overflow\n");
//            }
//        }
//    }
//
//    return 0; // no complete command yet
//}
//
//
//int main(void)
//{
//    UART1_Init();
//    TIM3_Delay_Init();
//
//    //UART1_SendString("UART Ready\n");
//
//    char *received_cmd;
//
//    while (1)
//    {
//        if (UART_ReadCommand(&received_cmd))
//        {
//            UART1_SendString(received_cmd);
//            UART1_SendString("T\n");
//        }
//
//        TIM3_Delay_ms(10);
//    }
//}

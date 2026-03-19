#include "stm32f4xx.h"
#include <stdint.h>

#include "uart.h"
#include "timer.h"

#define BUFFER_SIZE 50

static char cmd[BUFFER_SIZE];
static uint8_t index = 0;

/*
 * Returns:
 * 1 → command ready
 * 0 → still receiving
 */
uint8_t UART_ReadCommand(char **out_cmd)
{
    while (UART2_Available())
    {
        char c = UART2_Read();

        if (c == '\r' || c == '\n')
        {
            if (index > 0)
            {
                cmd[index] = '\0';   // terminate string
                *out_cmd = cmd;

                index = 0;           // reset for next command
                return 1;            // command ready
            }
        }
        else
        {
            if (index < BUFFER_SIZE - 1)
            {
                cmd[index++] = c;
                UART2_SendChar(c);   // optional echo
            }
            else
            {
                // overflow protection
                index = 0;
                UART2_SendString("\nError: Overflow\n");
            }
        }
    }

    return 0; // no complete command yet
}


int main(void)
{
    UART2_Init();
    TIM3_Delay_Init();

    UART2_SendString("UART Ready\n");

    char *received_cmd;

    while (1)
    {
        if (UART_ReadCommand(&received_cmd))
        {
            UART2_SendString("\nReceived: ");
            UART2_SendString(received_cmd);
            UART2_SendString("\n");
        }

        TIM3_Delay_ms(10);
    }
}

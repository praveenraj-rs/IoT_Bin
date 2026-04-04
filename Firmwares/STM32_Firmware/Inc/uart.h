// UART_1 Serial Communication Nucleo

// UART_1 Tx -> PB6
// UART_1 Rx -> PB7

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

// Initialize UART1 (PB6 TX, PB7 RX)
void UART1_Init(void);

// Send single character
void UART1_SendChar(char c);

// Send string
void UART1_SendString(char *s);

// Send Integer Number
void UART1_SendInt(int num);

// Receive UART1 Available (Any Data)
int UART1_Available(void);

// Receive UART1 Char
char UART1_Read(void);

// Receive UART2 String
uint8_t UART1_ReadString(char *buffer, uint16_t buf_size, uint16_t *index);

#endif // UART_H_

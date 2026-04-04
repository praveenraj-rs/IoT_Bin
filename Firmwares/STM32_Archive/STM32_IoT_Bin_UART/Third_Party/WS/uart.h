// UART_1 Serial Communication Nucleo

// UART_1 Tx -> PB6
// UART_1 Rx -> PB7

#ifndef UART_H_
#define UART_H_

#include <stm32f4xx.h>
#include <stdio.h>

// Initialize UART1 (PA2 TX, PA3 RX)
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

// Trim String
void trim_string(char *str);


#endif // UART_H_

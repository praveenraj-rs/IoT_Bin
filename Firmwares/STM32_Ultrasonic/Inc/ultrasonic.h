#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "stm32f4xx.h"
#include <stdint.h>

/* CONFIG */
#define ULTRASONIC_DIVISOR 925

/* FUNCTION PROTOTYPES */

// Init
void Ultrasonic_Init(void);
void TIM2_Ultrasonic_Init(void);

// Read distance (cm)
uint32_t Ultrasonic_Read_cm(uint8_t bin);

// Fill %
uint8_t Bin_GetFillPercent(uint32_t distance, uint32_t bin_height);

#endif

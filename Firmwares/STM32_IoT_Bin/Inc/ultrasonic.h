// Ultrasonic Control (TIM2)

// Trigger Pin -> PB12

// Echo Pins
// Ultrasonic_1 -> PB13
// Ultrasonic_2 -> PB14
// Ultrasonic_3 -> PB15

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stm32f4xx.h>
#include <stdint.h>

// Init
void Ultrasonic_Init(void);
void TIM2_Ultrasonic_Init(void);

// Read distance (cm)
uint32_t Ultrasonic_Read_cm(uint8_t bin);

// Fill %
uint8_t Bin_GetFillPercent(uint32_t distance, uint32_t bin_height);

#endif // ULTRASONIC_H

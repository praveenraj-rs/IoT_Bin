// Servo Control with Timer1 Channels

// Servo1 -> TIM1_CH1 -> PA8
// Servo2 -> TIM1_CH2 -> PA9
// Servo3 -> TIM1_CH3 -> PA10

#ifndef SERVO_H
#define SERVO_H

#include <stm32f4xx.h>
#include <stdint.h>

// Servo Init
void Servo_TIM1_Init(void);

// Set Angle
void Servo_SetAngle(uint8_t channel, uint8_t angle);


#endif // SERVO_H

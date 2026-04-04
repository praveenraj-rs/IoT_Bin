// --- PIN CONFIGURATION ---

// PIR       -> PA0 (Interrupt Enable)
//----------------------------------------------
// UART_1 Tx -> PB6  (ESP32-CAM comms)
// UART_1 Rx -> PB7
//----------------------------------------------
// Servo1    -> TIM1_CH1 -> PA8  (Red 	- 	Bin1)
// Servo2    -> TIM1_CH2 -> PA9  (Green - 	Bin2)
// Servo3    -> TIM1_CH3 -> PA10 (Blue 	- 	Bin3)
//----------------------------------------------
// Ultrasonic_Trig	-> PB8
// Ultrasonic_Mov 	-> PB12	( User Movement Distance )
// Ultrasonic_1   	-> PB13	(Red 	- 	Bin1)
// Ultrasonic_2   	-> PB14	(Green - 	Bin2)
// Ultrasonic_3   	-> PB15	(Blue 	- 	Bin3)
//----------------------------------------------

#ifndef CONFIG_H_
#define CONFIG_H_

// --- SYSTEM CONFIG ---

// System Clock
#define SysClk     16000000U   // 16 MHz

// UART Config
#define UART1_BAUD_RATE 0x0683;  //9600   @16MHz

// --- BIN CONFIG ---

// --- BIN CONSTANTS ---
#define BIN_SERVO_ANGLE_OPEN    90
#define BIN_SERVO_ANGLE_CLOSE   0
#define BIN_SERVO_DELAY_MS      5000
#define BIN_HEIGHT_CM           50

#define USER_TO_BIN_THRESHOLD 	25
#define ULTRASONIC_SAMPLES      5

#define BUFFER_SIZE             64

// BIN Message Formatting
#define MSG_PIR_DETECT  "PIR_DETECT\n"
#define MSG_READY       "READY\n"
#define MSG_ERR         "ERR\n"

#endif // CONFIG_H_

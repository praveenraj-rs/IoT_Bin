#ifndef BIN_H_
#define BIN_H_

#include <stdint.h>

// --- BIN OPERATION ---
long UserToBin_Distance(void);
void Bin_Init(void);
void Process_Command(char *cmd);
void Bin_Operate(uint8_t bin);



// --- SERVO CONTROL ---
void Servo_Open(uint8_t channel);
void Servo_Close(uint8_t channel);


#endif /* BIN_H_ */

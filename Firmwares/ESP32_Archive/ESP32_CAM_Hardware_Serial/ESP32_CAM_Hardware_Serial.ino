#include <Arduino.h>

// UART1 pins
#define RX_PIN 13
#define TX_PIN 12
#define BAUD   115200

HardwareSerial mySerial(1);

void setup()
{
    // USB Serial (for debugging)
    Serial.begin(115200);
    delay(1000);

    Serial.println("ESP32 UART Test");

    // UART1 init
    mySerial.begin(BAUD, SERIAL_8N1, RX_PIN, TX_PIN);

    Serial.println("Sending hello...");
}
 
void loop()
{
    mySerial.print("hello\n");   // send via GPIO12 (TX)
    Serial.println("Sent: hello");

    delay(1000);  // every 1 sec
}

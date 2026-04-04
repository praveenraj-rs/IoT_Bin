#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include "ov3660_camera.h"
#include "bin.h"

// SETUP
void setup(void)
{
    // STM32 communication (UART0)
    Serial.begin(9600);
    delay(1000);
    
    pinMode(FLASH_LED_PIN, OUTPUT);
    digitalWrite(FLASH_LED_PIN, LOW);

    OV3660_Camera_Init();
    connectWiFi();
    
    unsigned long t0 = millis();
    while (millis() - t0 < 5000)
    {
        if (Serial.available())
        {
            String line = Serial.readStringUntil('\n');
            line.trim();

            if (line == "READY")
            {
                break;
            }
        }
    }
}

// LOOP
void loop(void)
{
    // STM32 → ESP32
    if (Serial.available())
    {
        String line = Serial.readStringUntil('\n');
        line.trim();

        if (line == "PIR_DETECT")
        {
          HandlePIRDetect();
        }
        else if (line.startsWith("FILL"))
        {
            ReportFillToEdge(line);
        }
    }
}

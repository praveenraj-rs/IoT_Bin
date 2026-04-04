#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "ov3660_camera.h"

/* WIFI */
const char* ssid = "praveenrajrs";
const char* password = "12345678";

/* SERVER */
String serverURL = "http://192.168.194.95:5000/upload";

// --- WIFI ---
void connectWiFi(void)
{
    Serial.println("Connecting WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

// --- SEND IMAGE ---
void sendImage(void)
{
    Serial.println("Flash ON");

    /* Capture Image */
    camera_fb_t *fb = OV3660_Capture();
    if (!fb)
    {
        Serial.println("Capture failed");
        return;
    }

    /* HTTP POST */
    HTTPClient http;
    http.begin(serverURL);
    http.setTimeout(15000);
    http.addHeader("Content-Type", "image/jpeg");

    Serial.println("Sending image to server...");

    int httpResponseCode = http.POST(fb->buf, fb->len);

    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0)
    {
        String payload = http.getString();

        Serial.println("Server Response:");
        Serial.println(payload);

        /* Decision Logic */
        if (payload.indexOf("RED") >= 0)
        {
            Serial.println("👉 Open RED bin");
        }
        if (payload.indexOf("BLUE") >= 0)
        {
            Serial.println("👉 Open BLUE bin");
        }
        if (payload.indexOf("GREEN") >= 0)
        {
            Serial.println("👉 Open GREEN bin");
        }
    }
    else
    {
        Serial.println("HTTP Request Failed ❌");
    }

    http.end();

    /* Return frame buffer */
    OV3660_ReturnFrame(fb);
}

// --- SETUP ---
void setup(void)
{
    Serial.begin(9600);
    delay(1000);

    Serial.println("\n=== ESP32 CAM SYSTEM START ===");

    /* Flash LED */
    pinMode(FLASH_LED_PIN, OUTPUT);
    digitalWrite(FLASH_LED_PIN, LOW);

    /* Init Camera */
    OV3660_Camera_Init();

    /* Connect WiFi */
    connectWiFi();
}

// --- LOOP ---
void loop(void)
{
    /* Check Serial Command */
    if (Serial.available())
    {
        String msg = Serial.readStringUntil('\n');
        msg.trim();

        Serial.print("Serial Command: ");
        Serial.println(msg);

        if (msg == "motion")
        {
            Serial.println("🚨 Motion detected!");
            sendImage();
        }
    }
}
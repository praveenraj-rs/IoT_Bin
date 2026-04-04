#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "ov3660_camera.h"

// ---------------------------------------------------------------
// WiFi 
// ---------------------------------------------------------------
const char* ssid     = "praveenrajrs";
const char* password = "12345678";

// ---------------------------------------------------------------
// Edge Server (RPi5)
// ---------------------------------------------------------------
const char* EDGE_CLASSIFY_URL = "https://192.168.194.95:5000/upload";
const char* EDGE_FILL_URL     = "https://192.168.194.95:5000/bin_fill";

// ---------------------------------------------------------------
// UART Configuration
// ---------------------------------------------------------------
// STM32 → UART0 (GPIO1 TX, GPIO3 RX)
#define STM32_SERIAL Serial

// Debug → UART1 (GPIO12 TX, GPIO13 RX)
HardwareSerial DebugSerial(1);

// ---------------------------------------------------------------
// WiFi connect
// ---------------------------------------------------------------
void connectWiFi(void)
{
    DebugSerial.println("[WiFi] Connecting...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        DebugSerial.print(".");
    }

    DebugSerial.println();
    DebugSerial.print("[WiFi] Connected — IP: ");
    DebugSerial.println(WiFi.localIP());
}

// ---------------------------------------------------------------
// HTTPS helper
// ---------------------------------------------------------------
static void configureHTTPS(HTTPClient& http, const char* url)
{
    WiFiClientSecure* client = new WiFiClientSecure();
    client->setInsecure();
    http.begin(*client, url);
    http.setTimeout(15000);
}

// ---------------------------------------------------------------
// Capture + classify
// ---------------------------------------------------------------
String ClassifyViaEdge(void)
{
    camera_fb_t* fb = OV3660_Capture();
    if (!fb)
    {
        DebugSerial.println("[CAM] Capture failed");
        return "";
    }

    HTTPClient http;
    configureHTTPS(http, EDGE_CLASSIFY_URL);
    http.addHeader("Content-Type", "image/jpeg");

    DebugSerial.println("[HTTP] POST /upload...");
    int code = http.POST(fb->buf, fb->len);

    OV3660_ReturnFrame(fb);

    if (code <= 0)
    {
        DebugSerial.printf("[HTTP] POST failed: %d\n", code);
        http.end();
        return "";
    }

    String response = http.getString();
    response.trim();
    http.end();

    DebugSerial.printf("[HTTP] Response: %s\n", response.c_str());
    return response;
}

// ---------------------------------------------------------------
// Label → Bin
// ---------------------------------------------------------------
String LabelToBinToken(const String& label)
{
    if (label.indexOf("RED")   >= 0) return "B1";
    if (label.indexOf("BLUE")  >= 0) return "B2";
    if (label.indexOf("GREEN") >= 0) return "B3";
    return "";
}

// ---------------------------------------------------------------
// Fill reporting
// ---------------------------------------------------------------
void ReportFillToEdge(const String& fillMsg)
{
    int colon = fillMsg.indexOf(':');
    if (colon < 4)
    {
        DebugSerial.println("[FILL] Parse error");
        return;
    }

    uint8_t bin  = fillMsg.substring(4, colon).toInt();
    uint8_t fill = fillMsg.substring(colon + 1).toInt();

    DebugSerial.printf("[FILL] Bin %d = %d%%\n", bin, fill);

    char payload[48];
    snprintf(payload, sizeof(payload),
             "{\"bin\":%d,\"fill\":%d}", bin, fill);

    HTTPClient http;
    configureHTTPS(http, EDGE_FILL_URL);
    http.addHeader("Content-Type", "application/json");

    int code = http.POST((uint8_t*)payload, strlen(payload));
    DebugSerial.printf("[HTTP] /bin_fill → %d\n", code);

    http.end();
}

// ---------------------------------------------------------------
// PIR handler
// ---------------------------------------------------------------
void HandlePIRDetect(void)
{
    DebugSerial.println("[EVENT] PIR_DETECT");

    String label = ClassifyViaEdge();

    if (label == "")
    {
        STM32_SERIAL.print("ERR\n");
        return;
    }

    String token = LabelToBinToken(label);

    if (token == "")
    {
        STM32_SERIAL.print("ERR\n");
        return;
    }

    DebugSerial.printf("[SEND] %s → STM32\n", token.c_str());
    STM32_SERIAL.print(token + "\n");
}

// ---------------------------------------------------------------
// SETUP
// ---------------------------------------------------------------
void setup(void)
{
    // STM32 communication (UART0)
    STM32_SERIAL.begin(9600);

    // Debug UART (UART1)
    DebugSerial.begin(115200, SERIAL_8N1, 13, 12);

    delay(1000);
    DebugSerial.println("\n=== ESP32-CAM STARTED ===");

    pinMode(FLASH_LED_PIN, OUTPUT);
    digitalWrite(FLASH_LED_PIN, LOW);

    OV3660_Camera_Init();
    connectWiFi();

    DebugSerial.println("[STM32] Waiting for READY...");

    STM32_SERIAL.println("Hello from ESP32 CAM");
    unsigned long t0 = millis();
    while (millis() - t0 < 5000)
    {
        if (STM32_SERIAL.available())
        {
            String line = STM32_SERIAL.readStringUntil('\n');
            line.trim();

            if (line == "READY")
            {
                DebugSerial.println("[STM32] Connected ✅");
                break;
            }
        }
    }
}

// ---------------------------------------------------------------
// LOOP
// ---------------------------------------------------------------
void loop(void)
{
    // STM32 → ESP32
    if (STM32_SERIAL.available())
    {
        String line = STM32_SERIAL.readStringUntil('\n');
        line.trim();

        DebugSerial.printf("[RX] %s\n", line.c_str());

        if (line == "PIR_DETECT")
        {
            HandlePIRDetect();
        }
        else if (line.startsWith("FILL"))
        {
            ReportFillToEdge(line);
        }
        else
        {
            DebugSerial.println("[WARN] Unknown msg");
        }
    }

    // Debug commands via UART1
    if (DebugSerial.available())
    {
        String cmd = DebugSerial.readStringUntil('\n');
        cmd.trim();

        if (cmd == "test")
        {
            DebugSerial.println("[TEST] Trigger PIR");
            HandlePIRDetect();
        }
    }
}

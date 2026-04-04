#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include "ov3660_camera.h"
#include "bin.h"
#include "config.h"


// WiFi connect
void connectWiFi(void)
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
}

// HTTPS helper
static void configureHTTPS(HTTPClient& http, const char* url)
{
    WiFiClientSecure* client = new WiFiClientSecure();
    client->setInsecure();
    http.begin(*client, url);
    http.setTimeout(8000);
}


// Capture + Classify
static String ClassifyViaEdge(void)
{
    camera_fb_t* fb = OV3660_Capture();
    if (!fb) // Capture failed
    {
        return "";
    }

    HTTPClient http;
    configureHTTPS(http, EDGE_CLASSIFY_URL);
    http.addHeader("Content-Type", "image/jpeg");
    
    int code = http.POST(fb->buf, fb->len);

    OV3660_ReturnFrame(fb);

    if (code <= 0) // POST failed
    {
        http.end();
        return "";
    }

    String response = http.getString();
    response.trim();
    http.end();
    
    return response;
}

// Label → Bin
static String LabelToBinToken(const String& label)
{
    if (label.indexOf("RED")   >= 0) return "B1";
    if (label.indexOf("GREEN")  >= 0) return "B2";
    if (label.indexOf("BLUE") >= 0) return "B3";
    return "";
}

// Fill reporting
void ReportFillToEdge(const String& fillMsg)
{
    int colon = fillMsg.indexOf(':');
    if (colon < 4) // Parse error
    {
        return;
    }

    uint8_t bin  = fillMsg.substring(4, colon).toInt();
    uint8_t fill = fillMsg.substring(colon + 1).toInt();

    char payload[48];
    snprintf(payload, sizeof(payload),
             "{\"bin\":%d,\"fill\":%d}", bin, fill);

    HTTPClient http;
    configureHTTPS(http, EDGE_FILL_URL);
    http.addHeader("Content-Type", "application/json");

    int code = http.POST((uint8_t*)payload, strlen(payload));
    //DebugSerial.printf("[HTTP] /bin_fill → %d\n", code);

    http.end();
}

// PIR handler
void HandlePIRDetect(void)
{

    String label = ClassifyViaEdge();

    if (label == "")
    {
        //Serial.print("ERR\n");
        Serial.print("B2\n");
        return;
    }

    String token = LabelToBinToken(label);

    if (token == "")
    {
        Serial.print("ERR\n");
        return;
    }
    Serial.print(token + "\n");
}

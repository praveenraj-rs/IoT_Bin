  #include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

/* WIFI */
const char* ssid = "praveenrajrs";
const char* password = "12345678";

/* SERVER */
String serverURL = "http://192.168.30.9:5000/upload";

/* FLASH LED */
#define FLASH_LED_PIN 4

/* CAMERA PINS (AI Thinker) */
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

/* CAMERA INIT */

void startCamera() 
{
    camera_config_t config;

    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;

    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;

    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;

    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;

    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_QVGA;
    config.pixel_format = PIXFORMAT_JPEG;

    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
  
    config.jpeg_quality = 12;
    config.fb_count = 1;

    esp_camera_init(&config);
    
    sensor_t *s = esp_camera_sensor_get();
    
    // initial sensors are flipped vertically and colors are a bit saturated
    if (s->id.PID == OV3660_PID) {
      s->set_vflip(s, 1);        // flip it back
      s->set_brightness(s, 1);   // up the brightness just a bit
      s->set_saturation(s, -2);  // lower the saturation
    }

    Serial.println("Camera started");
}

/* WIFI */

void connectWiFi() {
    Serial.println("Connecting WiFi");
    WiFi.begin(ssid,password);

    while(WiFi.status()!=WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");
    Serial.println(WiFi.localIP());
}

/* SEND IMAGE */

void sendImage() {

    Serial.println("Flash ON");
    digitalWrite(FLASH_LED_PIN,HIGH);
    delay(200);
    camera_fb_t * fb = esp_camera_fb_get();

    if(!fb)
    {
        Serial.println("Camera failed");
        return;
    }

    Serial.print("Image size: ");
    Serial.println(fb->len);

    HTTPClient http;
    http.begin(serverURL);
    http.setTimeout(15000); // 10sec timeout
    http.addHeader("Content-Type","image/jpeg");
    
    Serial.println("Sending image to server");
    
    int httpResponseCode = http.POST(fb->buf, fb->len);

    Serial.print("HTTP Code: ");
    Serial.println(httpResponseCode);

    if(httpResponseCode > 0){

        String payload = http.getString();
        Serial.println("Server response:");
        Serial.println(payload);
        
        /* Decide bin */
        if(payload.indexOf("RED") >= 0){
            Serial.println("Open RED bin");
        }
        if(payload.indexOf("BLUE") >= 0){
            Serial.println("Open BLUE bin");
        }
        if(payload.indexOf("GREEN") >= 0){
            Serial.println("Open GREEN bin");
        }

    }
    http.end();
    esp_camera_fb_return(fb);
    digitalWrite(FLASH_LED_PIN,LOW);
    Serial.println("Flash OFF");
}

// SETUP
void setup()
{
    Serial.begin(115200);
    pinMode(FLASH_LED_PIN,OUTPUT);
    startCamera();
    connectWiFi();
}

// LOOP
void loop()
{
    if(Serial.available())
    {
        String msg = Serial.readStringUntil('\n');
        msg.trim();

        Serial.print("Serial command: ");
        Serial.println(msg);

        if(msg == "motion")
        {
            Serial.println("Motion detected");
            sendImage();
        }
    }
}

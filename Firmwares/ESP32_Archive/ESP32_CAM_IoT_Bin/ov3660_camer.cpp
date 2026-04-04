#include "ov3660_camera.h"

/* CAMERA INIT */
void OV3660_Camera_Init(void)
{
    camera_config_t config;

    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer   = LEDC_TIMER_0;

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

    config.pin_pwdn  = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    config.xclk_freq_hz = 20000000;
    config.frame_size   = FRAMESIZE_QVGA;
    config.pixel_format = PIXFORMAT_JPEG;

    config.grab_mode  = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;

    config.jpeg_quality = 12;
    config.fb_count     = 1;

    if (esp_camera_init(&config) != ESP_OK)
    {
        Serial.println("Camera Init Failed");
        return;
    }

    sensor_t *s = esp_camera_sensor_get();

    if (s->id.PID == OV3660_PID)
    {
        s->set_vflip(s, 1);
        s->set_brightness(s, 1);
        s->set_saturation(s, -2);
    }

    Serial.println("OV3660 Camera Init Done");
}

/* CAPTURE IMAGE */
camera_fb_t* OV3660_Capture(void)
{
    digitalWrite(FLASH_LED_PIN, HIGH);
    delay(200);

    camera_fb_t *fb = esp_camera_fb_get();

    if (!fb)
    {
        Serial.println("Capture Failed");
        digitalWrite(FLASH_LED_PIN, LOW);
        return NULL;
    }

    Serial.print("Image size: ");
    Serial.println(fb->len);

    return fb;
}

/* RETURN FRAME BUFFER */
void OV3660_ReturnFrame(camera_fb_t *fb)
{
    if (fb)
    {
        esp_camera_fb_return(fb);
    }

    digitalWrite(FLASH_LED_PIN, LOW);
    Serial.println("Flash OFF");
}
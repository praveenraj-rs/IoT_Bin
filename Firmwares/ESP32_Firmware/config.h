#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

// --- MACROS ---
#define DUSTBIN_ID 3

// --- WIFI ---
extern const char* ssid     = "iot";
extern const char* password = "12345678";


// --- Edge Server ---
extern const char* EDGE_CLASSIFY_URL = "https://192.168.189.215:5000/upload";
extern const char* EDGE_FILL_URL     = "https://192.168.189.215:5000/bin_fill";


#endif

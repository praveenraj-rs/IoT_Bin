#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

// --- WIFI ---
extern const char* ssid     = "praveenrajrs";
extern const char* password = "12345678";


// --- Edge Server ---
extern const char* EDGE_CLASSIFY_URL = "https://192.168.192.95:5000/upload";
extern const char* EDGE_FILL_URL     = "https://192.168.192.95:5000/bin_fill";


#endif

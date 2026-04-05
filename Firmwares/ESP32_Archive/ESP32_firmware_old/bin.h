#ifndef BIN_H
#define BIN_H

#include "Arduino.h"

void connectWiFi(void);

void HandlePIRDetect(void);
void ReportFillToEdge(const String& fillMsg);

#endif

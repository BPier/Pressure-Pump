#ifndef LEDStatusLibrary_H
#define LEDStatusLibrary_H

#include <Arduino.h>

void UpdateStatus(int *Statusarray, int Xstatus);

void DisplayStatus(int *Statusarray,const int *pindisplayarray);

void SetupStatus(const int *pindisplayarray);

#endif
#ifndef PumpLibrary_H
#define PumpLibrary_H

#include <Arduino.h>

void SetupPump(const int pin);

void StartPump(const int PinMotor, bool *pumpstatus);
void StopPump(const int PinMotor, bool *pumpstatus);

#endif
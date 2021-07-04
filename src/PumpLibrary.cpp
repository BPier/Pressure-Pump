#include "PumpLibrary.h"

// Setting up the pump pin
void SetupPump(const int pin)
{
    pinMode(pin,OUTPUT);
}

// Starting up the pump and trying to put the circuit under pressure

void StartPump(const int PinMotor, bool *pumpstatus)
{
    digitalWrite(PinMotor,HIGH);
    *pumpstatus=true;
    return;
}
void StopPump(const int PinMotor, bool *pumpstatus);
#ifndef SensorReading_H
#define SensorReading_H

#include <Arduino.h>

void SetupPressureSensor(const char PinSensor);
float GetPressure(const char PinSensor);


#endif
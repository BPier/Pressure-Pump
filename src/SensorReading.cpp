#include "SensorReading.h"

// ==== Pressure sensor function ====
// These function work for a 30PSI Pressure Sensor

void SetupPressureSensor(const char PinSensor)
{
    pinMode(PinSensor,INPUT);
}

float GetPressure(const char PinSensor)
{
  float pressure1 = analogRead(PinSensor);//read the pressure
  delay(10);
  float pressure2 = analogRead(PinSensor);
  delay(10);
  float pressure3 = analogRead(PinSensor);
  delay(10);
  float pressure4 = analogRead(PinSensor);//read the pressure
  delay(10);
  float pressure5 = analogRead(PinSensor);
  delay(10);
  float pressure6 = analogRead(PinSensor);

  float pressure = (pressure1+pressure2+pressure3+pressure4+pressure5+pressure6)/6; //average the readings
  Serial.print("Voltage =");Serial.print(pressure1);Serial.print(" - ");Serial.print(pressure2);Serial.print(" - ");Serial.print(pressure3);Serial.print(" - ");Serial.print(pressure4);Serial.print(" - ");Serial.print(pressure5);
  pressure = pressure*5000/1024; //voltage
  //Serial.print("Voltage =");Serial.print(pressure/1000);
  pressure = 0.025*pressure - 12.5;
  pressure = pressure/14.503773800722; // Convert PSI to BAR
  return pressure;
}


// ==== Flow sensor function ====
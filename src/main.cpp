#include <Arduino.h>
#include <LEDStatusLibrary.h>
#include <SensorReading.h>

/* ===========================
==== Variables definition ====
==============================*/

// ==== User Variable ====
const int SetupTimer = 15000; //Maximum number of milliseconds that the setup should last
const int SetupTry = 3; // numbre of time that the setup will restart
const int SetupDelay = 2000; // Delay between setup trials
const float PressureMin = 1.4; // Pressure at wich the pump will start in bar
const float PressureMax = 3; // Minimum running pressure of the pump. The pump will not stop unless this pressure is achieved in bar

// ==== PIN Definition ====
const int pinLED[]={6,11,10,9};
const char pinPressureSensor = A6;
const int pinFlowSensor = 2;
const int pinPumpRelay = 5;

// ==== Status Variable ====
int status[]={1,1,1,1};

// ==== Time Variable ====
unsigned long currentTime;
unsigned long cloopTime,xTime;
const int TimePeriod=1000;

// ==== Pump Variable ====
bool PumpRunning = false;

// ==== Flow Variables ====
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
int FlowValue=0;

// ==== Pressure Variable ====
float PressureValue = 0;

// === Warning Variable ===
// --- Low Pressure ---
bool LowPressureWarning = false;
unsigned long LowPressureTime = 0;
const unsigned int LowPressureWarningDelay = 4000;

// Variable For Display
int Display = 2; // 1 for terminal output, 2 for excel export
/* ===========================
==== Functions definition ====
==============================*/

// ==== Flow Funtion =====
void flow ();
void SetupFlowSensor(const int PinSensor);
int GetFlow();

// ==== Other Funtion =====
void SetupSequence();

// ==== Pump Function =====
void SetupPump();
void StartPump();
void StopPump();

// === Warning Function ===
// --- Low Pressure ---
void LowPressureWarningTest();

/* ====================
==== Main Programm ====
=======================*/

void setup() {
  Serial.begin(9600);
  SetupStatus(pinLED);
  SetupPressureSensor(pinPressureSensor);
  SetupFlowSensor(pinFlowSensor);
  SetupPump();

  SetupSequence();

}

void loop() {
  DisplayStatus(status, pinLED);
  UpdateStatus(status, 5555);
  currentTime = millis();

  if((currentTime - cloopTime) >= TimePeriod)
  {
    cloopTime = currentTime;
    FlowValue=GetFlow();
    delay(20);
    PressureValue = GetPressure(pinPressureSensor);
    if(Display==1)
    {
    Serial.print(" | Pressure = ");Serial.print(PressureValue);
    Serial.print(" | Flow = ");Serial.print(FlowValue);
    Serial.print(" | Pump = ");Serial.println(PumpRunning);
    }
    if(Display==2)
    {
    Serial.print(", Pressure = ,");Serial.print(PressureValue);
    Serial.print(", Flow = ,");Serial.print(FlowValue);
    Serial.print(", Pump = ,");Serial.println(int(PumpRunning));
    }
    //If pressure get low start the pump
    if(((PressureValue<=PressureMin))&&PumpRunning==false)
    {
      //Second reading to confirm the first one
      FlowValue=GetFlow();
      PressureValue = GetPressure(pinPressureSensor);
      if((PressureValue<=PressureMin)&&PumpRunning==false)
      {
        StartPump();
      }  
    }
    //If pressure is high and flow is 0 then stop the pump
    if(((PressureValue>=PressureMax) && (FlowValue==0))&&PumpRunning==true)
    {
      //Second reading to confirm the first one
      FlowValue=GetFlow();
      PressureValue = GetPressure(pinPressureSensor);
      if((PressureValue>=PressureMax) && (FlowValue==0))
      {
        StopPump();
      }
      
    }
    LowPressureWarningTest();
  }
}

/* ================
==== Functions ====
===================*/

// ==== Flow Functions ====
void flow () // Interrupt function
{
   flow_frequency++;
}

void SetupFlowSensor(const int PinSensor)
{
    pinMode(PinSensor,INPUT);
    digitalWrite(PinSensor, HIGH); // Optional Internal Pull-Up
    attachInterrupt(0, flow, RISING); // Setup Interrupt
    sei(); // Enable interrupts
}

int GetFlow()
{
  // Pulse frequency (Hz) = 11Q, Q is flow rate in L/min.
  int l_hour = (flow_frequency * 60 / 11); // (Pulse frequency x 60 min) / 11Q = flowrate in L/hour
  flow_frequency = 0; // Reset Counter
  return(l_hour);
}

// ==== Setup Functions ====
void SetupSequence()
{
    unsigned long TimeStartSetup = millis();
    StartPump();
    delay(1000);
    for(int i = 1;i<=SetupTry;i++)
    {
      while((millis()-TimeStartSetup) < SetupTimer)
      {
        PressureValue = GetPressure(pinPressureSensor);
        if (PressureValue>=PressureMax)
        {
          StopPump();
          Serial.print("Reset Successful - Pressure = ");Serial.print(PressureValue);Serial.print(" bar");
          return;
        }
      }
    }
    StopPump();
    UpdateStatus(status,1444);
    Serial.print("Reset Failed - Pressure = ");Serial.print(PressureValue);Serial.print(" bar");
    for(;;)
    {
      DisplayStatus(status,pinLED);
    }
    
}

// ==== Pump Functions ====
// Setting up the pump pin
void SetupPump()
{
    pinMode(pinPumpRelay,OUTPUT);
}

void StartPump()
{
    digitalWrite(pinPumpRelay,LOW);
    PumpRunning=true;
    return;
}
void StopPump()
{
    digitalWrite(pinPumpRelay,HIGH);
    PumpRunning=false;
    return;
}

// === Warning Function ===
// --- Low Pressure Warning ---
// The goal of this function is to detect if the pump is running dry or if the pump is not running at all.
// If there is no water the pump will be running but the pressure will stay low and the flow will be low or equal to 0. Same thing if the pump has a problem
void LowPressureWarningTest()
{
  if(PumpRunning && !LowPressureWarning &&(PressureValue<(PressureMax*.8)) && (FlowValue<20))
  {
    LowPressureWarning=true;
    LowPressureTime=currentTime;
  }
  if(LowPressureWarning && ((currentTime-LowPressureTime)>=LowPressureWarningDelay))
  {
    StopPump();
    UpdateStatus(status,1334);
    for(;;)
    {
      DisplayStatus(status,pinLED);
    }
  }
  if((PressureValue>=PressureMax) || (FlowValue>30))
  {
    LowPressureWarning=false;
  }
}
/* ======================
==== Error Code List ====
=========================

2111 - Pump Running Normally
3333 - Reset Running
1444 - Reset Failed
1334 - Low Pressure for mor than 5s -  Pump stopped




*/
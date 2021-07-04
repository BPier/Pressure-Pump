#include "LEDStatusLibrary.h"


// Set Up the pin used for displaying the status.
// The pins have to be pwd for the fading function to work
// Example of variable definition : int pinLED[]={6,11,10,9};
// Function example : SetUpStatus(pinLED);
void SetupStatus(const int *pindisplayarray)
{
  for (int i=0;i<4;i++)
  {
    pinMode(pindisplayarray[i],OUTPUT);
  }
}

// This function allows to change the status of the PIN
// Function example : UpdateStatus(status, 4444);
// This example set the 4 LED to fading simultaneously
// /!\ Do Not USE an Integer starting by 0, 0444 for example 
//
// Blink Setup: 1 - Turned off ; 2 - Turned on ; 3 - Blink SLowly ; 4 - Blink Fast
void UpdateStatus(int *Statusarray, int Xstatus)
{
  Statusarray[3] = (Xstatus%10);
  Statusarray[2] = ((Xstatus/10)%10);
  Statusarray[1] = ((Xstatus/100)%10);
  Statusarray[0] = (Xstatus/1000);
}

// This function send the information to the pins.
// Function example : DisplayStatus(status, pinLED);
void DisplayStatus(int *Statusarray,const int *pindisplayarray)
{
  int SlowBlink = 0;
  int QuickBlink = 0;
  int FadeBlink = 0;
  for (int i=0;i<4;i++)
  {
    switch (Statusarray[i])
    {
    case 1:
    // Simply light off the LED
      digitalWrite(pindisplayarray[i],LOW);
      break;
    case 2:
    // Simply light up the LED
      digitalWrite(pindisplayarray[i],HIGH);
      break;
    case 3:
    // Make the LED blink slowly
      SlowBlink = (millis() / 1000) % 2;
      digitalWrite(pindisplayarray[i], SlowBlink);
      break;
    case 4:
    // Make the LED blink fast
      QuickBlink = (millis() / 100) % 2;
      digitalWrite(pindisplayarray[i], QuickBlink);
      break;
    case 5:
    // Make the LED brightness slowly fade up and down
      FadeBlink = ((millis()/10)%510)-255;
      FadeBlink = abs(FadeBlink);
      analogWrite(pindisplayarray[i], FadeBlink);
      break;
    default:
    // All the other values are  turning the LED off
      digitalWrite(pindisplayarray[i],LOW);
      break;
    }
  }
}
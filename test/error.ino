#include <Arduino.h>

const char LEDPin[] = {12,11,10,9};
int ERRORdigit[]={9,9,9,9};

void setup() {
  for (int i=0; i<4; i++)
  {
    pinMode(LEDPin[i],OUTPUT);
  }
  
}

void loop() {
  SetError(0333);
  DisplayError();
}

void DisplayError() {
  for (int i=0; i<4; i++)
  {
    switch (ERRORdigit[i]) {
      case 0:
        digitalWrite(LEDPin[i], LOW);
        break;
      case 1:
        digitalWrite(LEDPin[i], HIGH);
        break;
      case 2:
        digitalWrite(LEDPin[i], (millis() / 1000) % 2);
        break;
      case 3:
        digitalWrite(LEDPin[i], (millis() / 250) % 2);
        break;

    }
  }
}

void SetError(int error) {
  ERRORdigit[0]=error/1000;
  ERRORdigit[1]=((error/100)%10);
  ERRORdigit[2]=((error/10)%10);
  ERRORdigit[3]=(error%10);
  return;
}
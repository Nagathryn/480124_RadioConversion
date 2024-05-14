#include <Arduino.h>
#include <ezButton.h>

#define B_PLAY        32
#define LED           26

ezButton button1(B_PLAY); 

int ledState = LOW;     // the current state of LED
int lastButtonState;    // the previous state of button
int currentButtonState; // the current state of button

void setup() {

  Serial.begin(115200);

  int iDebounce = 10;
  button1.setDebounceTime(iDebounce); 
  pinMode(LED, OUTPUT);

}

void loop() {

  button1.loop(); 
  if(button1.isPressed()) 
  {
    ledState = !ledState;
       digitalWrite(LED, ledState);
  }
}

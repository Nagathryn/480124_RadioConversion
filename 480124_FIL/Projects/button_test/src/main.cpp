#include <Arduino.h>
#include <ezButton.h>

#define BUTTON_PIN  26 // ESP32 pin GPIO18, which connected to button
#define LED_PIN     32 // ESP32 pin GPIO21, which connected to led

ezButton button(BUTTON_PIN);  // create ezButton object that attach to pin 7;

// variables will change:
int led_state = LOW;   // the current state of LED

void setup() {
  Serial.begin(9600);         // initialize serial
  pinMode(LED_PIN, OUTPUT);   // set ESP32 pin to output mode
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  button.loop(); // MUST call the loop() function first

  if (button.isPressed()) {
    Serial.println("The button is pressed");

    // toggle state of LED
    led_state = !led_state;

    // control LED arccoding to the toggleed sate
    digitalWrite(LED_PIN, led_state);
  }
}
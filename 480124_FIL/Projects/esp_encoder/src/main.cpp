#include <Arduino.h>

// Rotary Encoder Inputs
#define CLK 12
#define DT 14

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

void setup() {
        
	pinMode(CLK,INPUT);
	pinMode(DT,INPUT);

	Serial.begin(115200);

	lastStateCLK = digitalRead(CLK);
}

void loop() {
        
	currentStateCLK = digitalRead(CLK);

	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		if (digitalRead(DT) != currentStateCLK) {
      if(counter -5 >= 0) counter = counter - 5;
			currentDir ="CCW";
		} else {
			if(counter + 5 <= 127) counter = counter + 5;
			currentDir ="CW";
		}

		Serial.print("Direction: ");
		Serial.print(currentDir);
		Serial.print(" | Counter: ");
		Serial.println(counter);
	}

	lastStateCLK = currentStateCLK;

	delay(1);
}
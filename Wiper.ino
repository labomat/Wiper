// *********************************************************************
//
//      window wiper simulator
//
//  window wiper simulator for rc model boat
//  micro geared stepper motor and a498 compatible stepper driver board
//  author: Kai Laborenz 
//  CC-BY SA
//
// *********************************************************************

#include <IBusBM.h>

const int dirPin = 4;
const int stepPin = 3;
const int switchPin = 5;
const int enablePin = 6;

const int stepsPerDirection = 3000;  // steps for one direction 

int stepDelay = 100;            // pause between steps => speed
int midDelay = 100;             // pause before reverse 
int endDelay = 100;             // pause after one cycle (for intervall mode)

int mode = 1;
bool start = 0;

IBusBM IBus; 

void setup()
{
  IBus.begin(Serial);

	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop()
{

if (IBus.readChannel(4) > 1700 ) {
      mode = 2;
      start = 1;
} 
else if (IBus.readChannel(4) < 1300 ) {
      mode = 3;
      start = 1;
} else {
    mode = 1;
    start = 0;
}
  switch (mode) {
  case 1:             // normal speed
    stepDelay = 100;
    midDelay = 100;  
    endDelay = 100;  
    break;
  case 2:             // high speed
    stepDelay = 60;
    midDelay = 80;  
    endDelay = 80;  
    break;
  case 3:             // slow interval
    stepDelay = 110;
    midDelay = 100;  
    endDelay = 800;  
    break;
  case 4:             // fast interval
    stepDelay = 100;
    midDelay = 100;  
    endDelay = 1600;  
    break;

 }

	// Set motor direction clockwise
	digitalWrite(dirPin, HIGH);

  //if(!digitalRead(switchPin)) {
if (start) {
  digitalWrite(LED_BUILTIN, HIGH);
  
  digitalWrite(enablePin, LOW);
	// Spin motor 
	for(int x = 0; x < stepsPerDirection; x++)
	{
		digitalWrite(stepPin, HIGH);
		delayMicroseconds(stepDelay);
		digitalWrite(stepPin, LOW);
		delayMicroseconds(stepDelay);
	}
	delay(midDelay); 
	
	// Set motor direction counterclockwise
	digitalWrite(dirPin, LOW);

	// Spin motor backwards
	for(int x = 0; x < stepsPerDirection; x++)
	{
		digitalWrite(stepPin, HIGH);
		delayMicroseconds(stepDelay);
		digitalWrite(stepPin, LOW);
		delayMicroseconds(stepDelay);
	}
	delay(endDelay); 
  
  }
  else {
    digitalWrite(enablePin, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    }
}
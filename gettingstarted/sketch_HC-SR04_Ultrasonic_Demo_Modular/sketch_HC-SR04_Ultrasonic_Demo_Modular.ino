
#include "HCSR04.h"    // <> means look in standard lib location.  Using " to look locally
#include <stdio.h> // for function sprintf
#include <TM1637Display.h>  //4-digit display


// Pins
const int TRIG_PIN = 7;
const int ECHO_PIN = 8;
const int PIN_4DIGIT_CLOCK = 10;
const int PIN_4DIGIT_DIO = 9;

HCSR04 *sensor = new HCSR04();;
char msg[80];


// init the 4-digit LCD display 
TM1637Display  display = TM1637Display(PIN_4DIGIT_CLOCK, PIN_4DIGIT_DIO);
display.showNumberDec(dd)
 



void setup() {

  // initialize the HC-SR04 Ultrasonic range finder
  sensor->setPinTrig(TRIG_PIN)->setPinEcho(ECHO_PIN)->setup();

  // Turn on the display. Set brightness A number from 0 (lowes brightness) to 7 (hi).
  display.setBrightness(0x04, true /*on*/);
  
 
  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
}



void loop() {

//Serial.println("loop");
  sensor->loop();  // not really needed


  int clearance = sensor->read();
  sprintf(msg, "%d cm", clearance);   // arduino sprintf does not support float
  Serial.println(msg);

  displayInt( clearance );

  // Wait at least X ms before next measurement
  delay(200);
}


void displayInt( int num){

  uint8_t len = 1;
  if ( num > 9 ) { len = 2; };
  if ( num > 99 ) { len = 3; };
  if ( num > 999 ) { len = 4; };
  
  display.showNumberDec(num, false);

}


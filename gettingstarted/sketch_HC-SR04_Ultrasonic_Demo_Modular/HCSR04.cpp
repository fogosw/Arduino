#include "HCSR04.h"

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;


void HCSR04::setup() {

  // The Trigger pin will tell the sensor to range find
  pinMode(pinTrig, OUTPUT);
  digitalWrite(pinTrig, LOW);
}



int HCSR04::read() {
  return (int)readFloat();
}


float HCSR04::readFloat() {

  unsigned long  t0 = micros();
  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(pinEcho) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(pinEcho) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed 
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;
  inches = pulse_width / 148.0;


  // out of range ?
  if ( pulse_width > MAX_DIST ) {
    // cm = 999.0;   // out of range
    cm = 400.0;   // out of range
  }

  return cm;
}

  

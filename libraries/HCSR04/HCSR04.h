#ifndef HCSR04_H
#define HCSR04_H

#include "Sensor.h"

/* HC-SR04 Demo
 * Demonstration of the HC-SR04 Ultrasonic Sensor
 * Date: August 3, 2016
 * 
 * Description:
 *  Connect the ultrasonic sensor to the Arduino as per the
 *  hardware connections below. Run the sketch and open a serial
 *  monitor. The distance read from the sensor will be displayed
 *  in centimeters and inches.
 * 
 * Hardware Connections:
 *  Arduino | HC-SR04 
 *  -------------------
 *    5V    |   VCC     
 *    7     |   Trig     
 *    8     |   Echo     
 *    GND   |   GND
 */

class HCSR04 : public Sensor  {

    public:

      HCSR04(int pinTrig, int pinEcho);
      void setup();
      /**
       * return distance in cm.  999 indicates out of range.  Effective range seems to be 4-250cm
       */
      float readFloat();
      int read();
      
      HCSR04* setPinTrig( int pinNo ) { pinTrig = pinNo;  return this;};
      HCSR04* setPinEcho( int pinNo ) { pinEcho = pinNo;  return this;};
      
    private:
      int pinTrig;
      int pinEcho;
};
#endif

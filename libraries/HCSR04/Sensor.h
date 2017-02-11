#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>


/**
    Abstract class Sensor;  represents a sensor device.
  
   A class that contains at least one pure virtual function is considered an abstract class. 
   Classes derived from the abstract class must implement the pure virtual function or they, 
   too, are abstract classes

   about virtual functions
   https://msdn.microsoft.com/en-us/library/0y01k918.aspx

   about abstract classes
   https://msdn.microsoft.com/en-us/library/c8whxhf1.aspx
*/
class Sensor {
  public:
    //initialize the sensor
    //this is called a pure virtual function;  must be implimented in derived class
    virtual void setup() = 0;

   
    /**
     * 
     * Assumed to be called in a timely fashion.  Can be used to avoid long internal delays
     * This virtual function may be overriden in derived class
     * 
     */
    virtual void loop() {};

    //read function must be defined in derived class
    virtual int read() = 0;
    
   
    inline void debug(char* debugString) {
      Serial.println(debugString);
    };
};
#endif

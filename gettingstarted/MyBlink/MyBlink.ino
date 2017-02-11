/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
  
  modified 8 Sep 2016
  by Colby Newman
*/


const int SECOND = 1000;
int code = 0;  // default blink code

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Enter a number. LED will blink n times followed by 5sec delay");
}

// the loop function runs over and over again forever
void loop() {

  

  if ( Serial.available() ) {
    
    char ch = Serial.read();

    // 0-9
    if ( ch >= '0' && ch <= '9' ) {

      // new blink code
      code = ch - '0';   // ASCII number to int
      Serial.print("Received ");
      Serial.println(code);
      
    } else {
      
      Serial.print("Not a number: ");
      Serial.println(ch);
      
    }
  }

  /*
   * Some blinks followed by a delay
   */
  ledBlinkCode(code);

}


/*
 * led on then led off
 */
void ledOneCycle(int onTime, int offTime) {
  
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(onTime);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(offTime);   
}


void ledBlinkCode(int blinkCount){

  for (int i=0; i< blinkCount; i++){
    ledOneCycle(500, 500);
  }

  delay(5 * SECOND);
}


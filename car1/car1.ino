// blue car kit;  take 1

//#include <HCSR04.h>        // <> means look in standard lib location.  Using " to look locally
const int SONAR_RANGE_MAX_CM = 400; // max dist that HCSR04 can report, in cm
#include <NewPing.h>                // HC-SR04 improved, see https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home
#include <stdio.h>                  // for function sprintf
#include <TM1637Display.h>          //4-digit display
#include <L298N.h>

char msg[80];
// int motor_post_op_delay_ms = 1500; // after motor does an operation, delay this long to let the operation take place (why? could this be 0?)
// int motor_post_op_delay_ms = 100; // after motor does an operation, delay this long to let the operation take place (why? could this be 0?)
int motor_post_op_delay_ms = 1000; // after motor does an operation, delay this long to let the operation take place (why? could this be 0?)
int speed = 100;                   // of 250 max
char cmd = ' ';                    //command
int clearance = 0;                 // available range in front of car; in cm

const char CMD_NOP = ' ';

// Pins
const int HCSR04_PIN_TRIG = 7;
const int HCSR04_PIN_ECHO = 8;
const int DIGIT4_PIN_DIO = 9;
const int DIGIT4_PIN_CLOCK = 10;

// motor pins
const int L298N_PIN_BIN1 = 12; //purple   // digital
const int L298N_PIN_BIN2 = 13; //grey   // digital
const int L298N_PIN_BPWM = 3;  //blue   // pwm
const int L298N_PIN_AIN1 = 4;  //white   // digital
const int L298N_PIN_AIN2 = 2;  //black   // digital
const int L298N_PIN_APWM = 5;  //green   // pwm      aka enable pin

// init the ultrasonic range finder
//HCSR04 *sonar = new HCSR04(HCSR04_PIN_TRIG, HCSR04_PIN_ECHO);
NewPing sonar(HCSR04_PIN_TRIG, HCSR04_PIN_ECHO, SONAR_RANGE_MAX_CM);

// init the 4-digit LCD display
TM1637Display display = TM1637Display(DIGIT4_PIN_CLOCK, DIGIT4_PIN_DIO);

// Init the motor.
// Motor direction is relative to the wheel. When wheels on both sides move in same direction, the car spins.
// To adjust for this, I swapped the placement of AIN1 and AIN2 in the constructor of L298N. Now for forward and
// backward cmds, all wheels are going the same direction.
// are moving in the same direction.\ =hpn
L298N motor = L298N(L298N_PIN_APWM, L298N_PIN_AIN2, L298N_PIN_AIN1, L298N_PIN_BIN1, L298N_PIN_BIN2, L298N_PIN_BPWM);

void setup()
{

  // initialize the HC-SR04 Ultrasonic range finder
  // sonar->setup();

  // Turn on the display. Set brightness A number from 0 (lowest brightness) to 7 (hi).
  display.setBrightness(0x04, true /*on*/);

  // We'll use the serial monitor to view the sonar output
  Serial.begin(9600);

  Serial.println("enter a direction: f, r, l, b,   any other char to stop");
}

void loop()
{

  // read the front range finder
  // clearance = rangeCorrection(sonar->read());
  // clearance = sonar->read();
  // clearance = rangeCorrection2( sonar.convert_cm(sonar.ping_median(5)));   // avg of 5 pings
  clearance = sonar.convert_cm(sonar.ping_median(5)); // avg of 5 pings
  display.showNumberDec(clearance, false /*no leading zero*/);

  // default is go
  cmd = 'f'; //go

  // check for cmd from remote controller
  if (Serial.available())
  {
    cmd = Serial.read();
    Serial.print("rx: ");
    Serial.println(cmd);
  }

  //
  // Commands generated below will override any remote cmd received
  //

  if (clearance != 0)
  {
    // emergency stop ?
    if (clearance < 5)
    {
      cmd = 's'; //stop
    }

    if (clearance < 20)
    {
      cmd = 'b'; //backup
    }

    if (clearance < 50)
    {
      cmd = 'l'; // turn left
    }
  }

  sprintf(msg, "%c  %d \n", cmd, clearance);
  Serial.print(msg);
  /*
   *   Send cmd to motors
   */
  doCommand(cmd, motor_post_op_delay_ms);

  delay(100); //ms
}

void doCommand(char cmd, int motor_delay_ms)
{

  if (cmd != CMD_NOP)
  {
    // Serial.print("cmd: ");
    // Serial.println(cmd);

    // stop befor changing direction
    // motor.full_stop(motor_post_op_delay_ms);
    switch (cmd)
    {
    case 'f':
      // Serial.println("forward");
      motor.forward(speed, motor_delay_ms);
      break;
    case 'r':
      // Serial.println("reverse");
      motor.turn_right(speed, motor_delay_ms);
      break;
    case 'l':
      // Serial.println("left");
      motor.turn_left(speed, motor_delay_ms);
      break;
    case 'b':
      // Serial.println("backward");
      motor.backward(speed, motor_delay_ms);
      break;
    case '1':
      speed = 100;
      break;
    case '2':
      speed = 150;
      break;
    case '3':
      speed = 200;
      break;
    case '4':
      speed = 250;
      break;
    case 's':
    // fall through
    default:
      // Serial.println("stop");
      motor.full_stop(motor_delay_ms);
      break;
    }
  }
}

/*
** Return adjusted range.
** Adjust under range to zero
*/
int rangeCorrection(int range)
{

  if (range = 0)
  {
    range = 999;
  }

  static int last = -1;
  if (last == -1)
  {
    last = range;
  }

  if (range >= (SONAR_RANGE_MAX_CM - 50) && last < 20)
  {
    // under range limit
    range = 0;
  }

  // if (range - last  > 10){
  //   range = last -1;
  // }

  last = range;
  return range;
}

int rangeCorrection2(int range)
{

  if (range = 0)
  {
    range = 999;
  }
}
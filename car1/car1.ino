// blue car kit;  take 1

//#include <HCSR04.h>        // <> means look in standard lib location.  Using " to look locally
const int SONAR_RANGE_MAX_CM = 400; // max dist that HCSR04 can report, in cm
#include <NewPing.h>                // HC-SR04 improved, see https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home
#include <stdio.h>                  // for function sprintf
#include <TM1637Display.h>          //4-digit display
#include <L298N.h>

const char CMD_NOP = ' ';

char msg[80];
// int motor_post_op_delay_ms = 1500; // after motor does an operation, delay this long to let the operation take place (why? could this be 0?)
// int motor_post_op_delay_ms = 100; // after motor does an operation, delay this long to let the operation take place (why? could this be 0?)
int motor_post_op_delay_ms = 1000; // after motor does an operation, delay this long to let the operation take place (why? could this be 0?)
int speed = 100;                   // of 250 max
char cmd = 'f';                    // initial command command,
int clearance = 0;                 // available range in front of car; in cm
bool isSerialActive = false;       // true if we hav received ffff (the init sequence) from the serial controller
char serialCmdRx = CMD_NOP;        // last cmd rx from serial port
int initSequencCount = 0;

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

  // Turn on the display. Set brightness A number from 0 (lowest brightness) to 7 (hi).
  display.setBrightness(0x03, true /*on*/);

  // We'll use the serial monitor to view the sonar output
  Serial.begin(9600);

  Serial.println("enter a direction: f, r, l, b,   any other char to stop. Send fff to get started.");
}

void loop()
{

  // unless we are stopped, forward is what we want
  if (cmd != 's' && cmd != 'b')
  {
    cmd = 'f';
  }

  // read the front range finder
  // clearance = rangeCorrection(sonar->read());
  // clearance = sonar->read();
  // clearance = rangeCorrection2( sonar.convert_cm(sonar.ping_median(5)));   // avg of 5 pings
  clearance = sonar.convert_cm(sonar.ping_median(5)); // avg of 5 pings
  display.showNumberDec(clearance, false /*no leading zero*/);

  // check for cmd from remote controller
  if (Serial.available())
  {
    serialCmdRx = Serial.read();
    serialCmdRx = tolower(serialCmdRx); // normalize to lower case

    Serial.print("rx: ");
    Serial.println(serialCmdRx);
    display.showNumberDec(serialCmdRx, true /*w leading zero*/);

    if (serialCmdRx == 'f')
    {
      if (++initSequencCount >= 4  && !isSerialActive)
      {
        isSerialActive = true; // we have received the init seq
        Serial.println("sync OK");
      }
    }
    else
    {
      initSequencCount = 0;
    }

    if (isSerialActive)
    {
      cmd = serialCmdRx;
    }
  }

  sprintf(msg, "cmd=%c  range=%dcm", cmd, clearance);
  Serial.println(msg);

  cmd = cmdOverride(cmd, clearance);
  /*
   *   Send cmd to motors
   */
  doCommand(cmd, motor_post_op_delay_ms);

  delay(100); //ms
}

char cmdOverride(int cmd, int clearance)
{

  //
  // Commands generated below will override any remote cmd received
  // (except stop)
  //
  if (cmd != 's' && clearance != 0)
  {
    // emergency stop ?

    if (clearance < 50)
    {
      cmd = 'l'; // turn left
    }

    if (clearance < 30)
    {
      cmd = 'b'; //backup
    }

    if (clearance < 15)
    {
      cmd = 's'; //stop
    }
  }
  return cmd;
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
    case '0':
      speed = 0;
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
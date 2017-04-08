/*
 You will need to wire up your components as such:
 
 +GND
 |__________
 |          |
 <          <
 >          >
 <          > 100 Ohm resistors
 |          |
 -----     -----  LEDs (both used as photodiode) 
  \ /       \ /   (yes wire them in reverse bias)
 -----     -----  (Green 525nm LEDs work best, yellow is ok, red is meh)
 |         |
 |         |
 |         +RIGHT_LED_PIN
 |
 +LEFT_LED_PIN
*/


#define PWR_SAVE_MODE             false
//#define Serial_DEBUG            false

#define FWD_BTN_PIN         7
#define BWD_BTN_PIN         6
#define UP_BTN_PIN          5
#define DN_BTN_PIN          4

#define POLL_WAIT_STATUS_PIN    13
#define AUX_PWR_PIN             11 // relay control pin for things like servos, lcds, backlights, etc.
#define NUM_OF_SERVOS           1

 unsigned int SensorPollingLimit        = 200;
 unsigned int SleepModeAfterMillisec    = 20000; // go into night mode after x milliseconds, and return to RestingPos
 unsigned int AdjustServosAfterMillisec = 2; 

/* IGNORE BEGIN */
struct SensorSettings {
  Servo __Servo;
  int   __ServoPos           = 0;
  int   ServoPinNumber       = 0;
  int   ServoRestingPosition = 0;
  int   ServoConstraints[2]  = {0,0};
  int   SensorPinNumbers[2]  = {0,0};
} Sensors [NUM_OF_SERVOS];
/* IGNORE END */

void setvars(void){
  Sensors[0].ServoPinNumber       = 7;
  Sensors[0].SensorPinNumbers[0]  = 6;
  Sensors[0].SensorPinNumbers[1]  = 5;
  Sensors[0].ServoRestingPosition = 90;
  Sensors[0].ServoConstraints[0]  = 0;
  Sensors[0].ServoConstraints[1]  = 180;
}



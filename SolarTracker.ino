/*
    Solar Tracker
    Copyright (C) 2013-2014  Mitchell Lafferty <coolspeedy6@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *** Config options are in Settings.h ***
 
 need to add eeprom!
 and LCDSerial code
 We sould make the code disconnect Servos if it hasn't moved for several hours
 I borked the code again, ugh :(
*/

/* DON'T TOUCH ANYTHING BELOW LOL */

void setvars(void); // ugh I hate the fact I am using a function for the vars




#include <Servo.h>
#include <Bounce.h>
#include "./Settings.h"

unsigned int  iServo                = 0;
unsigned long LastTimeWeSawTheLight = 0;
unsigned long LastTimeServosMoved   = 0;

// int NumberOfSensors = sizeof(Sensors) / sizeof(SensorSettings);

// Bounce fwdButton = Bounce(FWD_BTN_PIN, DEBOUNCE_MILLISEC);
// Bounce bwdButton = Bounce(BWD_BTN_PIN, DEBOUNCE_MILLISEC);
// Bounce upButton  = Bounce(UP_BTN_PIN, DEBOUNCE_MILLISEC);
// Bounce dnButton  = Bounce(DN_BTN_PIN, DEBOUNCE_MILLISEC);

void setup()
{

  Serial.begin(1200);

  pinMode(POLL_WAIT_STATUS_PIN, OUTPUT);
  pinMode(AUX_PWR_PIN, OUTPUT);

  setvars();

  for (unsigned int i = 0; i < NUM_OF_SERVOS; i++) {
    Sensors[i].__Servo.attach(Sensors[i].ServoPinNumber);
    Sensors[i].__Servo.write(Sensors[i].ServoRestingPosition);
  }

}

void loop()
{


  if (LastTimeWeSawTheLight && (millis() - LastTimeWeSawTheLight) >= SleepModeAfterMillisec) {
     for (unsigned int i = 0; i < NUM_OF_SERVOS; i++) {
       Sensors[i].__ServoPos = Sensors[i].ServoRestingPosition;
     }
     LastTimeWeSawTheLight = 0;
  }

  if (iServo++ >= NUM_OF_SERVOS - 1) iServo = 0;

  unsigned int iSensor = 0;

  do {
    pinMode(Sensors[iServo].SensorPinNumbers[iSensor], OUTPUT);
    digitalWrite(Sensors[iServo].SensorPinNumbers[iSensor], HIGH);

    // turns off internal pull up
    pinMode(Sensors[iServo].SensorPinNumbers[iSensor], INPUT);
    digitalWrite(Sensors[iServo].SensorPinNumbers[iSensor], LOW);

    digitalWrite(POLL_WAIT_STATUS_PIN, LOW);

    for (unsigned int iPolls = 0; iPolls <= SensorPollingLimit; iPolls++){

      // Count how long it takes the diode to bleed back down to a logic zero
      if (digitalRead(Sensors[iServo].SensorPinNumbers[iSensor]) == 0) {
          if ((millis() -  LastTimeServosMoved) >= AdjustServosAfterMillisec){
            if (iSensor) {
                if (Sensors[iServo].__ServoPos < Sensors[iServo].ServoConstraints[iSensor])
                  Sensors[iServo].__ServoPos++;
            }
            else {
                if (Sensors[iServo].__ServoPos > Sensors[iServo].ServoConstraints[iSensor])
                  Sensors[iServo].__ServoPos--;
            }
            LastTimeServosMoved   = millis();
          }
          LastTimeWeSawTheLight = millis();
          digitalWrite(POLL_WAIT_STATUS_PIN, HIGH);
          break;
      }
    }

    if (Sensors[iServo].__ServoPos != Sensors[iServo].__Servo.read()) {
      Sensors[iServo].__Servo.write(Sensors[iServo].__ServoPos);
      Serial.println(Sensors[iServo].__ServoPos);
    }
    else {
       // To avoid slowing down the Servo, "iSensor" is increased when we are done moving the Servo
      iSensor++;
    }
  }
  while (iSensor <= 1);
}

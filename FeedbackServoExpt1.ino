/* 
High Speed Continuous Rotation Servo with Feedback
This file is to test out its behavior.

Servo is powered by Arduino 5V pin.
Caution: Do not resist the servo's rotation. Stalling the servo will draw more
current from the Arduino than the USB port can provide.

Uses two buttons ("tactile switches"): one to speed up rotation and one to slow it down

Displays output from the servo's feedback pin in the serial monitor.
*/

#include <Servo.h>

Servo servo;  // create servo object to control a servo

int servoSpeed = 1500;    // variable to store the servo speed (1500 = stopped)
// Example speeds:
// 1250 = FAST clockwise; 1350 = medium clockwise; 1450 = slow clockwise; 1500 = STOP
// Speeds between 1520 and 1750 would be counter-clockwise

int speedUpPin = 3;    // Pushbutton: one terminal connected to pin 3; the other to GND
int slowDownPin = 2;   // Pushbutton:  "     "         "      "  "  2;  "    "    "  "
int feedbackPin = 5;
int feedback = 0;   // pulse time in microseconds from servo Hall sensor

void setup() {
  Serial.begin(115200);
  servo.attach(9);  // attaches the servo motor on pin 9 to the servo object
  pinMode(speedUpPin, INPUT_PULLUP);
  pinMode(slowDownPin, INPUT_PULLUP);
  pinMode(feedbackPin, INPUT);
}

void loop() {
  if (digitalRead(speedUpPin) == LOW) {
    servoSpeed -= 10;
    Serial.print("Servo speed: ");
    Serial.println(servoSpeed);
    servo.writeMicroseconds(servoSpeed);
    delay(100);
  }
  else if (digitalRead(slowDownPin) == LOW) {
    servoSpeed += 10;
    Serial.print("Servo speed: ");
    Serial.println(servoSpeed);
    servo.writeMicroseconds(servoSpeed);
    delay(100);
  }
  
  // Position information of the motor is given using a repeated 3.3 Volt pulse. The length of time
  // that the pulse is at 3V before returning to 0V (ground) indicates what angle the servo
  // is pointing.
  //
  // An angle closer to 0 degrees has shorter pulses:
  //
  //    ____        ____        ____          3.3 Volts
  //    |  |        |  |        |  |
  // ___|  |________|  |________|  |________  Ground
  //
  //
  //
  // An angle closer to 359 degrees has longer pulses:
  //
  //    __________  __________  __________
  //    |        |  |        |  |        |
  // ___|        |__|        |__|        |___
  //

  feedback = pulseIn(feedbackPin, HIGH);  // waits for the voltage to rise and then measures
                                          // the time in microseconds that the voltage remains high
  Serial.print("Microseconds: ");
  Serial.println(feedback);
  delay(100);
}

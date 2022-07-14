/* 
 * Used with classroom set of Feedback Servos connected to rotating platforms (records).
 * Powered from 10 - 14 Volts DC bench supply to banana connectors. Board equipped with
 * Adafruit Metro Mini, LCD display, and two pushbuttons.
 * 
 * NOTE: This version relies on different pin connections. It is
 * made for use with the first set of purple custom circuit boards. Add a jumper
 * wire on the bottom connecting pin 13 to pin 4. Pin 13 should
 * be disconnected from the Metro Mini (remove socket pin)
 *  
High Speed Continuous Rotation Servo with Feedback
This program is made for a rotational motion physics lab. The servo is attached to 
a record. One button speeds up the rotation; the other slows it down.
Possible experiments:
• Various materials can be put on the record to measure coefficients of friction.
• Identical objects can be placed at different distances from axis. Which will slide
off first as rotational speed is increased?
• Put marble next to an incline. At what speed will ball roll up the incline?
• Put marble in a bottle cap with opening facing the center of the record. Speed up
the rotation. Tilt the spinning record up to a vertical plane. Slow down the record
until the ball drops out? At what rotational speed should this occur?

Sean Fottrell
11/28/2021
*/

#include <Servo.h>
#include <LiquidCrystal.h>

Servo servo;  // create servo object to control a servo

const int rs = 6, en = 7, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int servoSpeed = 1485;    // variable to store the servo speed (1480 - 1520 = stopped)
int speedUpPin = 8;
int slowDownPin = 4;
int feedbackPin = 5;
int x1 = 0;   // pulse time in microseconds from servo Hall sensor
int x2 = 0;
int time1 = 0;
int time2 = 0;
float rps = 0;
float rotations = 0;
int deltaT = 0;

void setup() {
  Serial.begin(115200);
  servo.attach(3);  // attaches the servo on pin 3 to the servo object
  pinMode(speedUpPin, INPUT_PULLUP);
  pinMode(slowDownPin, INPUT_PULLUP);
  pinMode(feedbackPin, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop() {
  lcd.clear();
  lcd.print(" Accel    Slow");
  lcd.setCursor(0, 1);  // set the cursor to column 0, line 1
  lcd.print("RPS: ");
  lcd.print(rps);
//  lcd.print("  ");
//  lcd.print(servoSpeed);

  if (digitalRead(speedUpPin) == LOW)
  {
    servoSpeed -= 2;
  }
  else if (digitalRead(slowDownPin) == LOW)
  {
    servoSpeed += 2;
  }
  if (servoSpeed >= 1485)
  {
    servoSpeed = 1485;
  }
  else if (servoSpeed <= 1250)
  {
    servoSpeed = 1250;
  }
  servo.writeMicroseconds(servoSpeed);

  // Measure rotational speed
  x1 = pulseIn(feedbackPin, HIGH);
  time1 = millis();
  delay(200);
  x2 = pulseIn(feedbackPin, HIGH);
  time2 = millis();

  if (x2 + 50 < x1)  // detect when rotational position rolls over from max value to min value
  {                  // (The "50" allows for some measurement imprecision)
    x1 = x1 - 1035;  // I think one full revolutions is 94.2% of 1099 microseconds pulse time
  }

  rotations = (x2 - x1) / 1035.0;
  deltaT = time2 - time1;
  rps = 1000 * rotations / deltaT;  // multiply by 1000 to compensate for deltaT in milliseconds
}

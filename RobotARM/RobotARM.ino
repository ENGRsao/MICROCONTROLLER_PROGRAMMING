/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo baseServo, handleServo, wingServo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
    // attaches the servo on pin 9 to the servo object
  baseServo.attach(3); handleServo.attach(5);
  wingServo.attach(6);
Serial.begin(9600);
}

void loop() {


    
    baseServo.write(60); //clockwise
    delay(2000);
    baseServo.write(90); //stop clockwise movement
  Serial.println(3);
  delay(2000);
  baseServo.write(00); //stop anticlockwise movement
  Serial.println(3);
  delay(2000);
 baseServo.write(120);  //anticlockwise
  Serial.println(4);
  delay(2000);
  baseServo.write(0); //stop  anticlockwise movememt
  delay(2000);
  baseServo.write(60);
  delay(2000);
    baseServo.write(90); //stop clockwise movement
  Serial.println(1);
  delay(5000);
// handleServo.write(60);
  //wingServo.write(30);
  
}

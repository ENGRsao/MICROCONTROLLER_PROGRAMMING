
#include <Servo.h>

Servo myservo1,myservo2,myservo3,myservo4;  


void setup() {
     myservo1.attach(3); 
     myservo2.attach(5); 
    myservo3.attach(6); 
    myservo4.attach(9); 
    // armin motor
    myservo1.write(10);
    myservo2.write(10);
    myservo3.write(10);
    myservo4.write(10);
    delay(100);
}

void loop() {
     myservo1.write(80);
    myservo2.write(80);
    myservo3.write(80);
   myservo4.write(80);
}



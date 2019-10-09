int trigpin = 2;
int echopin= 3;
int echoval;
void setup() {
  // put your setup code here, to run once:
pinMode(echopin,INPUT);

pinMode(trigpin,OUTPUT);
 Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 digitalWrite(trigpin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin,HIGH);
    delayMicroseconds(5);
    digitalWrite(trigpin,LOW);
    delayMicroseconds(2);

       echoval = pulseIn(echopin,HIGH);
       int distance = echoval/60;
       Serial.print(distance);
      Serial.println("\t centimeter");
      delay(1000);
}

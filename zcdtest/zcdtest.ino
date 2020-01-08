long period, powerFactor;
 bool count ;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(13,OUTPUT);
attachInterrupt(0,voltageZCD,RISING);
attachInterrupt(1,currentZCD,RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);
}

void voltageZCD()
{
  count = !count;
  if (count)
    period = millis();
  else
  {
    period = millis() - period;
    Serial.println(1000/period);
  }
    
}
void currentZCD()
{
  
}


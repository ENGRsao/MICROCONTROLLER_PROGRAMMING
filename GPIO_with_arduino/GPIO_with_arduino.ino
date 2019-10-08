int ledpinblue = 10;
int ledpinreen= 9;
int button4 = 4;
int button4state;
int button3 = 1;
int button3state;
int pot = A0;
int button = 0;
int potvalue;
void setup() {
  // put your setup code here, to run once:
pinMode(ledpinblue,OUTPUT);
pinMode(ledpinreen, OUTPUT);
pinMode(button, OUTPUT);
pinMode( button3, INPUT);
pinMode(pot,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
button4state = digitalRead(button4);
potvalue =  analogRead(pot);
analogWrite(button,map(potvalue,0,1023,0,255));
   if (button4state == HIGH)
    {  
      digitalWrite(ledpinblue,HIGH);
    }
    else
    {
        digitalWrite(ledpinblue,LOW);
    }
button3state = digitalRead(button3);
if (button3state == HIGH)
{     
  digitalWrite(ledpinreen,HIGH);
}
  else
  {
    digitalWrite(ledpinreen,LOW);
  }    
}


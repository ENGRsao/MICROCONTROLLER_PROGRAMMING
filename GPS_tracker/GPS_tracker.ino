
#include "tracker_system.h"

//int data_available;String answer="";int x = 0;int networkstatus=0;String data = "";int m=0;String val= "";


long timeout = 5000;
int voltageval= A0;
int cutoff = 12;
float val_b, battery_volt,error=0.06;

void setup() {
   // put your setup code here, to run once:
     analogReference(EXTERNAL); //set Areff as reference
      Serial.begin(4800); //baud rate
      sim.begin(4800);
      gps.begin(9600);
      pinMode(cutoff,OUTPUT);
      pinMode(voltageval,INPUT);
      delay(200);
      sim.listen();
      delay(1);
      sim.write("AT\r\n");
      delay(10);
      simSetup();  //settting up sim module
      delay(100);
      establishGPRS_conn(); //establishing GPRS connection 
      delay(100);
      openTCP_connection(); //open TCP for Data communication
      delay(200);
      mqtt_connect(); //connect to mqqt server
      delay(300); 
      gps.listen();    
      
}
unsigned long previous_time = millis();  
 
void loop() {
  // put your main code here, to run repeatedly:    
      //GPSdata();
      unsigned long current_time = millis();
      val_b = analogRead(voltageval);
      battery_volt = ((val_b * 3.2227) / 1024);
       Serial.println(battery_volt);
       if (battery_volt >=  2.08)
       {
        digitalWrite(cutoff,HIGH);
       }
       else
       {
        digitalWrite(cutoff,LOW);
       }
       delay(1000);
      if ((unsigned long)(current_time - previous_time) >= timeout)
    {     
      String  data="";
      Serial.println("Read data");
        gps.listen();
       while (!(gps.find("$GPRMC,")))
      {
      }
         data += gps.readStringUntil('$');
         Serial.println(data);     
          String a = GPSdata(data)  ;   //format data 
           publish(a);      
           previous_time = millis();            
     }
}


#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>

SoftwareSerial SIM800L(3, 4);
const int rs = 11, en = 10, d4 = 9, d5 = 8, d6 = 7, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int led = 5;

char minValue1[5], maxValue1[5], sum1[5], average1[5], minValue2[5], maxValue2[5],  sum2[5], average2[5], minValue3[5], maxValue3[5], sum3[5], average3[5];

uint16_t minVal1, maxVal1, ave1,  minVal2, maxVal2, ave2,  minVal3, maxVal3, ave3;
uint32_t  summer1, summer2, summer3;

bool dataAvailable  =  false;
char dump[10];
const char* server = "api.thingspeak.com";
String apiWritekey = "EHHNME4EM4FNJLO";// replace with your THINGSPEAK WRITEAPI key here
char *recipient_number  = "08037893725";
int _SIM800L_status = 0, xmit = 0;
String SIM800L_apn = "web.mtnnigeria.com", port_no = "80", ip_address = "api.thingspeak.com", API_KEY = "EHHNME4EM4FNJLOT", field = "field1=",reply;
char serialData[150];
long previousMillis, currentMillis;
char *data;
uint8_t error_val;uint8_t i = 1; bool callibratingFlag = false, sendFlag =  false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SIM800L.begin(9600);
  SIM800L.write("AT\r\n");
  delay(3000);
  Serial.println(data_received());

  SIM800L.write("AT+CSCS=\"GSM\"");
  delay(200);
  Serial.println(data_received());
  SIM800L.write("AT+CFUN?");
  delay(200);
  Serial.println(data_received());
  SIM800L.write("AT+CREG = 1\r\n");
  delay(1000);
  Serial.println(data_received());
  pinMode(led, OUTPUT);
  lcd.begin(20, 4);
  digitalWrite(led, HIGH);
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("STRUCTURE HEALTH");
  lcd.setCursor(2, 4);
  lcd.print("MONITORING");
  delay(5000);
  Homepage();
  Serial.println("started:");
  // sendSMS("Hi Good Morning.... here we are again");
}

void loop() {
  // put your main code here, to run repeatedly:

    if (xmit == 0)
        { SIM800L_debug();  }

//    if (sendFlag)
//    {
//      printData();
//      SIM800L_debug();
//      transportData();
//      delay(1000);
//      SIM800L_debug();
//      i++;
//      delay(1000);
////      if(i > 2) {
////         printData();
////         //saveToEEPROM();
////         // readEEPROM();
//       
////        i = 1;
//        sendFlag = false;
//      //}
//    //  previousMillis = millis();
//    }

  if (Serial.available() > 0)
  {
     for (int j = 0; j < 150; j++)       serialData[j]  = '\0';
    int i = 0; 
    while (Serial.available() > 0)
    {
      serialData[i++] = char(Serial.read());
      delay(5);
    }
       Serial.println(serialData);

    if (serialData[0] == 'c')
    {
      Serial.println("sending message");
       lcd.clear();
       lcd.setCursor(2, 0);
       lcd.print("Callibrating Device");
       callibratingFlag = true;
      //sendSMS("Hi Good Morning.... here we are again");
    }
    else
    {
        dataAvailable = true;
    }
     
  }
  if (dataAvailable)
  {
//   minVal1 = 10; maxVal1 = 29; summer1 = 32; ave1 = 45;  minVal2 = 50; maxVal2 = 70; summer2 = 65; ave2 = 87;  minVal3 = 45; maxVal3 = 65; summer3 = 99; ave3 = 78, error_val = 89;
//       delay(1000);
//      SIM800L_debug();
//    transportData();
   
    String D;
    int k[14], j = 0;
    for (int i = 0; i < strlen(serialData); i++)
    {
      if (serialData[i] != ';')
      {
        D += serialData[i];
      }
      else
      {
        k[j] = D.toInt();
        Serial.println(k[j++]);
         D = "";
        if (j > 14) break;       
      }
    }
    Serial.print("done");
   // Serial.flush();
    minVal1 = k[0]; maxVal1 = k[1]; summer1 = k[2]; ave1 = abs(k[3]);  minVal2 = k[4]; maxVal2 = k[5]; summer2 = k[6]; ave2 = abs(k[7]);  minVal3 = k[8]; maxVal3 = k[9]; summer3 = k[10]; ave3 = abs(k[11]), error_val = k[12];
 SIM800L_debug();
    if (callibratingFlag)
    {
      saveToEEPROM();
      Homepage();
      printData();
      callibratingFlag = false;
    }
    else
    {
       printData();
      int l = 0;
//       previousMillis = millis();
//       while (l < 7) 
       SIM800L_debug();
             while (i <3){
             if (xmit == 1)
             {
              transportData();
              break;
             }
             else
             {
               SIM800L_debug();
               i++;
             }
             }
//             while ((millis() - previousMillis) < 10000)  {}
//                    previousMillis = millis();
     //    }
//         if ((error_val) > 0x01)
//         {
//          sendSMS();
//         }
         
       }
       dataAvailable = false;
    } 
}



void transportData()
{

  uint8_t systemStatus = error_val;
  sendMultiple_Data(API_KEY, "field1=", String(minVal1),"field2=", String(minVal2),"field3=", String(minVal3),"field4=", String(maxVal1),"field5=", String(maxVal2),"field6=", String(maxVal3),"field7=", String(systemStatus));
//  switch (field_val)
//  {
//    case 1: sendData(API_KEY, "field1=", String(minVal1)); break;
//    case 2: sendData(API_KEY, "field2=", String(minVal2)); break;
//    case 3: sendData(API_KEY, "field3=", String(minVal3)); break;
//
//    case 4: sendData(API_KEY, "field4=", String(maxVal1)); break;
//    case 5: sendData(API_KEY, "field5=", String(maxVal2)); break;
//    case 6: sendData(API_KEY, "field6=", String(maxVal3)); break;
//
//    case 7: sendData(API_KEY, "field7=", String(systemStatus)); break;
//    default: break;
//  }
  delay(1000);
}

void sendMultiple_Data(String channel_key, String field1, String message1,String field2, String message2,String field3, String message3,String field4, String message4,String field5, String message5,String field6, String message6,String field7, String message7)
{
//  int retry=0;
//  while (retry < 3)
//  {
      SIM800L.write("AT+CIPSEND\r\n");
      delay(100);
      String result_d = data_received();
      Serial.println(result_d);
      
//      if(result_d.indexOf('>'))
//      {
          SIM800L.write("GET https://api.thingspeak.com/update?api_key="); SIM800L.print(channel_key); 
          SIM800L.write("&"); SIM800L.print(field1); SIM800L.print(message1); 
          SIM800L.write("&"); SIM800L.print(field2); SIM800L.print(message2);
          SIM800L.write("&"); SIM800L.print(field3); SIM800L.print(message3);
          SIM800L.write("&"); SIM800L.print(field4); SIM800L.print(message4);
          SIM800L.write("&"); SIM800L.print(field5); SIM800L.print(message5);
          SIM800L.write("&"); SIM800L.print(field6); SIM800L.print(message6);
          SIM800L.write("&"); SIM800L.print(field7); SIM800L.print(message7);
          SIM800L.write("\r\n");
          delay(100);
          SIM800L.write(0x1A);
          delay(500);
          Serial.println(data_received());
//          break;
//      }
//      else
//    {
//         delay(1000);
//          SIM800L_debug();
//          retry++;
//    }
//  }
//
//  Serial.println(data_received());
}

void readEEPROM()
{
  minVal1 = (EEPROM.read(0) << 8 | EEPROM.read(1));
  maxVal1 = (EEPROM.read(2) << 8 | EEPROM.read(3));
  ave1 = (EEPROM.read(4) << 8 | EEPROM.read(5));

  minVal2 = (EEPROM.read(6) << 8 | EEPROM.read(7));
  maxVal2 = (EEPROM.read(8) << 8 | EEPROM.read(9));
  ave2 = (EEPROM.read(10) << 8 | EEPROM.read(11));

  minVal1 = (EEPROM.read(12) << 8 | EEPROM.read(13));
  maxVal1 = (EEPROM.read(14) << 8 | EEPROM.read(15));
  ave1 = (EEPROM.read(16) << 8 | EEPROM.read(17));
  sprintf(serialData, "%d %d %d %d %d %d %d %d %d", minVal1, maxVal1, ave1, minVal2, maxVal2, ave2, minVal3, maxVal3, ave3);
  Serial.print(serialData);
}
void saveToEEPROM()
{
  EEPROM.write(0, minVal1 >> 8);
  EEPROM.write(1, minVal1 & 0xFF);
  EEPROM.write(2, maxVal1 >> 8);
  EEPROM.write(3, maxVal1 & 0xFF);
  EEPROM.write(4, ave1 >> 8);
  EEPROM.write(5, ave1 & 0xFF);

  EEPROM.write(6, minVal2 >> 8);
  EEPROM.write(7, minVal2 & 0xFF);
  EEPROM.write(8, maxVal2 >> 8);
  EEPROM.write(9, maxVal2 & 0xFF);
  EEPROM.write(10, ave2 >> 8);
  EEPROM.write(11, ave2 & 0xFF);

  EEPROM.write(12, minVal3 >> 8);
  EEPROM.write(13, minVal3 & 0xFF);
  EEPROM.write(14, maxVal3 >> 8);
  EEPROM.write(15, maxVal3 & 0xFF);
  EEPROM.write(16, ave3 >> 8);
  EEPROM.write(17, ave3 & 0xFF);
}

void Homepage()
{
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Amp");
  lcd.setCursor(6, 0);
  lcd.print("ToF");
  lcd.setCursor(11, 0);
  lcd.print("Ave");
  lcd.setCursor(16, 0);
  lcd.print("Corr");
}

void printData()
{
  sprintf(minValue1, "%d", minVal1);
  sprintf(maxValue1, "%d", maxVal1);
  sprintf(sum1, "%d", summer1);
  sprintf(average1, "%d", ave1);

  sprintf(minValue2, "%d", minVal2);
  sprintf(maxValue2, "%d", maxVal2);
  sprintf(sum2, "%d", summer2);
  sprintf(average2, "%d", ave2);

  sprintf(minValue3, "%d", minVal3);
  sprintf(maxValue3, "%d", maxVal3);
  sprintf(sum3, "%d", summer3);
  sprintf(average3, "%d", ave3);

  lcd.setCursor(1, 1);
  lcd.print("     ");
  lcd.setCursor(1, 1);
  lcd.print(minValue1);
  lcd.setCursor(6, 1);
  lcd.print("     ");
  lcd.setCursor(6, 1);
  lcd.print(maxValue1);
  lcd.setCursor(11, 1);
  lcd.print("     ");
  lcd.setCursor(11, 1);
  lcd.print(sum1);
  lcd.setCursor(16, 1);
  lcd.print("     ");
  lcd.setCursor(16, 1);
  lcd.print(average1);


  lcd.setCursor(1, 2);
  lcd.print("     ");
  lcd.setCursor(1, 2);
  lcd.print(minValue2);
  lcd.setCursor(6, 2);
  lcd.print("     ");
  lcd.setCursor(6, 2);
  lcd.print(maxValue2);
  lcd.setCursor(11, 2);
  lcd.print("     ");
  lcd.setCursor(11, 2);
  lcd.print(sum2);
  lcd.setCursor(16, 2);
  lcd.print("     ");
  lcd.setCursor(16, 2);
  lcd.print(average2);


  lcd.setCursor(1, 3);
  lcd.print("     ");
  lcd.setCursor(1, 3);
  lcd.print(minValue3);
  lcd.setCursor(6, 3);
  lcd.print("     ");
  lcd.setCursor(6, 3);
  lcd.print(maxValue3);
  lcd.setCursor(11, 3);
  lcd.print("     ");
  lcd.setCursor(11, 3);
  lcd.print(sum3);
  lcd.setCursor(16, 3);
  lcd.print("     ");
  lcd.setCursor(16, 3);
  lcd.print(average3);

}

void confirmSIM800L()
{
  SIM800L.listen();
  delay(10);
  SIM800L.write("AT\r\n");
  delay(50);
  while (1)
  {
    if (SIM800L.find("OK"))
    {
      break;
    }
    Serial.println("NO SIM800L");
    //    blink_led(led);
    SIM800L.write("AT\r\n");
    delay(200);
  }
}

void shutConnection()
{
  SIM800L.write("AT+CIPSHUT\r\n");
  delay(1000);
  Serial.println(data_received());
}

void setConnectiontype()
{
  SIM800L.write("AT+CIPMUX=0\r\n");
  delay(1000);
  Serial.println(data_received());
  SIM800L.write("AT+CGATT=1\r\n");
  delay(400);
  Serial.println(data_received());
  SIM800L.write("AT+CSTT=\""); SIM800L.print(SIM800L_apn); SIM800L.write("\",\"\",\"\"\r\n");
  delay(1000);
  Serial.println(data_received());
}

void startWireless()
{
  SIM800L.write("AT+CIICR\r\n");
  delay(1000);
  Serial.println(data_received());

}

void startTCP()
{
  SIM800L.write("AT+CIPSTART=\"TCP\",\""); SIM800L.print(ip_address); SIM800L.write("\",\""); SIM800L.print(port_no); SIM800L.write("\"\r\n");
  delay(1000);
  Serial.println(data_received());
}

void stopTCP()
{
  SIM800L.write("AT+CIPCLOSE\r\n");
  delay(500);
  Serial.println(data_received());
}

void sendData(String channel_key, String field, String message)
{
  SIM800L.write("AT+CIPSEND\r\n");
  delay(100);
  Serial.println(data_received());
  SIM800L.write("GET https://api.thingspeak.com/update?api_key="); SIM800L.print(channel_key); SIM800L.write("&"); SIM800L.print(field); SIM800L.print(message); SIM800L.write("\r\n");
  delay(100);
  SIM800L.write(0x1A);
  delay(500);

  Serial.println(data_received());
}

void SIM800L_status()
{
  SIM800L.listen();
  delay(10);
  SIM800L.write("AT+CIPSTATUS\r\n");
  delay(500);
  Serial.println(data_received());
  if (check_response("IP INITIAL", reply) == 1)
  {
    _SIM800L_status = 0;
  } else if (check_response("IP START", reply) == 1)
  {
    _SIM800L_status = 1;
  } else if (check_response("IP CONFIG", reply) == 1)
  {
    _SIM800L_status = 2;
  } else if (check_response("IP GPRSACT", reply) == 1)
  {
    _SIM800L_status = 3;
  } else if (check_response("IP STATUS", reply) == 1)
  {
    _SIM800L_status = 4;
  } else if (check_response("TCP CONNECTING", reply) == 1)
  {
    _SIM800L_status = 5;
  } else if (check_response("CONNECT OK", reply) == 1)
  {
    _SIM800L_status = 6;
  } else if (check_response("TCP CLOSING", reply) == 1)
  {
    _SIM800L_status = 7;
  } else if (check_response("TCP CLOSED", reply) == 1)
  {
    _SIM800L_status = 8;
  } else if (check_response("PDP DEACT", reply) == 1)
  {
    _SIM800L_status = 9;
  }
}

//------------------------------------------------------------------------------------------
void SIM800L_debug()
{
  SIM800L.listen();
  delay(10);

  SIM800L_status();

  if (_SIM800L_status == 0) // status:IP INITIAL. Shut IP. Set APN.
  {
    shutConnection();
    setConnectiontype();
    SIM800L_status();
    xmit = 0;
    digitalWrite(led, HIGH);
  }
  if (_SIM800L_status == 1) // status:IP START. Activate GPRS connection
  {
    xmit = 0;
    startWireless();
    SIM800L_status();
    digitalWrite(led, HIGH);
  }
  if (_SIM800L_status == 3) // status:IP GPRSACT. Get IP
  {
    xmit = 0;
    SIM800L_ip();
    SIM800L_status();
    digitalWrite(led, HIGH);
  }
  if (_SIM800L_status == 4) // status:IP STATUS. Start TCP connection
  {
    xmit = 0;
    startTCP();
    SIM800L_status();
    digitalWrite(led, HIGH);
  }
  if (_SIM800L_status == 5) // status:TCP CONNECTING. Wait for TCP connection
  {
    xmit = 0;
    digitalWrite(led, HIGH);
  }
  if (_SIM800L_status == 6) // status:CONNECT OK. Set transmit flag
  {
    if (xmit == 0)
    {
      // startTCP();
      xmit = 1;
    }
    //   blink_led(led);
  }
  if (_SIM800L_status == 7) // status:TCP CLOSING. Clear transmit flag.
  {
    xmit = 0;
    SIM800L_status();
    digitalWrite(led, HIGH);
  }
  if (_SIM800L_status == 8) // status:TCP CLOSED. Restart TCP connection.
  {
    xmit = 0;
    startTCP();
    digitalWrite(led, HIGH);
  }
  if (_SIM800L_status == 9) // status:PDP DEACT. Shut IP.
  {
    xmit = 0;
    shutConnection();
    digitalWrite(led, HIGH);
  }
}

void SIM800L_ip()
{
  SIM800L.write("AT+CIFSR\r\n");
  delay(1000);
  Serial.println(data_received());
}

String data_received()
{
  reply = ""; char data = "";
  SIM800L.listen();
  delay(10);
  while (SIM800L.available() > 0)
  {
    data = SIM800L.read();
    reply += data;
  }
  return reply;
}

short check_response(String query, String source)
{
  short stat = 0;
  int query_len = query.length();

  for (int i = 0; i < (source.length()); i++)
  {
    if (source.substring(i, i + query_len) == query)
    {
      stat = 1;
    }
  }
  return stat;
}

void clear_buf()
{
  delay(1000);
  while (SIM800L.available() > 0)
  {
    char a = SIM800L.read();
  }

}

void sendSMS()
{
  char mes[40];
  sprintf(mes,"The Structure Datum are %d,%d,%d,%d,%d,%d", maxVal1,ave1, maxVal2,ave2, maxVal3,ave3);
   SIM800L.write("AT+CREG?\r\n");
  delay(1000);
  Serial.println(data_received());
  SIM800L.println("AT+CMGF=1");
  delay(500);
  clear_buf();
  Serial.println(data_received());
  SIM800L.print(F("AT+CMGS="));
  SIM800L.print("\"");
  SIM800L.print(recipient_number);
  SIM800L.println("\"");
  delay(1000);
  long previousMillis  = millis();
  while (!SIM800L.find(">"))
  {
    if ((millis() - previousMillis) > 30000)
    {
      break;
    }
  }
  //String SMS = "How are you?";
  // Serial.println(data_received());
  SIM800L.print(mes);
  SIM800L.write(26);
  delay(1000);
  Serial.println(data_received());
}

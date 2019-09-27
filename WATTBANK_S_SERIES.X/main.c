/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F26K42
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "WS2812.h"
#include "characters.h"
#include "enumeration.h"
#include "lcd.h"
#include <string.h>
//%macro definition

//INPUT Pins
#define BUTTON_PRESSED PORTCbits.RC7
#define OVERLOAD_SENSE PORTAbits.RA1
#define GRID_AVAILABLE PORTAbits.RA3

#define GATEWAY_SEND(sysemDATA) UART2_Send_words(sysemDATA)

//OUTPUT PARAMETERS
#define BUZZER LATCbits.LATC3
#define LCD_BACKLIGHT LATBbits.LATB6
#define BOOST LATAbits.LATA0
#define FAN LATCbits.LATC1

//special lcd characters
#define battery_state1 0x00
#define battery_state2 0x01
#define SOLAR_CHARGE 0x02
#define GRID 0x03
#define WARN 0x04
#define CHAR_TIME 0x05
#define CHAR_HOUSE 0x06
#define PHOTIZZO_LOGO 0x07
#define BATTERY_CRITICAL_LOW 29.40
#define BATTERY_CRITICAL_CHARGE 50.20


//led animation sequence
#define INCREASE 0
#define DECREASE 1

//Temperature and system constant
#define NUMBER_OF_LED 3
#define TEMP_LIMIT1 1600
#define TEMP_LIMIT2 1700
#define TEMP_LIMIT3 1900
#define TEMP_LIMIT4 2000
#define UNSAFE_TEMPERATURE 2400
#define MINIMUM_CHARGING_CURRENT 1.00
#define OVERLOAD_RATING 1000.00
#define MAX_CURRENT 26.50

//LCD control parameters
#define LCD_ON 0x01A8  //85% duty cycle
#define LCD_OFF 0x00

//SENSING paramters
#define BATTERY_VOLTAGE_REFERENCE 0.0157 //(0.0139715) //Nadc * (5/4096) * (141/10)(0.017202)//(0.0141517)
#define CURRENT_SENSITIVITY 0.025 //(0.030518) //(1/32.768{40mV/A) 1/40
//#define CHARGING_CURRENT_REFERENCE 2500
//#define LOAD_CURRENT_REFERENCE 2490
#define TEMP_REFERENCE 0.122
#define AC_VOLTAGE_REFERENCE 0.0894 //0.1074
#define ENERGY_REFERENCE 0.00138889 ///Nacd * 3/3600 ///1000

//bms bit
#define status_bit_0 (Ba3Voltage[5] & 0x01)
#define status_bit_1  ((Ba3Voltage[5]>>1) & 0x01)
#define status_bit_2  ((Ba3Voltage[5]>>2) & 0x01)
#define status_bit_3  ((Ba3Voltage[5]>>3) & 0x01)
#define status_bit_4  ((Ba3Voltage[5]>>4) & 0x01)
#define status_bit_5  ((Ba3Voltage[5]>>5) & 0x01)
#define status_bit_6  ((Ba3Voltage[5]>>6) & 0x01)
#define status_bit_7  ((Ba3Voltage[5]>>7) & 0x01)
/*
                         Main application
 */

//modes of the systems
system_mode_t system_mode;
system_subMode_t system_subMode;
battery_status_t batterystatus;
warning_status_t warningstatus;
error_status_t errorstatus;
power_mode power_source;
fan_mode_t fan_mode;
bms_status_t bms_status;
bms_error_status_t bms_error_status;
charging_control charging_control_status;
bms_diagnostic_data_type bms_diagnostic_data;

//LED CONTROL COLOUR VARIABLE
uint8_t blue[3] = {20,0,64};
uint8_t green[3] = {64,0,0};
uint8_t red[3] = {0,64,0};
uint8_t off[3] = {0,0,0};
uint8_t orange[3] = {42,64,0};
uint8_t r = 0, g = 0, b = 0;

                uint16_t cell0; uint8_t *cell0PTR = &cell0;
                uint16_t cell1; uint8_t *cell1PTR = &cell1;
                uint16_t cell2; uint8_t *cell2PTR = &cell2;
                uint16_t cell3; uint8_t *cell3PTR = &cell3;
                uint16_t cell4; uint8_t *cell4PTR = &cell4;
                uint16_t cell5; uint8_t *cell5PTR = &cell5;
                uint16_t cell6; uint8_t *cell6PTR = &cell6;  
                uint16_t cell7; uint8_t *cell7PTR = &cell7;
                uint16_t cell8; uint8_t *cell8PTR = &cell8;
                uint16_t cell9; uint8_t *cell9PTR = &cell9;
                uint16_t cell10; uint8_t *cell10PTR = &cell10;
                uint16_t cell11; uint8_t *cell11PTR = &cell11;
                uint16_t batteryTemperature1; uint8_t *batteryTemperature1PTR = &batteryTemperature1;
                uint16_t batteryTemperature2; uint8_t *batteryTemperature2PTR = &batteryTemperature2;
                uint16_t ACS_offset; uint8_t *ACS_offsetPTR = &ACS_offset;
                uint32_t columbReference; uint8_t *columbReferencePTR = &columbReference;
                uint16_t chargeCurrent; uint8_t *chargeCurrentPTR = &chargeCurrent;
                uint16_t dischargeCurrent;  uint8_t *dischargeCurrentPTR = &dischargeCurrent;
                uint32_t columb_count_val;
                uint8_t *columb_countPTR =  &columb_count_val;
//time variables
volatile uint8_t hour = 0, minutes = 0, seconds = 0;

 //flags and parameters 
bool powerState = false;
bool updateMeasurement = false;
bool battery_flag = false;
bool overload_flag = false;
bool gateway_post_data = false;
bool diagnozeMode = false;
bool timer_tick;
//measurement variables
float chargingCurrent, loadCurrent, energyconsumed, loadPower,batteryVoltage ;
float CHARGING_CURRENT_REFERENCE = 2500.00, LOAD_CURRENT_REFERENCE = 2490.00;

char batteryPercentage[4];
uint32_t columb_count;
char columb_count_char[12];
uint8_t measurement = 0;
uint16_t systemData[8];
uint16_t boostTemperature, chargerTemperature, wattbankTemperature, outputV;
uint8_t Ba3Voltage[46],batteryV; 
uint8_t batterystate = 0;
uint8_t battery_oldState = 6;
bool updateTime =  false;
uint8_t homepageUpdate = 0;
uint8_t timeleft_hour, timeleft_minutes;
char timelefthrSTR[3] = "00",timeleftminSTR[3] = "00";
bool ledtoggle;
char GatewayData[75];
char energyConsumption[10], outputVoltage[4],loadOutput[9],systemTemperature[4],solarPower[8],batteryPower[8];
char loadPower_buff[9];
float charging_power;
bool activateOverload = false;
bool stopCharging =  false;

//function definition
void chargeControl();
void lcd_off_clear();
void BMS_error_check(void);
void BMS_CALIBRATION_PAGE(void);
void systemControl();
void convert2actualVal();
void controlChargingStatus();
void errorPage(uint8_t error);
void warningPage(uint8_t warning);
void setGridMode();
void defaultHomePage();
void offChargingPage();
void welcomePage();
void ResetAllParameters();
char* print_float(float val);
void DIAGNOSTIC_DATA();
void updateSystemMode();
static void errorCheck();
static void warningCheck();
void clearUART_buffer(uint8_t number);
void updateSystemData();
void energyCalculation();
void ButtonScan();
void shutDOWN();
void LCD_initialization();
uint16_t takeSystemMeasurement(uint8_t channel);
void CreateCustomCharacters();
void batteryBar();  
void Send_GatewayDATA();
void updateBatteryBAR(bool status, uint8_t range);
void set_on_time();
void percentageUpdate(uint8_t batteryvalue);
void gridPage();
void  requestBatteryPercentage();
void takeAllSystemMeasurement();
void setONparameters();
void homePage1(void);
void homePage2(void);
void WS2812_writeColour(uint8_t* colour);
void WS2812_writeRGB(uint8_t G,uint8_t R, uint8_t B);
void animateLED();
void fan_Control();
void listen4data();
void charging_status();
void measureDATA();
void BMS_DATA();
void BMSerrorPage(uint8_t error);
void timeleft(void);
void updateHomepage(uint8_t val);

void setDiagnosticMode();
void DIAGNOSTIC_DATA();
void diagnosticPage1();
void diagnosticPage2();
char* printlong(uint32_t d);
char* printData(uint16_t d);
void welcomeDiagnostic(void);


uint8_t rainbowColour_sequence[] = {0,0,0,0,0,0,0,64,0,42,64,0,64,64,0,64,0,0,0,0,64,0,32,64,35,64,64,0,
									0,0,0,0,0,};
bool rainbow_flag;
void check_charging();
void rainbowPowerUp(uint8_t colour_No);
void gridCheck();
void callibrateACS();
void controlChargingStatus();
/*
                         Main application
 */

 uint8_t diagnozepageUpdate = 0;
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    BOOST = 1;
    WWDT_SoftDisable();
    T0CON0bits.T0EN = 0;
    Ba3Voltage[0] = 100;
    system_mode = SYSTEM_OFF;
    system_subMode = SYSTEM_NOT_CHARGING;
    warningstatus = NORMAL_WARN;
    errorstatus = NORMAL_ERROR; 
	power_source = WATTBANK;
    batterystatus =  BAT_NORM;
    bms_error_status = BMS_NORMAL;
    charging_control_status = CHARGE_CONTROL_NORMAL;
	bms_status.byte_data = 0x00;
    
	ledtoggle = INCREASE;
	fan_mode = FAN_OFF;
	PWM2_LoadDutyValue(fan_mode);
	CCP2CONbits.CCP2EN = 0;
    TMR1_StopTimer();
	//initialize LCD
    LCD_initialization();  
    WS2812_writeColour(off);
    PWM1_LoadDutyValue(LCD_ON);
    CCP1CONbits.CCP1EN = 1;
    welcomePage();
    callibrateACS();
    measureDATA();    
    ResetAllParameters();
    fan_Control();
    PWM1_LoadDutyValue(LCD_OFF);
    CCP1CONbits.CCP1EN = 0;
    WS2812_writeColour( off);	
	T0CON0bits.T0EN = 1;
    INTERRUPT_GlobalInterruptEnable();
    WWDT_SoftEnable();
    PORTAbits.RA1 = 0;
   
    while (1)
    {
        CLRWDT();
        // Add your application code
        if ((system_mode == SYSTEM_ON)) 
        {
		  setONparameters();
          defaultHomePage();
		  rainbow_flag =  true;
		  int rainbow_val = 0;
          sprintf(loadPower_buff,"%4.2fW",0.00);
		  homePage1();         
          while (system_mode == SYSTEM_ON) 
          {
              CLRWDT();
			  if (rainbow_flag)
			  {
				  rainbowPowerUp(rainbow_val);
				  rainbow_val += 3;
                  __delay_ms(100);
			  if(rainbow_val > 24) {rainbow_flag = false; WS2812_writeColour(blue);}
			  }
			  measureDATA();
              fan_Control();
              systemControl();
                if (system_mode != SYSTEM_ON) break;
			  warningCheck(); 
              fan_Control();
              ButtonScan();
			    if (system_mode != SYSTEM_ON) break;   
		      //errorCheck();
              check_charging();
			  gridCheck();  
              controlChargingStatus();
			  
			if (updateMeasurement)
			{  // if (!activateOverload) {activateOverload = true; PIE0bits.IOCIE = 1; }
                
				homepageUpdate++;
				updateHomepage(homepageUpdate);  
                energyCalculation();
				updateMeasurement = false;
			} 				
              
			if ((power_source == GRID_POWER) && (system_mode == SYSTEM_ON))
			{
				setGridMode();
				gridPage();
                /*PWM1_LoadDutyValue(LCD_OFF);
                CCP1CONbits.CCP1EN = 0;
                lcd_off_clear();*/
                //activateOverload = false; PIE0bits.IOCIE = 0; 
				ledtoggle = INCREASE;
				g = 0;
                 fan_mode = FAN_100;
                 PWM2_LoadDutyValue(fan_mode);
                 CCP2CONbits.CCP2EN = 1;
				while ((power_source == GRID_POWER) && (system_mode == SYSTEM_ON))
				{		
                    CLRWDT();
					//errorCheck();
					if (updateMeasurement) { updateMeasurement = false;}//gridPage();lcd_off_clear();
                    measureDATA();
					check_charging();
					systemControl();
						if (system_mode != SYSTEM_ON) break;
				   warningCheck(); 
                   chargeControl();
                   controlChargingStatus();
					ButtonScan();
						if (system_mode != SYSTEM_ON) break;						 
					animateLED(); 
					gridCheck();
				}
				break;
			}
		  }
		}
		
        
        if (system_mode == SYSTEM_OFF)
        {
		  Lcd_Clear();
		  PWM1_LoadDutyValue(LCD_OFF);
          CCP1CONbits.CCP1EN = 0;    
          //activateOverload = false; PIE0bits.IOCIE = 0;           
          while ((system_mode == SYSTEM_OFF))
          {     
              CLRWDT();
              //warningCheck(); 
              ButtonScan();
                if (system_mode != SYSTEM_OFF) break;
              listen4data();
              measureDATA();
              fan_Control();
              gridCheck();
              chargeControl();
              controlChargingStatus();
              //errorCheck();
			  if ((system_subMode == SYSTEM_CHARGING))
              {				  
				  offChargingPage();
                  /*lcd_off_clear();
                  PWM1_LoadDutyValue(LCD_OFF);
                  CCP1CONbits.CCP1EN = 0;*/
				  ledtoggle = INCREASE;
				  g = 0;
                  fan_mode = FAN_100;
                  PWM2_LoadDutyValue(fan_mode);
                  CCP2CONbits.CCP2EN = 1;
				  while (system_subMode == SYSTEM_CHARGING)
				  {
                      CLRWDT();
                    warningCheck();
                    systemControl();                    
                      if (system_mode != SYSTEM_OFF) break;
					ButtonScan();
                        if (system_mode != SYSTEM_OFF) break;	
					animateLED();
                   charging_status();
                   chargeControl();
					if (updateMeasurement){ updateMeasurement = false;} //offChargingPage();lcd_off_clear();
					measureDATA();
                    gridCheck();
                    controlChargingStatus();
				  }    				  
                  WS2812_writeColour(off);
                  PWM1_LoadDutyValue(LCD_OFF);
                  CCP1CONbits.CCP1EN = 0;
				  Lcd_Clear();
              }			 
          }
        }
		
	if (system_mode == CALIBRATION)
		{			
			BMS_CALIBRATION_PAGE();
			ledtoggle = INCREASE;
			g = 0;
            //activateOverload = false; PIE0bits.IOCIE = 0; 
            fan_mode = FAN_100;
            PWM2_LoadDutyValue(fan_mode);
            CCP2CONbits.CCP2EN = 1;
			while (system_mode == CALIBRATION)
			{
                CLRWDT();
                gridCheck();
				measureDATA();
				systemControl();
                check_charging();
			    animateLED();	
			}
		}        
   
    if (system_mode == DIAGNOSTIC)
    {
        welcomeDiagnostic();
        setDiagnosticMode();
        PORTAbits.RA1 = 1; 
        diagnosticPage2();
         CLRWDT();
        while (system_mode == DIAGNOSTIC)
        {
             CLRWDT();
            ButtonScan();
            DIAGNOSTIC_DATA();
            
            if (updateMeasurement)
			{  
				diagnozepageUpdate++;
				if (diagnozepageUpdate < 3)
                    {
                        diagnosticPage2();
                    }
                    else if ((diagnozepageUpdate >= 3) && (diagnozepageUpdate <= 11))
                    {
                        diagnosticPage1();
                        if (diagnozepageUpdate >= 11) diagnozepageUpdate = 0;
                    }  
				updateMeasurement = false;
			} 
        }
      }
    }
}

//functions details

void controlChargingStatus()
{
    
        if (charging_control_status == CHARGE_CONTROL_THERMAL)
        {
            if (chargerTemperature < (UNSAFE_TEMPERATURE - 300))  {PORTAbits.RA1 = 0;  charging_control_status = CHARGE_CONTROL_NORMAL;} 
        }
        /*else if (charging_control_status == CHARGE_CONTROL_OVERCHARGED)
        {
         if (batteryVoltage < 49.4) {PORTAbits.RA1 = 0;  charging_control_status = CHARGE_CONTROL_NORMAL;}   
        }    */     
        
         else{}
}
void chargeControl()
{
	/*if (batteryVoltage >= BATTERY_CRITICAL_CHARGE)
	{
		 PORTAbits.RA1 = 1; 	 
         charging_control_status = CHARGE_CONTROL_OVERCHARGED;
	}*/
	 if (chargerTemperature >= UNSAFE_TEMPERATURE)
	{
		PORTAbits.RA1 = 1; 
        if (charging_control_status != CHARGE_CONTROL_THERMAL)
        {
            charging_control_status = CHARGE_CONTROL_THERMAL;
            warningstatus = THERMAL_SHUTDOWN; 
        }
	}
}


void callibrateACS()
{
    uint16_t ACS_value[2];int i = 1;
    for (i= 1; i < 3; i++)
    {
        uint32_t accumulator = 0;
        int j = 0;
        for (j = 0 ; j < 16 ; j++)
        {
            accumulator += takeSystemMeasurement(i);
            __delay_ms(2);
        }
        ACS_value[i-1] = accumulator >> 4; 
        ADCC_StartConversion(channel_VSS);
        while(!ADCC_IsConversionDone()){CLRWDT();}
    }
    CHARGING_CURRENT_REFERENCE = ACS_value[1];
    LOAD_CURRENT_REFERENCE = ACS_value[0];
}

void gridCheck()
{
    if (PORTAbits.RA3)
      {
        power_source = GRID_POWER;
      }
    else 
    {
        power_source = WATTBANK;
    }
}

void rainbowPowerUp(uint8_t colour_No)
 {
	 uint8_t rainbowColour[9];
     int i = 0;
	 for (i = 0; i < 9; i++)
	 {
		rainbowColour[i] = rainbowColour_sequence[i+colour_No]; 
	 }
	WS2812_Write(NUMBER_OF_LED,rainbowColour);
 }
 
 
void check_charging()
{
	if ((system_subMode == SYSTEM_CHARGING))
     {
           Lcd_Set_Cursor(1,14);
           Lcd_Write_Char(GRID);
     }
     else 
       {
          Lcd_Set_Cursor(1,14);
          Lcd_Write_String(" ");
       }
}

void Send_GatewayDATA()
{
    sprintf(GatewayData,"%2.3f,%2.3f,%2.3f,%i,%s,%i,%lu,%i,%i,%i,%i,%i,%i,%i,%i",loadCurrent,chargingCurrent,batteryVoltage,outputV,energyConsumption,Ba3Voltage[0],columb_count,hour,minutes,Ba3Voltage[6],Ba3Voltage[7],power_source,system_mode,system_subMode,warningstatus);
    GATEWAY_SEND(GatewayData);
 }         
         
void measureDATA()
{
    takeAllSystemMeasurement();  
    convert2actualVal();
	if ((chargingCurrent > MINIMUM_CHARGING_CURRENT)&&(chargingCurrent > loadCurrent))
	{
		system_subMode = SYSTEM_CHARGING;
	}
	else
	{		
		system_subMode = SYSTEM_NOT_CHARGING;
	}
    
    if (gateway_post_data)
    {
        gateway_post_data = false;
        Send_GatewayDATA();        
    }
}

void listen4data()
{
	 if(UART1_is_rx_ready() > 0)
    {
		int i = 0;
		
		for (i = 0; i < 46; i++) Ba3Voltage[i] = '\0';
		i = 0;
        uint8_t *columbPTR;
		while (UART1_is_rx_ready() > 0)
		{
			Ba3Voltage[i++] = UART1_Read();
            __delay_ms(10);
            
			 if (i > 46)break;
		}
        columbPTR = &columb_count;
        columbPTR[0] = Ba3Voltage[1];
        columbPTR[1] = Ba3Voltage[2];
        columbPTR[2] = Ba3Voltage[3];
        columbPTR[3] = Ba3Voltage[4]; 
		
		//BMS_error_check();
		timeleft();		
	}
	else{}
}

void timeleft(void)
{
	timeleft_hour = Ba3Voltage[6];
	timeleft_minutes = Ba3Voltage[7];
	
	(timeleft_hour < 10) ? sprintf(timelefthrSTR,"0%i",timeleft_hour) : sprintf(timelefthrSTR,"%i",timeleft_hour);
	
	(timeleft_minutes < 10) ? sprintf(timeleftminSTR,"0%i",timeleft_minutes) : sprintf(timeleftminSTR,"%i",timeleft_minutes);	
}

 void set_on_time()
{
    char hours_s[3], minutes_s[3], seconds_s[3];
	
	(hour < 10) ? sprintf(hours_s,"0%i",hour): sprintf(hours_s,"%i",hour);

	(minutes < 10)?	sprintf(minutes_s,"0%i",minutes):sprintf(minutes_s,"%i",minutes);
	
	/*if (seconds < 10)
		sprintf(seconds_s,"0%i",seconds);
	else{
		sprintf(seconds_s,"%i",seconds);
	}*/
   Lcd_Set_Cursor(3,11);
   Lcd_Write_String(hours_s);
   Lcd_Write_String(":");
   Lcd_Set_Cursor(3,14);
   Lcd_Write_String(minutes_s);
   //Lcd_Write_String(":");
   //Lcd_Set_Cursor(3,8);
   //Lcd_Write_String(seconds_s);
}

//system Control control
void fan_Control()
{
	if ((boostTemperature >= UNSAFE_TEMPERATURE))
    {
        warningstatus = THERMAL_SHUTDOWN;       
        fan_mode = FAN_100;
		PWM2_LoadDutyValue(fan_mode);
		CCP2CONbits.CCP2EN = 1;	
    }
	else if ((boostTemperature >= TEMP_LIMIT4) || (chargerTemperature >= TEMP_LIMIT4))
    {
        fan_mode = FAN_100;
		PWM2_LoadDutyValue(fan_mode);
		CCP2CONbits.CCP2EN = 1;		
        PORTAbits.RA1 = 0;
    }
	else if ((boostTemperature >= TEMP_LIMIT3) || (chargerTemperature >= TEMP_LIMIT3))
    {
        fan_mode = FAN_75;
		PWM2_LoadDutyValue(fan_mode);
		CCP2CONbits.CCP2EN = 1;	
        PORTAbits.RA1 = 0;
    }
	else if ((boostTemperature >= TEMP_LIMIT2) || (chargerTemperature >= TEMP_LIMIT2))
    {
        fan_mode = FAN_50;
		PWM2_LoadDutyValue(fan_mode);
		CCP2CONbits.CCP2EN = 1;	
        PORTAbits.RA1 = 0;
    }
	else if ((boostTemperature >= TEMP_LIMIT1)  || (chargerTemperature >= TEMP_LIMIT1))
    {
        fan_mode = FAN_25;
		PWM2_LoadDutyValue(fan_mode);
		CCP2CONbits.CCP2EN = 1;	
        PORTAbits.RA1 = 0;
    }
    else
    {
        fan_mode = FAN_OFF;
		PWM2_LoadDutyValue(0);
		CCP2CONbits.CCP2EN = 0;	
        PORTAbits.RA1 = 0;
	}
}


void BMS_DATA()
{
	 if(UART1_is_rx_ready() > 0)
    {
		int i = 0;
		
		for (i = 0; i < 46; i++) Ba3Voltage[i] = '\0';
		i = 0;
        uint8_t *columbPTR;
		while (UART1_is_rx_ready() > 0)
		{
			Ba3Voltage[i++] = UART1_Read();
            __delay_ms(10);            
			
			if (i > 46)break;
		}
        columbPTR = &columb_count;
		
        percentageUpdate(Ba3Voltage[0]);
        batteryBar();
        columbPTR[0] = Ba3Voltage[1];
        columbPTR[1] = Ba3Voltage[2];
        columbPTR[2] = Ba3Voltage[3];
        columbPTR[3] = Ba3Voltage[4]; 
        
		//bms_status.byte_data = Ba3Voltage[5];
        timeleft();
		
         if ((Ba3Voltage[5] & 0x01) || (batteryVoltage < BATTERY_CRITICAL_LOW))
		{			
			batterystatus = LOW_BATTERY;
		}
         else if(status_bit_1)
		{
			batterystatus = BATTERY_FULL;
		} 
         else
         {
             batterystatus = BAT_NORM;
         }
        
		if (system_mode == CALIBRATION)
		{
			
			sprintf(columb_count_char,"%luC",columb_count);
			//BMS_CALIBRATION_PAGE();
			Lcd_Set_Cursor(3,3);
			Lcd_Write_String("             ");
			Lcd_Set_Cursor(3,3);
			Lcd_Write_String(columb_count_char);
            if (status_bit_3 == 0)
            {
				clearUART_buffer(2);
                system_mode = SYSTEM_OFF;
                ResetAllParameters();
            }
		}
		else if (status_bit_3)
		{
            shutDOWN();			
		}
		else {}
    }
}

void BMS_error_check(void)
{
    if (status_bit_4)
    {
        bms_error_status = SHORT_CIRCUIT;
    }
    else if (status_bit_5)
    {
        bms_error_status = OVER_TEMPERATURE;
    }
    else if (status_bit_6)
    {
        bms_error_status = OVER_CURRENT;
    }
    else if (status_bit_7)
    {
        bms_error_status = BMS_FAIL_ERROR;
    }
    else{
		bms_error_status = BMS_NORMAL;
	}
	
	if (bms_error_status != BMS_NORMAL)
	{
		BMSerrorPage(bms_error_status);
	}
}

void lcd_off_clear()
 {
   Lcd_Init();
    Lcd_Clear();   
 }
void systemControl()
{
    BMS_DATA();	 
	
    if ((batterystatus == LOW_BATTERY) && (system_subMode != SYSTEM_CHARGING) && (system_mode == SYSTEM_ON))
    {
       warningstatus = LOW_BATTERY_WARNING;
       BUZZER = 1;
       return;
    }
	else {}
    if (loadCurrent >= MAX_CURRENT)
   {
        __delay_ms(200);
        takeAllSystemMeasurement();
        convert2actualVal();   
        
        if (loadCurrent >= MAX_CURRENT)
        {
            BOOST = 1;
            warningstatus = OVERLOAD;
            BUZZER = 1;
             return;
        }
   }
    
    if (loadPower > OVERLOAD_RATING)
	{
        __delay_ms(200);
        takeAllSystemMeasurement();
        convert2actualVal();
        
        if (loadPower > OVERLOAD_RATING)
        {
            BOOST = 1;
            warningstatus = OVERLOAD;
            BUZZER = 1;
             return;
        }
	}  
	else{}	
}

//battery Bar and percentage update
void batteryBar()
{
	 uint8_t battery_newState;
	if (Ba3Voltage[0] > 0 && Ba3Voltage[0] < 26)
	{
		battery_newState = 1;		
	}
	else if (Ba3Voltage[0] >= 26 && Ba3Voltage[0] < 51)
	{
		battery_newState = 2;
	}
	else if (Ba3Voltage[0] >= 50 && Ba3Voltage[0] < 75)
	{
		battery_newState = 3;
	}
	else if (Ba3Voltage[0] >= 75 && Ba3Voltage[0] < 100)
	{
		battery_newState = 4;
	}
	else if (Ba3Voltage[0]  == 100){
		battery_newState = 5;		
	}
	else{
		battery_newState = 0;
	}
		battery_oldState = battery_newState;
		battery_flag = !battery_flag; 
		updateBatteryBAR(battery_flag,battery_oldState);	
}

void updateBatteryBAR(bool status, uint8_t range)
{
	switch (range)
	{
		case 0:
			if (status)
				LCD_CreateCharacter(battery_state2,battery_empty);
			else
				LCD_CreateCharacter(battery_state1,battery_empty);
			break;
		case 1:
			if (status)
				LCD_CreateCharacter(battery_state2,battery_25);
			else
				LCD_CreateCharacter(battery_state1,battery_25);
			break;
		case 2:
			if (status)
				LCD_CreateCharacter(battery_state2,battery_50);
			else
				LCD_CreateCharacter(battery_state1,battery_50);
			break;
		case 3:
			if (status)
				LCD_CreateCharacter(battery_state2,battery_75);
			else
				LCD_CreateCharacter(battery_state1,battery_75);
			break;
		case 4:
			if (status)
				LCD_CreateCharacter(battery_state2,battery_100);
			else
				LCD_CreateCharacter(battery_state1,battery_100);
			break;
		case 5:
			if (status)
				LCD_CreateCharacter(battery_state2,battery_100);
			else
				LCD_CreateCharacter(battery_state1,battery_100);
			break;
		default:
			break;
	}
	if (status)
		batterystate = battery_state2;
	else 
		batterystate = battery_state1;
  
   if((system_mode == SYSTEM_ON)  || (system_mode == CALIBRATION) || ((GRID_AVAILABLE) && (system_mode != SYSTEM_OFF)))
	{
		Lcd_Set_Cursor(1,15);
		Lcd_Write_String(" ");
		Lcd_Set_Cursor(1,15);
		Lcd_Write_Char(batterystate);
	}
	else{
		Lcd_Set_Cursor(3,11);
		Lcd_Write_String(" ");
		Lcd_Set_Cursor(3,11);
		Lcd_Write_Char(batterystate);
	}
}

void percentageUpdate(uint8_t batteryvalue)
{
	sprintf (batteryPercentage, "%i%%",batteryvalue);
	
		if((system_mode == SYSTEM_ON)  || (system_mode == CALIBRATION) || ((GRID_AVAILABLE) && (system_mode != SYSTEM_OFF)))
		{
			Lcd_Set_Cursor(1,16);
			Lcd_Write_String("     ");
			Lcd_Set_Cursor(1,16);
			Lcd_Write_String(batteryPercentage);
		}
		else{
			Lcd_Set_Cursor(3,12);
			Lcd_Write_String("     ");
			Lcd_Set_Cursor(3,12);
			Lcd_Write_String(batteryPercentage);
		}
}

//system measurement and update
char* print_float(float val)
{
    char s1[8];
    int w = (int)val;
    int decimal = (int)((val - w) * 100);
       sprintf(s1,"%d.%dW", w,decimal);
    return s1;
}

void energyCalculation()
    {
       float energy = (loadPower)* ENERGY_REFERENCE; 
       energyconsumed += energy;
    }

void convert2actualVal()
{
    float bat_P ;
    loadCurrent = (LOAD_CURRENT_REFERENCE - ((float)systemData[0])) * CURRENT_SENSITIVITY;
    chargingCurrent = (CHARGING_CURRENT_REFERENCE - ((float)systemData[1])) * CURRENT_SENSITIVITY;
    batteryVoltage = ((float)systemData[2]) * BATTERY_VOLTAGE_REFERENCE; 
    outputV = (int)(((float)systemData[3]) * AC_VOLTAGE_REFERENCE);
    boostTemperature = systemData[4];
    chargerTemperature = systemData[5];    
    //wattbankTemperature =  systemData[6]; 
  
	if (loadCurrent < 1.00) loadCurrent = 0.0;
    
    if(chargingCurrent < 0.750) chargingCurrent = 0.0;
        
    charging_power = (chargingCurrent*((float)batteryVoltage));	
    loadPower = loadCurrent*((float)batteryVoltage);
    
	
    (loadPower >= charging_power) ? bat_P = (loadPower - charging_power): bat_P = 0.00;
    	
	//convert data to string format
   //sprintf(systemTemperature,"%iC",wattbankTemperature);   
   sprintf(outputVoltage,"%iV",outputV);       
   sprintf(loadPower_buff,"%4.2fW",loadPower);
   sprintf(batteryPower,"%4.2fW",bat_P);
   
   (energyconsumed >= 1000) ? sprintf(energyConsumption,"%3.2fkWH",(energyconsumed/1000.00)): sprintf(energyConsumption,"%3.2fWH",energyconsumed); 
    
   
   /*  //debugging sequence    
   printf("%4.2f,",energy);
   printf("%4.2f,",energyConsumption);
   printf("%s,",outputVoltage);
   printf("%s,",energyConsumption);
   printf("%s,",loadPower_buff);
   printf("%s,",print_float(charging_power));
   printf("%s,",solarPower);
   printf("%s,",batteryPower);*/   
}

void animateLED()
{
	r = 0; b = 0;
	WS2812_writeRGB(g,r,b);
	if (ledtoggle == INCREASE)
	{
		g++;
		if (g == 64) ledtoggle = DECREASE;
	}
	else{
		g--;
		if(g == 0)	ledtoggle = INCREASE;
	}
    __delay_ms(100);
}


void takeAllSystemMeasurement()
{
    int i = 1;
    for (i = 1; i < 7;i++)
    {
		int j = 0;
		uint32_t accumulator = 0;		
		for (j = 0 ; j < 16 ; j++)
		{
			accumulator += takeSystemMeasurement(i);
			__delay_ms(2);			
			//ADCC_GetSingleConversion(channel_VSS);
		}
			systemData[i-1] = accumulator >> 4;    
			ADCC_StartConversion(channel_VSS);
            while(!ADCC_IsConversionDone()){CLRWDT();}
            //printf("%i,",systemData[i-1]);
    }
}

uint16_t takeSystemMeasurement(uint8_t channel)
{
    uint8_t channel_t;
    switch (channel)
    {
        case 1:
            channel_t = LOAD_CURRENT;
            break;
        case 2:
            channel_t = SOLAR_CURRENT;
            break;
        case 3:
            channel_t = BATTERY;
            break;
        case 4:
            channel_t = OUTPUT_VOLTAGE;
            break;
        case 5:
            channel_t = TEMP2;
            break;
        case 6:
            channel_t = TEMP1;
            break;        
        default:
                break;             
    }
	
    ADCC_StartConversion(channel_t);
    while(!ADCC_IsConversionDone()){CLRWDT();}
    return ADCC_GetConversionResult();
}

//mode settings of the system
void setONparameters()
 {
	LCD_initialization();    
	hour = 0; minutes = 0; seconds = 0; 
    defaultHomePage();
    batteryBar();
	percentageUpdate(Ba3Voltage[0]);  
    WS2812_writeColour(blue);
    BOOST = 0;	
    PWM1_LoadDutyValue(LCD_ON);
    CCP1CONbits.CCP1EN = 1;
    T0CON0bits.T0EN = 1;    
 }
 
 void updateHomepage(uint8_t val)
	{
		if (homepageUpdate < 7)
		{
			homePage1();
			check_charging();
		}
		else if ((homepageUpdate >= 7) && (homepageUpdate <= 11))
		{
			homePage2();
			check_charging();
			if (homepageUpdate >= 11) homepageUpdate = 0;
		}
	}

void setGridMode()
{    
    Lcd_Clear();
    BUZZER = 0;
    //T0CON0bits.T0EN = 0;
    hour = 0;
    minutes = 0; 
    seconds =  0;
    energyconsumed = 0;
	warningstatus = NORMAL_WARN;
    errorstatus = NORMAL_ERROR;
	T0CON0bits.T0EN = 1;    
}

void shutDOWN()
{
	Lcd_Clear();
    powerState = false;
    BUZZER = 0;
    BOOST = 1;
    T0CON0bits.T0EN = 1;
    hour = 0;
    minutes = 0; 
    seconds =  0;
    energyconsumed = 0;
	warningstatus = NORMAL_WARN;
    errorstatus = NORMAL_ERROR;	
    WS2812_writeColour( blue);
    system_mode = CALIBRATION;
}

void ResetAllParameters()
{
    Lcd_Clear();
    powerState = false;
    BUZZER = 0;
    BOOST = 1;
    PWM1_LoadDutyValue(LCD_OFF);
    CCP1CONbits.CCP1EN = 0;
    system_mode = SYSTEM_OFF;
    hour = 0;
    minutes = 0; 
    seconds =  0;
    energyconsumed = 0;
    bms_error_status = BMS_NORMAL;
	warningstatus = NORMAL_WARN;
    errorstatus = NORMAL_ERROR;
    WS2812_writeColour(off);
}

//Warning and error checking functions
static void errorCheck()
{
    if (GRID_AVAILABLE)
    {
		__delay_ms(300);		
        if ((system_subMode == SYSTEM_NOT_CHARGING) && (batterystatus != BATTERY_FULL) && (status_bit_2 != 1))
        {            
            errorstatus = PLUG_IN_NOT_CHARGING;
			errorPage(PLUG_IN_NOT_CHARGING);
        }
    }
    
   /* else if (powerState)
    {
        if (outputV < 50)
        {
          errorstatus = NO_OUTPUT_VOLTAGE;
		  errorPage( NO_OUTPUT_VOLTAGE);
        }
    }  */
    else{}
}

static void warningCheck()
{
    
    if (warningstatus == OVERLOAD)
    {
         warningPage(OVERLOAD);
    }
    else if (warningstatus == LOW_BATTERY_WARNING)
    {
        warningPage(LOW_BATTERY_WARNING);
    }
    else if (warningstatus == THERMAL_SHUTDOWN)
    {
     warningPage(THERMAL_SHUTDOWN);
    }
	else{}
}

//button activities
void ButtonScan()
{
    bool update = false;
    if (!BUTTON_PRESSED)
    {
        __delay_ms(100); //switch debounce check
        if (!BUTTON_PRESSED)
        {
            powerState = !powerState;
            update = true;
            TMR1_StartTimer();
        }
        while(!BUTTON_PRESSED) {CLRWDT();}
        if (timer_tick >= 10)
        {
            UART2_Write('C');
            WS2812_writeColour(orange);
            __delay_ms(1000);
            WS2812_writeColour(off); 
            diagnozeMode = !diagnozeMode;            
            if (!diagnozeMode)
            {
                update = true;
                powerState = false;
                system_mode = SYSTEM_OFF;
                PORTAbits.RA1 = 0;
            }
            else
            {
                system_mode = DIAGNOSTIC;
                update = false;
                 
            }
              timer_tick = 0;TMR1_StopTimer();
        }       
        timer_tick = 0;TMR1_StopTimer();
    }
    
   if (UART2_is_rx_ready() > 0)  
   {
       uint8_t subscribe_data = UART2_Read();
       if (subscribe_data == '1')
       {
            powerState = true;
            update = true;
       }
       else if (subscribe_data == '0')
       {
            powerState = false;
            update = true;
       }
   }
    if ((update) && (!diagnozeMode))
    {
        if (powerState)
        {     
            //clearUART_buffer();
            BOOST = 0;    
			system_mode = SYSTEM_ON;  			 
            update = false;            
        } 
        else if (!powerState)
        {       
            system_mode = SYSTEM_OFF;
            BOOST = 1 ;
            ResetAllParameters();
            update = false;
        }        
    }
}

//pages of the software
void welcomePage(){       
    Lcd_Clear();
    Lcd_Set_Cursor(1,8);
    Lcd_Write_String("WELCOME"); 
    LCD_CreateCharacter(PHOTIZZO_LOGO,photizzo_logo); 
    __delay_ms(1500);
    Lcd_Clear();
     Lcd_Set_Cursor(1,1);
     Lcd_Write_Char(PHOTIZZO_LOGO);
    Lcd_Set_Cursor(2,2);
    Lcd_Write_String("PHOTIZZO");
    //__delay_ms(1000);
    //Lcd_Set_Cursor(3,7);
    //Lcd_Write_String("TECHNOLOGIES");
    __delay_ms(1000);
    Lcd_Set_Cursor(4,6);
    Lcd_Write_String("WATTBANK");    
    __delay_ms(2000);
    CreateCustomCharacters();
}
void clearUART_buffer(uint8_t number)
{
	if (number == 1)
	{
		if (UART1_is_rx_ready() > 0)
		{
			while (UART1_is_rx_ready() > 0)
			{
				uint8_t dump =  UART1_Read();
				__delay_ms(2);
			}
		}
	}
	else if (number == 2)
	{
		if (UART2_is_rx_ready() > 0)
		{
			while (UART2_is_rx_ready() > 0)
			{
				uint8_t dump =  UART1_Read();
				__delay_ms(2);
			}
		}
	}
}

void homePage2(void)
{
    //defaultHomePage();
	Lcd_Set_Cursor(2,1);
    Lcd_Write_String("                    ");
	Lcd_Set_Cursor(2,1);
    Lcd_Write_String("SOLAR:");
    Lcd_Set_Cursor(3,1);
    Lcd_Write_String("                    ");
    Lcd_Set_Cursor(3,1);
    Lcd_Write_String("BATTERY:");
    Lcd_Set_Cursor(4,1);
    Lcd_Write_String("                   ");
    //Lcd_Set_Cursor(4,1);
    //Lcd_Write_String("VOLT:");
    Lcd_Set_Cursor(4,1);
    Lcd_Write_String("ENERGY:");
   
	
    Lcd_Set_Cursor(2,9);
    Lcd_Write_String(print_float(charging_power));//solarPower);    
   
    Lcd_Set_Cursor(3,9);
    Lcd_Write_String(batteryPower);
	
    //Lcd_Set_Cursor(4,6);
    //Lcd_Write_String(outputVoltage);
	
	Lcd_Set_Cursor(4,9);
    Lcd_Write_String(energyConsumption);	
}

void homePage1(void)
{
    //defaultHomePage();
	Lcd_Set_Cursor(2,1);
    Lcd_Write_String("                    ");
	Lcd_Set_Cursor(2,1);
    Lcd_Write_String("LOAD:");
    Lcd_Set_Cursor(3,1);
    Lcd_Write_String("                    ");
    Lcd_Set_Cursor(3,1);
    Lcd_Write_String("RUN TIME:");
    Lcd_Set_Cursor(4,1);
    Lcd_Write_String("                    ");
    Lcd_Set_Cursor(4,1);
    Lcd_Write_String("TIME LEFT:");
	
    Lcd_Set_Cursor(2,11);
    Lcd_Write_String(loadPower_buff);
	
	set_on_time();
	
	Lcd_Set_Cursor(4,11);
    Lcd_Write_String(timelefthrSTR);
	Lcd_Write_String(":");
	Lcd_Write_String(timeleftminSTR);  
   
}

void WS2812_writeColour(uint8_t colour[])
{
    uint8_t colour2print[9];
	colour2print[0]  = colour[0]; 
    colour2print[1]  = colour[1];
    colour2print[2]  = colour[2]; 
    colour2print[3] = colour[0];
    colour2print[4] = colour[1];
    colour2print[5] = colour[2];
    colour2print[6] = colour[0];
    colour2print[7] = colour[1];
    colour2print[8] = colour[2];
    WS2812_Write(NUMBER_OF_LED,colour2print);
}

void WS2812_writeRGB(uint8_t G,uint8_t R, uint8_t B)
{
	uint8_t GRB[3];
    GRB[0] = G;
    GRB[1] = R;
    GRB[2] = B;
	WS2812_writeColour(GRB);
}

void defaultHomePage()
{
    //Lcd_Clear();
	LCD_initialization();
	
    Lcd_Set_Cursor(1,1);
    Lcd_Write_Char(PHOTIZZO_LOGO);
    batteryBar();
    percentageUpdate(Ba3Voltage[0]);  	 
    Lcd_Set_Cursor(1,4); 
    Lcd_Write_String("WATTBANK"); 
    
    /*Lcd_Set_Cursor(2,1);
    Lcd_Write_Char(CHAR_HOUSE);
    
    Lcd_Set_Cursor(3,12); 
    Lcd_Write_Char(SOLAR_CHARGE);
    
    Lcd_Set_Cursor(3,1);
    Lcd_Write_Char(CHAR_TIME);
    
    Lcd_Set_Cursor(2,12);
    Lcd_Write_Char(batterystate);
    
    Lcd_Set_Cursor(4,1);
    Lcd_Write_String("LOAD:");*/
}

void BMSerrorPage(uint8_t error)
{
    char *ERROR_STATEMENT;
    switch (error)
    {
        case SHORT_CIRCUIT:
            ERROR_STATEMENT = "ERROR BSC";
            break;
        case OVER_TEMPERATURE:
            ERROR_STATEMENT = "ERROR OVT";
            break;
        case OVER_CURRENT:
            ERROR_STATEMENT = "ERROR OVC";
            break;
		case BMS_FAIL_ERROR:
			ERROR_STATEMENT = "ERROR BFE";
            break;			
        default:
            break;
    }
    Lcd_Clear();
    Lcd_Set_Cursor(2,6);    
    Lcd_Write_String(ERROR_STATEMENT);
    Lcd_Set_Cursor(3,1);
    Lcd_Write_String("CONTACT YOUR SERVICE") ;
    Lcd_Set_Cursor(4,1);
    Lcd_Write_String("PROVIDER URGENTLY");
	WS2812_writeColour(red);
    PWM1_LoadDutyValue(LCD_ON);
    CCP1CONbits.CCP1EN = 1;
    __delay_ms(2000);
    //BUZZER = 0; 
     ResetAllParameters();
	//while(1);
}

void errorPage(uint8_t error)
{
    char *ERROR_STATEMENT;
    switch (error)
    {
        case PLUG_IN_NOT_CHARGING:
            ERROR_STATEMENT = "ERROR 499";
            break;
        case NO_OUTPUT_VOLTAGE:
            ERROR_STATEMENT = "ERROR 699";
            break;
        default:
            break;
    }
    Lcd_Clear();	
    Lcd_Set_Cursor(2,6);    
    Lcd_Write_String(ERROR_STATEMENT);
    Lcd_Set_Cursor(3,1);
    Lcd_Write_String("CONTACT YOUR SERVICE") ;
    Lcd_Set_Cursor(4,1);
    Lcd_Write_String("PROVIDER URGENTLY");
	WS2812_writeColour(red);
    PWM1_LoadDutyValue(LCD_ON);
    CCP1CONbits.CCP1EN = 1;
	while(1){CLRWDT();}
}

void warningPage(uint8_t warning)
{
    char *WARNING_STATEMENT;
    BUZZER = 1;
    PWM1_LoadDutyValue(LCD_ON);
    CCP1CONbits.CCP1EN = 1;
    switch(warning)
    {
        case  OVERLOAD:
            WARNING_STATEMENT = "OVERLOAD ALERT!!!";
            break;
        case LOW_BATTERY_WARNING:
            WARNING_STATEMENT = "LOW BATTERY ALERT";
            break;
        case THERMAL_SHUTDOWN:
            WARNING_STATEMENT = "SYSTEM HOT!!";
            break;
        default:
            break;
    }
    if (warning == THERMAL_SHUTDOWN)
    {
        Lcd_Clear();
        Lcd_Set_Cursor(1,10); 
        Lcd_Write_Char(WARN);
        Lcd_Write_Char(WARN);
        Lcd_Write_Char(WARN);
        Lcd_Set_Cursor(2,2);    
        Lcd_Write_String(WARNING_STATEMENT);
        Lcd_Set_Cursor(3,1);
        Lcd_Write_String("COOLING DOWN.....") ;
        __delay_ms(2000);
    }
    else
    {
        Lcd_Clear();
        Lcd_Set_Cursor(1,10); 
        Lcd_Write_Char(WARN);
        Lcd_Write_Char(WARN);
        Lcd_Write_Char(WARN);
        Lcd_Set_Cursor(2,2);    
        Lcd_Write_String(WARNING_STATEMENT);
        Lcd_Set_Cursor(3,1);
        Lcd_Write_String("SHUTTING DOWN......") ;
    }
	__delay_ms(2000);
    BUZZER = 0; 
    WS2812_writeColour( off);
     ResetAllParameters();
}

void charging_status()
{
   if (batterystatus == BATTERY_FULL)
    {
      Lcd_Set_Cursor(2,4); 
      Lcd_Write_String("SYSTEM CHARGED");
    }
    else
    {
        Lcd_Set_Cursor(2,4);
       Lcd_Write_String("SYSTEM CHARGING"); 
    }  
}
void offChargingPage()
{
	LCD_initialization();
    
    Lcd_Set_Cursor(3,10); 
    Lcd_Write_Char(GRID);
    Lcd_Set_Cursor(3,11); 
    batteryBar();
    percentageUpdate(Ba3Voltage[0]);
    if (batterystatus == BATTERY_FULL)
    {
      Lcd_Set_Cursor(2,4); 
      Lcd_Write_String("SYSTEM CHARGED");
    }
    else
    {
        Lcd_Set_Cursor(2,4);
       Lcd_Write_String("SYSTEM CHARGING"); 
    }
    
	T0CON0bits.T0EN = 1;
    PWM1_LoadDutyValue(LCD_ON);
    CCP1CONbits.CCP1EN = 1;    
}
 
void gridPage()
{
	LCD_initialization();
	defaultHomePage();	
    Lcd_Set_Cursor(3,1); 
    Lcd_Write_String("                    ");
    Lcd_Set_Cursor(3,6); 
    Lcd_Write_String("UPS MODE");
    PWM1_LoadDutyValue(LCD_ON);
    CCP1CONbits.CCP1EN = 1;
}

void BMS_CALIBRATION_PAGE(void)
{
	LCD_initialization();
	percentageUpdate(Ba3Voltage[0]);
    batteryBar();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_Char(PHOTIZZO_LOGO);
	 
    Lcd_Set_Cursor(1,4); 
    Lcd_Write_String("WATTBANK"); 
	Lcd_Set_Cursor(2,1); 
    Lcd_Write_String("SYSTEM CALIBRATING.."); 
	Lcd_Set_Cursor(3,1); 
    Lcd_Write_String("Q:"); 
	PWM1_LoadDutyValue(LCD_ON);
    CCP1CONbits.CCP1EN = 1;	
}



//lcd functions
void LCD_initialization()
 {
	 Lcd_Init();
	 CreateCustomCharacters();
	 Lcd_Clear();	 
 }

//custom characters used in the software
void CreateCustomCharacters(){
     LCD_CreateCharacter(battery_state1,battery_empty);
     LCD_CreateCharacter(battery_state2,battery_100);
     LCD_CreateCharacter(SOLAR_CHARGE,solar_house);
     LCD_CreateCharacter(GRID,grid_char);
     LCD_CreateCharacter(WARN,warning_sign);
     LCD_CreateCharacter(CHAR_TIME, char_time);
     LCD_CreateCharacter(CHAR_HOUSE, char_plug);
	 LCD_CreateCharacter(PHOTIZZO_LOGO,photizzo_logo); 
}
/**
 End of File
*/
void welcomeDiagnostic(void)
{
    BOOST = 1;
	LCD_initialization();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_Char(PHOTIZZO_LOGO);
	 
    Lcd_Set_Cursor(1,4); 
    Lcd_Write_String("WATTBANK");     
	Lcd_Set_Cursor(2,6); 
    Lcd_Write_String("WELCOME....");
	Lcd_Set_Cursor(3,1); 
    Lcd_Write_String("DIAGNOSTIC STARTED.");
	PWM1_LoadDutyValue(LCD_ON);
    CCP1CONbits.CCP1EN = 1;	
    __delay_ms(1500);
}

char* printData(uint16_t d)
{
    char result[5];
    sprintf(result,"%i",d);
    return result;
}
char* printlong(uint32_t d)
{
    char result[10];
    sprintf(result,"%lu",d);
    return result;
}

char* printByte(uint8_t d)
{
    char result[4];
    sprintf(result,"%i",d);
    return result;
}

void diagnosticPage2()
{
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_Char(PHOTIZZO_LOGO);
    //batteryBar();
    //percentageUpdate(Ba3Voltage[0]);  
	Lcd_Set_Cursor(1,16);
	Lcd_Write_String(printByte(Ba3Voltage[0]));
    Lcd_Set_Cursor(1,4); 
    Lcd_Write_String("WATTBANK"); 
    
   Lcd_Set_Cursor(2,1);
   Lcd_Write_String(printData(ACS_offset));
   Lcd_Set_Cursor(2,6);
   Lcd_Write_String(printData(chargeCurrent));
   Lcd_Set_Cursor(2,11);
   Lcd_Write_String(printData(dischargeCurrent));
   Lcd_Set_Cursor(3,1);
   Lcd_Write_String(printlong(columbReference));
    Lcd_Set_Cursor(4,1);
   Lcd_Write_String(printlong(columb_count_val));
}


void diagnosticPage1()
{
   Lcd_Clear();
   Lcd_Set_Cursor(1,1);
   Lcd_Write_String(printData(cell0));
   Lcd_Set_Cursor(1,6);
   Lcd_Write_String(printData(cell1));
   Lcd_Set_Cursor(1,11);
   Lcd_Write_String(printData(cell2));
   Lcd_Set_Cursor(1,16);
   Lcd_Write_String(printData(cell3));
   Lcd_Set_Cursor(2,1);
   Lcd_Write_String(printData(cell4));
   Lcd_Set_Cursor(2,6);
   Lcd_Write_String(printData(cell5));
   Lcd_Set_Cursor(2,11);
   Lcd_Write_String(printData(cell6));
   Lcd_Set_Cursor(2,16);
   Lcd_Write_String(printData(cell7));
   Lcd_Set_Cursor(3,1);
   Lcd_Write_String(printData(cell8));
   Lcd_Set_Cursor(3,6);
   Lcd_Write_String(printData(cell9));
   Lcd_Set_Cursor(3,11);
   Lcd_Write_String(printData(cell10));
   Lcd_Set_Cursor(3,16);
   Lcd_Write_String(printData(cell11));
   Lcd_Set_Cursor(4,1);
   Lcd_Write_String(printData(batteryTemperature1));
   Lcd_Set_Cursor(4,11);
   Lcd_Write_String(printData(batteryTemperature2));
    
}

void DIAGNOSTIC_DATA()
{
    bool update = false;
    
    
	 if(UART1_is_rx_ready() > 0)
    {
		uint8_t i = 0;
		for (i = 0; i < 46; i++) Ba3Voltage[i] = '\0';
		i = 0;
		while (UART1_is_rx_ready() > 0)
		{
            Ba3Voltage[i++] = UART1_Read();
            __delay_ms(20);  
			if (i > 46)break;
		}
        update = true;
     }
     
    if (update)
    {        
		
                columb_countPTR[0] = Ba3Voltage[1];columb_countPTR[1] = Ba3Voltage[2];columb_countPTR[2] = Ba3Voltage[3];columb_countPTR[3] = Ba3Voltage[4]; 
                cell0PTR [0] = Ba3Voltage[8]; cell0PTR[1] = Ba3Voltage[9];
                cell1PTR [0] = Ba3Voltage[10]; cell1PTR[1] = Ba3Voltage[11];
                cell2PTR [0] = Ba3Voltage[12]; cell2PTR[1] = Ba3Voltage[13];
                cell3PTR [0] = Ba3Voltage[14]; cell3PTR[1] = Ba3Voltage[15];
                cell4PTR [0] = Ba3Voltage[16]; cell4PTR[1] = Ba3Voltage[17];
                cell5PTR [0] = Ba3Voltage[18]; cell5PTR[1] = Ba3Voltage[19];
                cell6PTR [0] = Ba3Voltage[20]; cell6PTR[1] = Ba3Voltage[21];
                cell7PTR [0] = Ba3Voltage[22]; cell7PTR[1] = Ba3Voltage[23];
                cell8PTR [0] = Ba3Voltage[24]; cell8PTR[1] = Ba3Voltage[25];
                cell9PTR [0] = Ba3Voltage[26]; cell9PTR[1] = Ba3Voltage[27];
                cell10PTR [0] = Ba3Voltage[28]; cell10PTR[1] = Ba3Voltage[29];
                cell11PTR [0] = Ba3Voltage[30]; cell11PTR[1] = Ba3Voltage[31];
                
               batteryTemperature1PTR[0] = Ba3Voltage[32]; batteryTemperature1PTR[1] = Ba3Voltage[33];
               batteryTemperature2PTR[0] = Ba3Voltage[34]; batteryTemperature2PTR[1] = Ba3Voltage[35];
               ACS_offsetPTR[0] = Ba3Voltage[36]; ACS_offsetPTR[1] = Ba3Voltage[37]; 
               columbReferencePTR[0] = Ba3Voltage[38];columbReferencePTR[1] = Ba3Voltage[39];columbReferencePTR[2] = Ba3Voltage[40];columbReferencePTR[3] = Ba3Voltage[41];
               chargeCurrentPTR[0] = Ba3Voltage[42];chargeCurrentPTR[1] = Ba3Voltage[43];
               dischargeCurrentPTR[0] = Ba3Voltage[44];dischargeCurrentPTR[1] = Ba3Voltage[45];
               update = false;
    }
 }

void setDiagnosticMode()
{
    Lcd_Clear();
    powerState = false;
    BUZZER = 0;
    BOOST = 1;    
    system_mode = DIAGNOSTIC;
    T0CON0bits.T0EN = 1;
    hour = 0;
    minutes = 0; 
    seconds =  0;
    energyconsumed = 0;
    bms_error_status = BMS_NORMAL;
	warningstatus = NORMAL_WARN;
    errorstatus = NORMAL_ERROR;
    WS2812_writeColour(orange); 
    PWM1_LoadDutyValue(LCD_ON);
    CCP1CONbits.CCP1EN = 1;
}


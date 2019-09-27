/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F26K42
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.00
        MPLAB 	          :  MPLAB X 5.10	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set BOOST aliases
#define BOOST_TRIS                 TRISAbits.TRISA0
#define BOOST_LAT                  LATAbits.LATA0
#define BOOST_PORT                 PORTAbits.RA0
#define BOOST_WPU                  WPUAbits.WPUA0
#define BOOST_OD                   ODCONAbits.ODCA0
#define BOOST_ANS                  ANSELAbits.ANSELA0
#define BOOST_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define BOOST_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define BOOST_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define BOOST_GetValue()           PORTAbits.RA0
#define BOOST_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define BOOST_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define BOOST_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define BOOST_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define BOOST_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define BOOST_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define BOOST_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define BOOST_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set SHUTDOWN_CHARGER aliases
#define SHUTDOWN_CHARGER_TRIS                 TRISAbits.TRISA1
#define SHUTDOWN_CHARGER_LAT                  LATAbits.LATA1
#define SHUTDOWN_CHARGER_PORT                 PORTAbits.RA1
#define SHUTDOWN_CHARGER_WPU                  WPUAbits.WPUA1
#define SHUTDOWN_CHARGER_OD                   ODCONAbits.ODCA1
#define SHUTDOWN_CHARGER_ANS                  ANSELAbits.ANSELA1
#define SHUTDOWN_CHARGER_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define SHUTDOWN_CHARGER_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define SHUTDOWN_CHARGER_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define SHUTDOWN_CHARGER_GetValue()           PORTAbits.RA1
#define SHUTDOWN_CHARGER_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define SHUTDOWN_CHARGER_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define SHUTDOWN_CHARGER_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define SHUTDOWN_CHARGER_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define SHUTDOWN_CHARGER_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define SHUTDOWN_CHARGER_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define SHUTDOWN_CHARGER_SetAnalogMode()      do { ANSELAbits.ANSELA1 = 1; } while(0)
#define SHUTDOWN_CHARGER_SetDigitalMode()     do { ANSELAbits.ANSELA1 = 0; } while(0)

// get/set LOAD_CURRENT aliases
#define LOAD_CURRENT_TRIS                 TRISAbits.TRISA2
#define LOAD_CURRENT_LAT                  LATAbits.LATA2
#define LOAD_CURRENT_PORT                 PORTAbits.RA2
#define LOAD_CURRENT_WPU                  WPUAbits.WPUA2
#define LOAD_CURRENT_OD                   ODCONAbits.ODCA2
#define LOAD_CURRENT_ANS                  ANSELAbits.ANSELA2
#define LOAD_CURRENT_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define LOAD_CURRENT_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define LOAD_CURRENT_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define LOAD_CURRENT_GetValue()           PORTAbits.RA2
#define LOAD_CURRENT_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define LOAD_CURRENT_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define LOAD_CURRENT_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define LOAD_CURRENT_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define LOAD_CURRENT_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define LOAD_CURRENT_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define LOAD_CURRENT_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define LOAD_CURRENT_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)

// get/set GRID_SENSE aliases
#define GRID_SENSE_TRIS                 TRISAbits.TRISA3
#define GRID_SENSE_LAT                  LATAbits.LATA3
#define GRID_SENSE_PORT                 PORTAbits.RA3
#define GRID_SENSE_WPU                  WPUAbits.WPUA3
#define GRID_SENSE_OD                   ODCONAbits.ODCA3
#define GRID_SENSE_ANS                  ANSELAbits.ANSELA3
#define GRID_SENSE_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define GRID_SENSE_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define GRID_SENSE_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define GRID_SENSE_GetValue()           PORTAbits.RA3
#define GRID_SENSE_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define GRID_SENSE_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define GRID_SENSE_SetPullup()          do { WPUAbits.WPUA3 = 1; } while(0)
#define GRID_SENSE_ResetPullup()        do { WPUAbits.WPUA3 = 0; } while(0)
#define GRID_SENSE_SetPushPull()        do { ODCONAbits.ODCA3 = 0; } while(0)
#define GRID_SENSE_SetOpenDrain()       do { ODCONAbits.ODCA3 = 1; } while(0)
#define GRID_SENSE_SetAnalogMode()      do { ANSELAbits.ANSELA3 = 1; } while(0)
#define GRID_SENSE_SetDigitalMode()     do { ANSELAbits.ANSELA3 = 0; } while(0)

// get/set SOLAR_CURRENT aliases
#define SOLAR_CURRENT_TRIS                 TRISAbits.TRISA4
#define SOLAR_CURRENT_LAT                  LATAbits.LATA4
#define SOLAR_CURRENT_PORT                 PORTAbits.RA4
#define SOLAR_CURRENT_WPU                  WPUAbits.WPUA4
#define SOLAR_CURRENT_OD                   ODCONAbits.ODCA4
#define SOLAR_CURRENT_ANS                  ANSELAbits.ANSELA4
#define SOLAR_CURRENT_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define SOLAR_CURRENT_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define SOLAR_CURRENT_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define SOLAR_CURRENT_GetValue()           PORTAbits.RA4
#define SOLAR_CURRENT_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define SOLAR_CURRENT_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define SOLAR_CURRENT_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define SOLAR_CURRENT_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define SOLAR_CURRENT_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define SOLAR_CURRENT_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define SOLAR_CURRENT_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define SOLAR_CURRENT_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set BATTERY aliases
#define BATTERY_TRIS                 TRISAbits.TRISA5
#define BATTERY_LAT                  LATAbits.LATA5
#define BATTERY_PORT                 PORTAbits.RA5
#define BATTERY_WPU                  WPUAbits.WPUA5
#define BATTERY_OD                   ODCONAbits.ODCA5
#define BATTERY_ANS                  ANSELAbits.ANSELA5
#define BATTERY_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define BATTERY_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define BATTERY_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define BATTERY_GetValue()           PORTAbits.RA5
#define BATTERY_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define BATTERY_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define BATTERY_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define BATTERY_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define BATTERY_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define BATTERY_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define BATTERY_SetAnalogMode()      do { ANSELAbits.ANSELA5 = 1; } while(0)
#define BATTERY_SetDigitalMode()     do { ANSELAbits.ANSELA5 = 0; } while(0)

// get/set TEMP2 aliases
#define TEMP2_TRIS                 TRISAbits.TRISA6
#define TEMP2_LAT                  LATAbits.LATA6
#define TEMP2_PORT                 PORTAbits.RA6
#define TEMP2_WPU                  WPUAbits.WPUA6
#define TEMP2_OD                   ODCONAbits.ODCA6
#define TEMP2_ANS                  ANSELAbits.ANSELA6
#define TEMP2_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define TEMP2_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define TEMP2_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define TEMP2_GetValue()           PORTAbits.RA6
#define TEMP2_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define TEMP2_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)
#define TEMP2_SetPullup()          do { WPUAbits.WPUA6 = 1; } while(0)
#define TEMP2_ResetPullup()        do { WPUAbits.WPUA6 = 0; } while(0)
#define TEMP2_SetPushPull()        do { ODCONAbits.ODCA6 = 0; } while(0)
#define TEMP2_SetOpenDrain()       do { ODCONAbits.ODCA6 = 1; } while(0)
#define TEMP2_SetAnalogMode()      do { ANSELAbits.ANSELA6 = 1; } while(0)
#define TEMP2_SetDigitalMode()     do { ANSELAbits.ANSELA6 = 0; } while(0)

// get/set OUTPUT_VOLTAGE aliases
#define OUTPUT_VOLTAGE_TRIS                 TRISAbits.TRISA7
#define OUTPUT_VOLTAGE_LAT                  LATAbits.LATA7
#define OUTPUT_VOLTAGE_PORT                 PORTAbits.RA7
#define OUTPUT_VOLTAGE_WPU                  WPUAbits.WPUA7
#define OUTPUT_VOLTAGE_OD                   ODCONAbits.ODCA7
#define OUTPUT_VOLTAGE_ANS                  ANSELAbits.ANSELA7
#define OUTPUT_VOLTAGE_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define OUTPUT_VOLTAGE_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define OUTPUT_VOLTAGE_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define OUTPUT_VOLTAGE_GetValue()           PORTAbits.RA7
#define OUTPUT_VOLTAGE_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define OUTPUT_VOLTAGE_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)
#define OUTPUT_VOLTAGE_SetPullup()          do { WPUAbits.WPUA7 = 1; } while(0)
#define OUTPUT_VOLTAGE_ResetPullup()        do { WPUAbits.WPUA7 = 0; } while(0)
#define OUTPUT_VOLTAGE_SetPushPull()        do { ODCONAbits.ODCA7 = 0; } while(0)
#define OUTPUT_VOLTAGE_SetOpenDrain()       do { ODCONAbits.ODCA7 = 1; } while(0)
#define OUTPUT_VOLTAGE_SetAnalogMode()      do { ANSELAbits.ANSELA7 = 1; } while(0)
#define OUTPUT_VOLTAGE_SetDigitalMode()     do { ANSELAbits.ANSELA7 = 0; } while(0)

// get/set RS aliases
#define RS_TRIS                 TRISBbits.TRISB0
#define RS_LAT                  LATBbits.LATB0
#define RS_PORT                 PORTBbits.RB0
#define RS_WPU                  WPUBbits.WPUB0
#define RS_OD                   ODCONBbits.ODCB0
#define RS_ANS                  ANSELBbits.ANSELB0
#define RS_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define RS_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define RS_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RS_GetValue()           PORTBbits.RB0
#define RS_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define RS_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define RS_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define RS_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define RS_SetPushPull()        do { ODCONBbits.ODCB0 = 0; } while(0)
#define RS_SetOpenDrain()       do { ODCONBbits.ODCB0 = 1; } while(0)
#define RS_SetAnalogMode()      do { ANSELBbits.ANSELB0 = 1; } while(0)
#define RS_SetDigitalMode()     do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set EN aliases
#define EN_TRIS                 TRISBbits.TRISB1
#define EN_LAT                  LATBbits.LATB1
#define EN_PORT                 PORTBbits.RB1
#define EN_WPU                  WPUBbits.WPUB1
#define EN_OD                   ODCONBbits.ODCB1
#define EN_ANS                  ANSELBbits.ANSELB1
#define EN_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define EN_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define EN_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define EN_GetValue()           PORTBbits.RB1
#define EN_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define EN_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define EN_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define EN_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define EN_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define EN_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define EN_SetAnalogMode()      do { ANSELBbits.ANSELB1 = 1; } while(0)
#define EN_SetDigitalMode()     do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set D4 aliases
#define D4_TRIS                 TRISBbits.TRISB2
#define D4_LAT                  LATBbits.LATB2
#define D4_PORT                 PORTBbits.RB2
#define D4_WPU                  WPUBbits.WPUB2
#define D4_OD                   ODCONBbits.ODCB2
#define D4_ANS                  ANSELBbits.ANSELB2
#define D4_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define D4_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define D4_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define D4_GetValue()           PORTBbits.RB2
#define D4_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define D4_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define D4_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define D4_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define D4_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define D4_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define D4_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define D4_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set D5 aliases
#define D5_TRIS                 TRISBbits.TRISB3
#define D5_LAT                  LATBbits.LATB3
#define D5_PORT                 PORTBbits.RB3
#define D5_WPU                  WPUBbits.WPUB3
#define D5_OD                   ODCONBbits.ODCB3
#define D5_ANS                  ANSELBbits.ANSELB3
#define D5_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define D5_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define D5_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define D5_GetValue()           PORTBbits.RB3
#define D5_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define D5_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define D5_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define D5_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define D5_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define D5_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define D5_SetAnalogMode()      do { ANSELBbits.ANSELB3 = 1; } while(0)
#define D5_SetDigitalMode()     do { ANSELBbits.ANSELB3 = 0; } while(0)

// get/set D6 aliases
#define D6_TRIS                 TRISBbits.TRISB4
#define D6_LAT                  LATBbits.LATB4
#define D6_PORT                 PORTBbits.RB4
#define D6_WPU                  WPUBbits.WPUB4
#define D6_OD                   ODCONBbits.ODCB4
#define D6_ANS                  ANSELBbits.ANSELB4
#define D6_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define D6_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define D6_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define D6_GetValue()           PORTBbits.RB4
#define D6_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define D6_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define D6_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define D6_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define D6_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define D6_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define D6_SetAnalogMode()      do { ANSELBbits.ANSELB4 = 1; } while(0)
#define D6_SetDigitalMode()     do { ANSELBbits.ANSELB4 = 0; } while(0)

// get/set D7 aliases
#define D7_TRIS                 TRISBbits.TRISB5
#define D7_LAT                  LATBbits.LATB5
#define D7_PORT                 PORTBbits.RB5
#define D7_WPU                  WPUBbits.WPUB5
#define D7_OD                   ODCONBbits.ODCB5
#define D7_ANS                  ANSELBbits.ANSELB5
#define D7_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define D7_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define D7_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define D7_GetValue()           PORTBbits.RB5
#define D7_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define D7_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define D7_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define D7_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define D7_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define D7_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define D7_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define D7_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set LCD_BACKLIGHT aliases
#define LCD_BACKLIGHT_TRIS                 TRISBbits.TRISB6
#define LCD_BACKLIGHT_LAT                  LATBbits.LATB6
#define LCD_BACKLIGHT_PORT                 PORTBbits.RB6
#define LCD_BACKLIGHT_WPU                  WPUBbits.WPUB6
#define LCD_BACKLIGHT_OD                   ODCONBbits.ODCB6
#define LCD_BACKLIGHT_ANS                  ANSELBbits.ANSELB6
#define LCD_BACKLIGHT_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define LCD_BACKLIGHT_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define LCD_BACKLIGHT_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define LCD_BACKLIGHT_GetValue()           PORTBbits.RB6
#define LCD_BACKLIGHT_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define LCD_BACKLIGHT_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define LCD_BACKLIGHT_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define LCD_BACKLIGHT_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define LCD_BACKLIGHT_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define LCD_BACKLIGHT_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define LCD_BACKLIGHT_SetAnalogMode()      do { ANSELBbits.ANSELB6 = 1; } while(0)
#define LCD_BACKLIGHT_SetDigitalMode()     do { ANSELBbits.ANSELB6 = 0; } while(0)

// get/set LED aliases
#define LED_TRIS                 TRISBbits.TRISB7
#define LED_LAT                  LATBbits.LATB7
#define LED_PORT                 PORTBbits.RB7
#define LED_WPU                  WPUBbits.WPUB7
#define LED_OD                   ODCONBbits.ODCB7
#define LED_ANS                  ANSELBbits.ANSELB7
#define LED_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define LED_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define LED_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define LED_GetValue()           PORTBbits.RB7
#define LED_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define LED_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define LED_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define LED_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)
#define LED_SetPushPull()        do { ODCONBbits.ODCB7 = 0; } while(0)
#define LED_SetOpenDrain()       do { ODCONBbits.ODCB7 = 1; } while(0)
#define LED_SetAnalogMode()      do { ANSELBbits.ANSELB7 = 1; } while(0)
#define LED_SetDigitalMode()     do { ANSELBbits.ANSELB7 = 0; } while(0)

// get/set TEMP1 aliases
#define TEMP1_TRIS                 TRISCbits.TRISC0
#define TEMP1_LAT                  LATCbits.LATC0
#define TEMP1_PORT                 PORTCbits.RC0
#define TEMP1_WPU                  WPUCbits.WPUC0
#define TEMP1_OD                   ODCONCbits.ODCC0
#define TEMP1_ANS                  ANSELCbits.ANSELC0
#define TEMP1_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define TEMP1_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define TEMP1_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define TEMP1_GetValue()           PORTCbits.RC0
#define TEMP1_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define TEMP1_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define TEMP1_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define TEMP1_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define TEMP1_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define TEMP1_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define TEMP1_SetAnalogMode()      do { ANSELCbits.ANSELC0 = 1; } while(0)
#define TEMP1_SetDigitalMode()     do { ANSELCbits.ANSELC0 = 0; } while(0)

// get/set FAN aliases
#define FAN_TRIS                 TRISCbits.TRISC1
#define FAN_LAT                  LATCbits.LATC1
#define FAN_PORT                 PORTCbits.RC1
#define FAN_WPU                  WPUCbits.WPUC1
#define FAN_OD                   ODCONCbits.ODCC1
#define FAN_ANS                  ANSELCbits.ANSELC1
#define FAN_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define FAN_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define FAN_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define FAN_GetValue()           PORTCbits.RC1
#define FAN_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define FAN_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define FAN_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define FAN_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define FAN_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define FAN_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define FAN_SetAnalogMode()      do { ANSELCbits.ANSELC1 = 1; } while(0)
#define FAN_SetDigitalMode()     do { ANSELCbits.ANSELC1 = 0; } while(0)

// get/set RC2 procedures
#define RC2_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define RC2_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define RC2_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define RC2_GetValue()              PORTCbits.RC2
#define RC2_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define RC2_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define RC2_SetPullup()             do { WPUCbits.WPUC2 = 1; } while(0)
#define RC2_ResetPullup()           do { WPUCbits.WPUC2 = 0; } while(0)
#define RC2_SetAnalogMode()         do { ANSELCbits.ANSELC2 = 1; } while(0)
#define RC2_SetDigitalMode()        do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set BUZZER aliases
#define BUZZER_TRIS                 TRISCbits.TRISC3
#define BUZZER_LAT                  LATCbits.LATC3
#define BUZZER_PORT                 PORTCbits.RC3
#define BUZZER_WPU                  WPUCbits.WPUC3
#define BUZZER_OD                   ODCONCbits.ODCC3
#define BUZZER_ANS                  ANSELCbits.ANSELC3
#define BUZZER_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define BUZZER_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define BUZZER_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define BUZZER_GetValue()           PORTCbits.RC3
#define BUZZER_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define BUZZER_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define BUZZER_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define BUZZER_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define BUZZER_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define BUZZER_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define BUZZER_SetAnalogMode()      do { ANSELCbits.ANSELC3 = 1; } while(0)
#define BUZZER_SetDigitalMode()     do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set RC4 procedures
#define RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RC4_GetValue()              PORTCbits.RC4
#define RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define RC4_SetPullup()             do { WPUCbits.WPUC4 = 1; } while(0)
#define RC4_ResetPullup()           do { WPUCbits.WPUC4 = 0; } while(0)
#define RC4_SetAnalogMode()         do { ANSELCbits.ANSELC4 = 1; } while(0)
#define RC4_SetDigitalMode()        do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set RC5 procedures
#define RC5_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define RC5_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define RC5_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define RC5_GetValue()              PORTCbits.RC5
#define RC5_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define RC5_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define RC5_SetPullup()             do { WPUCbits.WPUC5 = 1; } while(0)
#define RC5_ResetPullup()           do { WPUCbits.WPUC5 = 0; } while(0)
#define RC5_SetAnalogMode()         do { ANSELCbits.ANSELC5 = 1; } while(0)
#define RC5_SetDigitalMode()        do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetPullup()             do { WPUCbits.WPUC6 = 1; } while(0)
#define RC6_ResetPullup()           do { WPUCbits.WPUC6 = 0; } while(0)
#define RC6_SetAnalogMode()         do { ANSELCbits.ANSELC6 = 1; } while(0)
#define RC6_SetDigitalMode()        do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set BUTTON aliases
#define BUTTON_TRIS                 TRISCbits.TRISC7
#define BUTTON_LAT                  LATCbits.LATC7
#define BUTTON_PORT                 PORTCbits.RC7
#define BUTTON_WPU                  WPUCbits.WPUC7
#define BUTTON_OD                   ODCONCbits.ODCC7
#define BUTTON_ANS                  ANSELCbits.ANSELC7
#define BUTTON_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define BUTTON_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define BUTTON_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define BUTTON_GetValue()           PORTCbits.RC7
#define BUTTON_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define BUTTON_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define BUTTON_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define BUTTON_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define BUTTON_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define BUTTON_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define BUTTON_SetAnalogMode()      do { ANSELCbits.ANSELC7 = 1; } while(0)
#define BUTTON_SetDigitalMode()     do { ANSELCbits.ANSELC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/
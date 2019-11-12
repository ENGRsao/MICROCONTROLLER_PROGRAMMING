/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F25K83
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.00
        MPLAB             :  MPLAB X 5.10

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
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

#include "pin_manager.h"
#include "tmr0.h"

extern bool frequencyMeasurement,pf_measurement;
int i = 0
void (*IOCCF5_InterruptHandler)(void);
void (*IOCCF6_InterruptHandler)(void);


void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;

    /**
    TRISx registers
    */
    TRISA = 0xFF;
    TRISB = 0xFE;
    TRISC = 0xE7;

    /**
    ANSELx registers
    */
    ANSELC = 0x07;
    ANSELB = 0xFF;
    ANSELA = 0xFF;

    /**
    WPUx registers
    */
    WPUE = 0x00;
    WPUB = 0x00;
    WPUA = 0x00;
    WPUC = 0x00;

    /**
    ODx registers
    */
    ODCONA = 0x00;
    ODCONB = 0x00;
    ODCONC = 0x18;

    /**
    SLRCONx registers
    */
    SLRCONA = 0xFF;
    SLRCONB = 0xFF;
    SLRCONC = 0xFF;


    /**
    IOCx registers 
    */
    //interrupt on change for group IOCCF - flag
    IOCCFbits.IOCCF5 = 0;
    //interrupt on change for group IOCCF - flag
    IOCCFbits.IOCCF6 = 0;
    //interrupt on change for group IOCCN - negative
    IOCCNbits.IOCCN5 = 1;
    //interrupt on change for group IOCCN - negative
    IOCCNbits.IOCCN6 = 0;
    //interrupt on change for group IOCCP - positive
    IOCCPbits.IOCCP5 = 1;
    //interrupt on change for group IOCCP - positive
    IOCCPbits.IOCCP6 = 1;



    // register default IOC callback functions at runtime; use these methods to register a custom function
    IOCCF5_SetInterruptHandler(IOCCF5_DefaultInterruptHandler);
    IOCCF6_SetInterruptHandler(IOCCF6_DefaultInterruptHandler);
   
    // Enable IOCI interrupt 
    PIE0bits.IOCIE = 1; 
    
	
    I2C1SDAPPS = 0x14;   //RC4->I2C1:SDA1;    
    RC3PPS = 0x21;   //RC3->I2C1:SCL1;    
    RB0PPS = 0x13;   //RB0->UART1:TX1;    
    RC4PPS = 0x22;   //RC4->I2C1:SDA1;    
    U1RXPPS = 0x17;   //RC7->UART1:RX1;    
    I2C1SCLPPS = 0x13;   //RC3->I2C1:SCL1;    
}
  
void PIN_MANAGER_IOC(void)
{   
	// interrupt on change for pin IOCCF5
    if(IOCCFbits.IOCCF5 == 1)
    {
        IOCCF5_ISR();  
    }	
	// interrupt on change for pin IOCCF6
    if(IOCCFbits.IOCCF6 == 1)
    {
        IOCCF6_ISR();  
    }	
}

/**
   IOCCF5 Interrupt Service Routine
*/
void IOCCF5_ISR(void) {

    // Add custom IOCCF5 code

    // Call the interrupt handler for the callback registered at runtime
    /*if(IOCCF5_InterruptHandler)
    {
        IOCCF5_InterruptHandler();
    }*/
    
    if(PORTCbits.RC5)
    {        
        if (frequencyMeasurement)
        {
            frequency +=  TMR0_ReadTimer;
            i++;
        }
        frequencyMeasurement = true;
        TMR0_StopTimer();
    }
    IOCCFbits.IOCCF5 = 0;
    TMR0_StartTimer();
    TMR1_StartTimer();
}

/**
  Allows selecting an interrupt handler for IOCCF5 at application runtime
*/
void IOCCF5_SetInterruptHandler(void (* InterruptHandler)(void)){
    IOCCF5_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for IOCCF5
*/
void IOCCF5_DefaultInterruptHandler(void){
    // add your IOCCF5 interrupt custom code
    // or set custom function using IOCCF5_SetInterruptHandler()
}

/**
   IOCCF6 Interrupt Service Routine
*/
void IOCCF6_ISR(void) {

    // Add custom IOCCF6 code

    // Call the interrupt handler for the callback registered at runtime
    if(IOCCF6_InterruptHandler)
    {
        IOCCF6_InterruptHandler();
    }
    IOCCFbits.IOCCF6 = 0;
}

/**
  Allows selecting an interrupt handler for IOCCF6 at application runtime
*/
void IOCCF6_SetInterruptHandler(void (* InterruptHandler)(void)){
    IOCCF6_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for IOCCF6
*/
void IOCCF6_DefaultInterruptHandler(void){
    // add your IOCCF6 interrupt custom code
    // or set custom function using IOCCF6_SetInterruptHandler()
}

/**
 End of File
*/
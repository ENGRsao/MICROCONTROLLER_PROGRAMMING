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
        Device            :  PIC18F25K83
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

// get/set RB0 procedures
#define RB0_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define RB0_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define RB0_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RB0_GetValue()              PORTBbits.RB0
#define RB0_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define RB0_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define RB0_SetPullup()             do { WPUBbits.WPUB0 = 1; } while(0)
#define RB0_ResetPullup()           do { WPUBbits.WPUB0 = 0; } while(0)
#define RB0_SetAnalogMode()         do { ANSELBbits.ANSELB0 = 1; } while(0)
#define RB0_SetDigitalMode()        do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set VOLT_MEASUREMENT aliases
#define VOLT_MEASUREMENT_TRIS                 TRISBbits.TRISB1
#define VOLT_MEASUREMENT_LAT                  LATBbits.LATB1
#define VOLT_MEASUREMENT_PORT                 PORTBbits.RB1
#define VOLT_MEASUREMENT_WPU                  WPUBbits.WPUB1
#define VOLT_MEASUREMENT_OD                   ODCONBbits.ODCB1
#define VOLT_MEASUREMENT_ANS                  ANSELBbits.ANSELB1
#define VOLT_MEASUREMENT_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define VOLT_MEASUREMENT_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define VOLT_MEASUREMENT_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define VOLT_MEASUREMENT_GetValue()           PORTBbits.RB1
#define VOLT_MEASUREMENT_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define VOLT_MEASUREMENT_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define VOLT_MEASUREMENT_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define VOLT_MEASUREMENT_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define VOLT_MEASUREMENT_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define VOLT_MEASUREMENT_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define VOLT_MEASUREMENT_SetAnalogMode()      do { ANSELBbits.ANSELB1 = 1; } while(0)
#define VOLT_MEASUREMENT_SetDigitalMode()     do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set VOLTAGE_MEASUREMENT aliases
#define VOLTAGE_MEASUREMENT_TRIS                 TRISBbits.TRISB2
#define VOLTAGE_MEASUREMENT_LAT                  LATBbits.LATB2
#define VOLTAGE_MEASUREMENT_PORT                 PORTBbits.RB2
#define VOLTAGE_MEASUREMENT_WPU                  WPUBbits.WPUB2
#define VOLTAGE_MEASUREMENT_OD                   ODCONBbits.ODCB2
#define VOLTAGE_MEASUREMENT_ANS                  ANSELBbits.ANSELB2
#define VOLTAGE_MEASUREMENT_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define VOLTAGE_MEASUREMENT_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define VOLTAGE_MEASUREMENT_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define VOLTAGE_MEASUREMENT_GetValue()           PORTBbits.RB2
#define VOLTAGE_MEASUREMENT_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define VOLTAGE_MEASUREMENT_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define VOLTAGE_MEASUREMENT_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define VOLTAGE_MEASUREMENT_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define VOLTAGE_MEASUREMENT_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define VOLTAGE_MEASUREMENT_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define VOLTAGE_MEASUREMENT_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define VOLTAGE_MEASUREMENT_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set RC3 procedures
#define RC3_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define RC3_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define RC3_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RC3_GetValue()              PORTCbits.RC3
#define RC3_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define RC3_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define RC3_SetPullup()             do { WPUCbits.WPUC3 = 1; } while(0)
#define RC3_ResetPullup()           do { WPUCbits.WPUC3 = 0; } while(0)
#define RC3_SetAnalogMode()         do { ANSELCbits.ANSELC3 = 1; } while(0)
#define RC3_SetDigitalMode()        do { ANSELCbits.ANSELC3 = 0; } while(0)

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

// get/set VOLT_ZERO_SENSE aliases
#define VOLT_ZERO_SENSE_TRIS                 TRISCbits.TRISC5
#define VOLT_ZERO_SENSE_LAT                  LATCbits.LATC5
#define VOLT_ZERO_SENSE_PORT                 PORTCbits.RC5
#define VOLT_ZERO_SENSE_WPU                  WPUCbits.WPUC5
#define VOLT_ZERO_SENSE_OD                   ODCONCbits.ODCC5
#define VOLT_ZERO_SENSE_ANS                  ANSELCbits.ANSELC5
#define VOLT_ZERO_SENSE_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define VOLT_ZERO_SENSE_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define VOLT_ZERO_SENSE_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define VOLT_ZERO_SENSE_GetValue()           PORTCbits.RC5
#define VOLT_ZERO_SENSE_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define VOLT_ZERO_SENSE_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define VOLT_ZERO_SENSE_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define VOLT_ZERO_SENSE_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define VOLT_ZERO_SENSE_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define VOLT_ZERO_SENSE_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define VOLT_ZERO_SENSE_SetAnalogMode()      do { ANSELCbits.ANSELC5 = 1; } while(0)
#define VOLT_ZERO_SENSE_SetDigitalMode()     do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set CURRENT_ZERO_SENSE aliases
#define CURRENT_ZERO_SENSE_TRIS                 TRISCbits.TRISC6
#define CURRENT_ZERO_SENSE_LAT                  LATCbits.LATC6
#define CURRENT_ZERO_SENSE_PORT                 PORTCbits.RC6
#define CURRENT_ZERO_SENSE_WPU                  WPUCbits.WPUC6
#define CURRENT_ZERO_SENSE_OD                   ODCONCbits.ODCC6
#define CURRENT_ZERO_SENSE_ANS                  ANSELCbits.ANSELC6
#define CURRENT_ZERO_SENSE_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define CURRENT_ZERO_SENSE_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define CURRENT_ZERO_SENSE_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define CURRENT_ZERO_SENSE_GetValue()           PORTCbits.RC6
#define CURRENT_ZERO_SENSE_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define CURRENT_ZERO_SENSE_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define CURRENT_ZERO_SENSE_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define CURRENT_ZERO_SENSE_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define CURRENT_ZERO_SENSE_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define CURRENT_ZERO_SENSE_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define CURRENT_ZERO_SENSE_SetAnalogMode()      do { ANSELCbits.ANSELC6 = 1; } while(0)
#define CURRENT_ZERO_SENSE_SetDigitalMode()     do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()             do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()           do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSELC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSELC7 = 0; } while(0)

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


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCCF5 pin functionality
 * @Example
    IOCCF5_ISR();
 */
void IOCCF5_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCCF5 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCCF5 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF5_SetInterruptHandler(MyInterruptHandler);

*/
void IOCCF5_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCCF5 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCCF5_SetInterruptHandler() method.
    This handler is called every time the IOCCF5 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF5_SetInterruptHandler(IOCCF5_InterruptHandler);

*/
extern void (*IOCCF5_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCCF5 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCCF5_SetInterruptHandler() method.
    This handler is called every time the IOCCF5 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF5_SetInterruptHandler(IOCCF5_DefaultInterruptHandler);

*/
void IOCCF5_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCCF6 pin functionality
 * @Example
    IOCCF6_ISR();
 */
void IOCCF6_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCCF6 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCCF6 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF6_SetInterruptHandler(MyInterruptHandler);

*/
void IOCCF6_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCCF6 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCCF6_SetInterruptHandler() method.
    This handler is called every time the IOCCF6 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF6_SetInterruptHandler(IOCCF6_InterruptHandler);

*/
extern void (*IOCCF6_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCCF6 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCCF6_SetInterruptHandler() method.
    This handler is called every time the IOCCF6 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF6_SetInterruptHandler(IOCCF6_DefaultInterruptHandler);

*/
void IOCCF6_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/
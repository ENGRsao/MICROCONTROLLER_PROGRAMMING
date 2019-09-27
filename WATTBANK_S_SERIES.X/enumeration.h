/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation
#include <stdint.h>
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

//enumeration definition 
typedef enum {
    SYSTEM_OFF = 0,
    SYSTEM_ON,
    CALIBRATION,
    DIAGNOSTIC,
} system_mode_t;

typedef enum{
	FAN_OFF = 0,
	FAN_25 = 190,
	FAN_50 = 290,
	FAN_75 = 390,
	FAN_100 = 499,	
}fan_mode_t ;

typedef enum {
    SYSTEM_NOT_CHARGING=0,
    SYSTEM_CHARGING,
} system_subMode_t;

typedef enum{
	WATTBANK,
	GRID_POWER,
}power_mode;

typedef enum
{
    BAT_NORM = 0,
    LOW_BATTERY,
    BATTERY_FULL,
}battery_status_t;


typedef enum
{
    CHARGE_CONTROL_NORMAL = 0,
    CHARGE_CONTROL_THERMAL,
    CHARGE_CONTROL_OVERCHARGED,
}charging_control;


typedef enum
{
    NORMAL_ERROR = 0,
    PLUG_IN_NOT_CHARGING,
    NO_OUTPUT_VOLTAGE,            
}error_status_t;

typedef enum
{
    NORMAL_WARN = 0,
    OVERLOAD,
    LOW_BATTERY_WARNING,
    THERMAL_SHUTDOWN, 
}warning_status_t;

typedef enum
{
    BMS_NORMAL = 0,
    SHORT_CIRCUIT,
    OVER_TEMPERATURE,
    OVER_CURRENT,
    BMS_FAIL_ERROR,
}bms_error_status_t;

typedef union{
uint8_t byte_data;
struct {
uint8_t bit0:1;  //battery_low
uint8_t bit1:1;  //battery full
uint8_t bit2:1;  //pre-calibration
uint8_t bit3:1;  //calibration
uint8_t bit4:1;  //short circuit
uint8_t bit5:1;  //over temperature
uint8_t bit6:1;  //over current
uint8_t bit7:1;  //bms failure error
}bit_data;
}bms_status_t;

typedef union
{
       struct
            {
                uint8_t system_percentage;
                uint32_t system_Columbcount; 
                uint8_t system_status;
                uint8_t hour_left;
                uint8_t minutes_left;
                uint16_t cell0;
                uint16_t cell1;
                uint16_t cell2;
                uint16_t cell3;
                uint16_t cell4;
                uint16_t cell5;
                uint16_t cell6;    
                uint16_t cell7;
                uint16_t cell8;
                uint16_t cell9;
                uint16_t cell10;
                uint16_t cell11;
                uint32_t batteryTemperature;
                uint16_t ACS_offset;
                uint32_t columbReference;
                uint16_t chargeCurrent;
                uint16_t dischargeCurrent;
            }bms_data_structure;
           uint8_t bms_datastructure[46];
}bms_diagnostic_data_type;

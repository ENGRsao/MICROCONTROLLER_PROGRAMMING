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

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

const uint8_t photizzo_logo[] = {
  0b00000,
  0b01111,
  0b01001,
  0b01001,
  0b11111,
  0b01000,
  0b00000,
  0b00000
};

const uint8_t battery_empty[] = { 
  0x0E,
  0x0A,
  0x1B,
  0x11,
  0x11,
  0x11,
  0x11,
  0x1F
};
const uint8_t battery_25[] = { 
  0x0E,
  0x0A,
  0x1B,
  0x11,
  0x11,
  0x11,
  0x1F,
  0x1F
};
const uint8_t battery_50[] = { 
  0x0E,
  0x0A,
  0x1B,
  0x11,
  0x11,
  0x1F,
  0x1F,
  0x1F
};
const uint8_t battery_75[] = { 
  0x0E,
  0x0A,
  0x1B,
  0x11,
  0x1F,
  0x1F,
  0x1F,
  0x1F
};
const uint8_t battery_100[] = { 
  0x0E,
  0x0E,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F
};
const uint8_t   char_plug[] = {
    0x04,
	0x0A,
	0x1F,
	0x1F,
	0x1F,
	0x1F,
	0x00,
	0x00
	/*0x0A,
    0x0A,
    0x1B,
    0x1F,
    0x1F,
    0x0E,
    0x04,
    0x04*/
};
const uint8_t   solar_house[] = {
  0x04,
  0x0F,
  0x08,
  0x0F,
  0x01,
  0x0F,
  0x00,
  0x00
};
const uint8_t grid_char[]={
	0b00010,
    0b00100,
    0b01000,
    0b11111,
    0b00010,
    0b00100,
    0b01000,
    0b10000
};
const uint8_t warning_sign[]={
  0b00000,
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b00000,
  0b10101
};
const uint8_t   char_time[] = {
    0b01110,
    0b00100,
    0b01110,
    0b11011,
    0b10101,
    0b10001,
    0b01110,
    0b00000
};
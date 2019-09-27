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
//LCD Functions Developed by electroSome
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/device_config.h"
#include <stdbool.h>
#include <stdint.h>
    #define RS LATBbits.LATB0
    #define EN LATBbits.LATB1
    #define D4 LATBbits.LATB2
    #define D5 LATBbits.LATB3
    #define D6 LATBbits.LATB4
    #define D7 LATBbits.LATB5


void Lcd_Port(char a); //send data to a port

void Lcd_Cmd(char a); //send command to LCD 

void Lcd_Clear(); // clear lcd screen

void LCD_Send(bool RSV,uint8_t data);

void LCD_CreateCharacter(uint8_t address,uint8_t * data);

void Lcd_Set_Cursor(char a, char b); // move cursor on LCD to a define location in the x,y direction


void Lcd_Init(); //initialize the LCD pheripheral


void Lcd_Write_Char(char a); // write a character to the LCD screen

//void CreateCustomCharacter (unsigned char *Pattern, const char Location); // create custom character on LCD screen
// parameters
// Pattern : Pattern intending to draw
//Location : location to be stored in the LCD memory

void Lcd_Write_String(char *a); //send string data to lcd 

void Lcd_Shift_Right(); //shift lcd characters to the right

void Lcd_Shift_Left(); //shift lcd characters to the left 


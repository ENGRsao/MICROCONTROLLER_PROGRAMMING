/*
 * File:   lcd.c
 * Author: ENGR SAO
 *
 * Created on June 7, 2019, 9:52 AM
 */
#include "lcd.h"


#define LCD_CMD 0
#define LCD_DATA 1

void Lcd_Port(char a)
{
	if(a & 1)
		D4 = 1;
	else
		D4 = 0;

	if(a & 2)
		D5 = 1;
	else
		D5 = 0;

	if(a & 4)
		D6 = 1;
	else
		D6 = 0;

	if(a & 8)
		D7 = 1;
	else
		D7 = 0;
}
void Lcd_Cmd(char a)
{
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
        __delay_ms(4);
        EN  = 0;             // => E = 0
}

void Lcd_Clear()
{
	Lcd_Cmd(0);
	Lcd_Cmd(1);
    __delay_ms(5);
}

void Lcd_Set_Cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
	{
	  temp = 0x80 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
    	else if(a == 3)
	{
		temp = 0x94 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
    	else if(a == 4)
	{
		temp = 0xD4 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
}

void Lcd_Init()
{
   Lcd_Port(0x00);
  __delay_ms(20); 
  Lcd_Cmd(0x03);
	__delay_ms(5);
  Lcd_Cmd(0x03);
	__delay_ms(11);
  Lcd_Cmd(0x03); 
  Lcd_Cmd(0x02); //02H is used for Return home -> Clears the RAM and initializes the LCD
  Lcd_Cmd(0x02); //02H is used for Return home -> Clears the RAM and initializes the LCD
  Lcd_Cmd(0x08); //Select Row 1
  Lcd_Cmd(0x00); //Clear Row 1 Display
  Lcd_Cmd(0x0C); //Select Row 2
  //Lcd_Cmd(0x00); //Clear Row 2 Display
  //Lcd_Cmd(0x0E); //Select Row 3
  //Lcd_Cmd(0x00); //Clear Row 2 Display
  //Lcd_Cmd(0x0F); //Select Row 4
  Lcd_Cmd(0x00); //Clear Row 2 Display*/
  Lcd_Cmd(0x06);
  //__delay_ms(10);

}


void Lcd_Write_Char(char a)
{
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   RS = 1;             // => RS = 1
   Lcd_Port(y>>4);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   Lcd_Port(temp);
   EN = 1;
   __delay_us(40);
   EN = 0;
}

void LCD_Send(bool RSV , uint8_t data)
{
	RS =  RSV;    //RS = 0, Command send. RS = 1, Data Send.
	EN = 0;
	/*	Send upper nibble first */
	D7 = (data >> 7) & 0x01 ;
	D6 = (data >> 6) & 0x01 ;
	D5 = (data >> 5) & 0x01 ;
	D4 = (data >> 4) & 0x01 ;

	EN = 1;
	__delay_us(40);
	EN = 0;
	/*	Now send lower nibble */
	D7 = (data >> 3) & 0x01 ;
	D6 = (data >> 2) & 0x01 ;
	D5 = (data >> 1) & 0x01 ;
	D4 = data & 0x01 ;

	EN = 1;
	__delay_us(40);
	EN = 0;

	EN= 0; 
	__delay_us(80);
	//return;
}

void LCD_CreateCharacter(uint8_t address, uint8_t *data)
{
    address = address & 0x07;
    LCD_Send(LCD_CMD, (0x40 | (address<<3)));
    
    for (int i = 0; i < 8; i++)
    {
        LCD_Send(LCD_DATA, data[i]);
    }
}

/*
void CreateCustomCharacter (unsigned char *Pattern, const char Location)
{ 
int i=0; 
Lcd_Cmd (0x04);
Lcd_Cmd (Location * 8);   //Send the Address of CGRAM
for (i=0; i<8; i++)
{
    Lcd_Write_Char (Pattern [i] );         //Pass the bytes of pattern on LCD 
}
Lcd_Cmd (0); 
Lcd_Cmd (2); 
}
*/
void Lcd_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OLED.h"
#include "mcc_generated_files/i2c1.h"
#include "fonts.h"
/*
 * 
 */
 //_current_font cfont;
void init_OLED(void)
{
    transmitCommand(OLED_DISPLAY_OFF);
	transmitCommand(OLED_MEMORY_ADDR_MODE);
    transmitCommand(0x10);
	transmitCommand(OLED_SET_PAGE_ADDR);
	transmitCommand(0xC8);
	transmitCommand(OLED_SET_LOWER_COLUMN);
	transmitCommand(OLED_SET_HIGHER_COLUMN);
	transmitCommand(OLED_SET_START_LINE);
	transmitCommand(OLED_SET_CONTRAST_CONTROL);
	transmitCommand(0x7F);
	
	transmitCommand(0xA1);
	transmitCommand(OLED_NORMAL_DISPLAY);
	transmitCommand(OLED_SET_MULTIPLEX_RATIO);
	transmitCommand(0x3F);
	transmitCommand(OLED_DISPLAY_ALL_ON_RESUME);
	transmitCommand(OLED_SET_DISPLAY_OFFSET	);
	 transmitCommand(0x00);	 
    transmitCommand(OLED_SET_DISPLAY_CLOCK_DIV_RATIO);
    transmitCommand(0xF0);
    transmitCommand(OLED_SET_PRECHARGE_PERIOD);
	transmitCommand(0x22);
   transmitCommand(OLED_SET_COM_PINS);
    transmitCommand(0x12);
	transmitCommand(OLED_SET_VCOM_DESELECT);
    transmitCommand(0x20);
    transmitCommand(OLED_CHARGE_PUMP);
	transmitCommand(0x14);
    
    transmitCommand(OLED_DISPLAY_ON);
    //clrscreen();
    //updateData();
    
}

void powerUP()
{
	transmitCommand(OLED_DISPLAY_ALL_ON);
}

void powerDown()
{
	transmitCommand(OLED_DISPLAY_OFF);
}

uint8_t clearPixel[32] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t leftOver[4] = {0x00,0x00,0x00,0x00};

void clearRow(uint8_t row)
{
	int page = OLED_SET_PAGE_ADDR + row;
	transmitCommand(page);
	int i = 0;
	for (i = 0; i < 4;i++)
	{
		transmitDATA(clearPixel,32);
	}	
	transmitDATA(leftOver,4);	
}

void clearScreen()
{
	int i = 0;
	for (i = 0;i < 8; i++)
	{
		clearRow(i);
		transmitCommand(OLED_SET_LOWER_COLUMN);
		transmitCommand(OLED_SET_HIGHER_COLUMN);
	}
}

//set cursor to row between 0  and 7
//se column between 0  and 31
void setCursorPosition(int row,int column)
{
	//set row
	int page = OLED_SET_PAGE_ADDR + row;
	transmitCommand(page);
	
	//compute column
	column += 2;
	int lowerAddressByte = (column & 0x0F);
	int upperAddressByte = (column >> 4) + OLED_SET_HIGHER_COLUMN;
	transmitCommand(upperAddressByte);
	transmitCommand(lowerAddressByte);
}


void transmitCommand(uint8_t command)
{
    i2c1_write1ByteRegister(OLED_ADDR,OLED_COMMAND,command);
}

void transmitByte(uint8_t data)
{
	i2c1_write1ByteRegister(OLED_ADDR,OLED_DATA_CONTINUE,data);
}

void transmitDATA(uint8_t *data,uint8_t datalen)
{
    i2c1_writeNBytes(OLED_ADDR,OLED_DATA_CONTINUE,data,datalen);
}

void print2Screen(uint8_t row , uint8_t column,char *data )
{
	int current_row, current_column;
	uint8_t pixelVal[5];
	setCursorPosition(row,column);
	current_row = row;
	current_column = column;
    int i = 0;
	for (i = 0; i < strlen(data) ; i++)
	{
		int character = ((char)(data[i]) - ' ') * 5;
		uint8_t k = 0;
		for (k = 0; k < 5; k++)
		{
			pixelVal[k] = capFont[character++];
		}
		transmitDATA(pixelVal,5);
		transmitByte(0x00);
		//current_column += 6;
		//if(current_column >= )		
	}
	
}





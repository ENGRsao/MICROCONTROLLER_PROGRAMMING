#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ssd1306.h"
#include <stdbool.h>
#include "i2c.h"
/*
 * 
 */

void init_OLED(void)
{
    // 128x64 pixel specific initialization.
        transmitCommand(SSD1306_DISPLAYOFF) ;                   // 0xAE
        transmitCommand(SSD1306_SETDISPLAYCLOCKDIV) ;           // 0xD5
        transmitCommand(0x80);               // the suggested ratio 0x80
        transmitCommand(SSD1306_SETMULTIPLEX) ;                 // 0xA8
        transmitCommand(0x3F);
        transmitCommand(SSD1306_SETDISPLAYOFFSET) ;             // 0xD3
        transmitCommand(0x0)   ;                                // no offset
        transmitCommand(SSD1306_SETSTARTLINE | 0x0) ;           // line #0
        transmitCommand(SSD1306_CHARGEPUMP) ;                   // 0x8D
        //if (vccstate == SSD1306_EXTERNALVCC)
           // transmitCommand(0x10);
        //else
        transmitCommand(0x14);
        transmitCommand(SSD1306_MEMORYMODE);                    // 0x20
        transmitCommand(0x00) ;                                 // 0x0 act like ks0108
        transmitCommand(SSD1306_SEGREMAP | 0x1);
        transmitCommand(SSD1306_COMSCANDEC);
        transmitCommand(SSD1306_SETCOMPINS) ;                   // 0xDA
        transmitCommand(0x12);
        transmitCommand(SSD1306_SETCONTRAST) ;                  // 0x81
        //if (vccstate == SSD1306_EXTERNALVCC)
           // transmitCommand(0x9F);
        //else
           transmitCommand(0xCF);
        transmitCommand(SSD1306_SETPRECHARGE);                  // 0xd9
       // if (vccstate == SSD1306_EXTERNALVC)
            //transmitCommand(0x22);
        //else
           transmitCommand(0xF1);
     
  
        transmitCommand(SSD1306_SETVCOMDETECT);                 //0xDB
        transmitCommand(0x40);
        //transmitCommand(SSD1306_DEACTIVATE_SCROLL );
        //transmitCommand(SSD1306_DISPLAYALLON);  
        transmitCommand(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
        transmitCommand(SSD1306_NORMALDISPLAY);                 // 0xA6
    
       transmitCommand(SSD1306_DISPLAYON);
    //clrscreen();
    //updateData();
    
}

void powerUP()
{
	transmitCommand(SSD1306_DISPLAYALLON);
}

void powerDown()
{
	transmitCommand(SSD1306_DISPLAYALLON);
}



void clearRow(uint8_t row)
{
	int page = SSD1306_PAGEADDR + row;
	transmitCommand(page);
	int i = 0;
	for (i = 0; i < 4;i++)
	{
        transmitByte(0x00);
		//transmitDATA(clearPixel,32);
	}	
    transmitByte(0x40);
	///transmitDATA(leftOver,4);	
}

void clearScreen()
{
	int i = 0;
	for (i = 0;i < 8; i++)
	{
		clearRow(i);
		transmitCommand(SSD1306_SETLOWCOLUMN);
		transmitCommand(SSD1306_SETHIGHCOLUMN );
	}
}

void setCursorPositions(int row,int column)
{
	//set row
	int page = 0xB0 + row;
//	transmitCommand(page);
	transmitCommand(page);
    
	//compute column
	column += 2;
	int lowerAddressByte = (column & 0x0F) + SSD1306_SETLOWCOLUMN;
	int upperAddressByte = (column >> 4) + SSD1306_SETHIGHCOLUMN;	
	transmitCommand(lowerAddressByte);
    transmitCommand(upperAddressByte);
}

void transmitCommand(uint8_t command)
{
//    uint16_t dat;
//    dat = (OLED_COMMAND << 8 ) | command;
//    I2C_Write2ByteRegister(OLED_ADDR<<1,OLED_COMMAND,dat);
   
    I2C_Master_Start();         //Start condition
    
    I2C_Master_Write(OLED_ADDR << 1);   //7 bit address + Write
    
     I2C_Master_Write(OLED_COMMAND); 
    
    I2C_Master_Write(command);    //Write data
    
    I2C_Master_Stop();          //Stop condition
    __delay_ms(10);    
}

void transmitByte(uint8_t data)
{
//	 uint16_t dat;
//     dat = (OLED_COMMAND << 8 ) | data;
//   I2C_Write2ByteRegister(OLED_ADDR<<1,OLED_DATA_CONTINUE,dat);
   
    I2C_Master_Start();         //Start condition
   
    I2C_Master_Write(OLED_ADDR <<1);   //7 bit address + Write
   
     I2C_Master_Write(OLED_DATA_CONTINUE); 
   
    I2C_Master_Write(data);    //Write data
    
    I2C_Master_Stop();          //Stop condition
    __delay_ms(10);  
}

void transmitDATA(uint8_t *data,uint8_t datalen)
{
 //   I2C_WriteNBytes(OLED_ADDR,data,datalen);
    uint8_t *dataN = data; 
    I2C_Master_Start();         //Start condition
    I2C_Master_Write(OLED_ADDR <<1);     //7 bit address + Write
     I2C_Master_Write(OLED_DATA_CONTINUE); 
     int t  = 0;
     for (t = 0 ; t < datalen; t ++)
     {
         int k = 0;
         uint8_t val = *dataN++;
         for (k =0; k <1;k++)
         {
            I2C_Master_Write(val);
         }    //Write data
     }
    I2C_Master_Stop();          //Stop condition
    __delay_ms(10);  
   
}
void setColAddress()
{
    transmitCommand(SSD1306_COLUMNADDR);
  transmitCommand(0x00);   // Column start address (0 = reset)
  transmitCommand(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)

}
/////////////////////////////////////////////////////
// Used when doing Horizontal or Vertical Addressing
void setPageAddress()
{
  transmitCommand(SSD1306_PAGEADDR);
  transmitCommand(0x00); // Page start address (0 = reset)
 
   transmitCommand(0x07); // Page end address
}
void TransferBuffer()
{
  uint16_t j=0;
 
      // set the Column and Page addresses to 0,0
     // setColAddress();
     // setPageAddress();
       
       I2C_Master_Start();         //Start condition
       I2C_Master_Write(OLED_ADDR << 1);     //7 bit address + Write
       I2C_Master_Write(OLED_DATA_CONTINUE);
      for(j=0;j<1024;j++)
      {
        I2C_Master_Write(0x00);
      }

      I2C_Master_Stop();
}


void print2Screen(uint8_t row , uint8_t column,char *data )
{
	int current_row, current_column;
	uint8_t pixelVal[5];
	setCursorPositions(row,column);
	current_row = row;
	current_column = column;
    int i = 0;
	for (i = 0; i < strlen(data) ; i++)
	{
		int character = ((char)(data[i]) - ' ') * 5;
		uint8_t k = 0;
       // pixelVal[0] = 0x40;
		for (k = 0; k < 5; k++)
		{
			pixelVal[k] = characterFont[character++];
		}
		transmitDATA(pixelVal,5);
		transmitByte(0x00);
		//current_column += 6;
		//if(current_column >= )		
	}	
}

void printCustomCharacter(uint8_t row , uint8_t column,char *data )
{
	int current_row, current_column;
	uint8_t pixelVal[5];
	setCursorPositions(row,column);
	current_row = row;
	current_column = column;
    int i = 0;
	for (i = 0; i < strlen(data) ; i++)
	{
		int character = ((char)(data[i]) - '0') * 5;
		uint8_t k = 0;
       // pixelVal[0] = 0x40;
		for (k = 0; k < 5; k++)
		{
			pixelVal[k] = customCharacter[character++];
		}
		transmitDATA(pixelVal,5);		
	}	
   // transmitByte(0x00);
}
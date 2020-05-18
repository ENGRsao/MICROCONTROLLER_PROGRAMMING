#include "i2c.h"


void I2C_Initialize()
{
    SSP1STAT = 0x00;
    SSP1CON1 = 0x08;
    SSP1CON2 = 0x00;
    SSP1ADD  = 0x09;
    SSP1CON1bits.SSPEN = 1;
}

//For Waiting
void I2C_Master_Wait()
{
    //while(!I2C_Open( 0x3c));
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); //Transmit is in progress
}
//Start Condition
void I2C_Master_Start()
{
  I2C_Master_Wait();    
  SEN = 1;             //Initiate start condition
}

void I2C_Master_RepeatedStart()
{
  I2C_Master_Wait();
  RSEN = 1;           //Initiate repeated start condition
}
//Stop Condition
void I2C_Master_Stop()
{
  I2C_Master_Wait();
  PEN = 1;           //Initiate stop condition
}
//Write Data
void I2C_Master_Write(unsigned char d)
{
  I2C_Master_Wait();
  SSPBUF  = d;         //Write data to SSPBUF
}
//Read Data
unsigned short I2C_Master_Read(unsigned short a)
{
  unsigned short temp;
  I2C_Master_Wait();
  RCEN = 1;
  I2C_Master_Wait();
  temp = SSPBUF;      //Read data from SSPBUF
  I2C_Master_Wait();
  ACKDT = (a)?0:1;    //Acknowledge bit
  ACKEN = 1;          //Acknowledge sequence
  return temp;
}
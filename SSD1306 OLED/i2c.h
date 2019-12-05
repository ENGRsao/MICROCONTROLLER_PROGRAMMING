
#include "../pic16f1769.h"

void I2C_Initialize(); //initialize i2c protocol at 400KHz
unsigned short I2C_Master_Read(unsigned short a); //read i2c data
void I2C_Master_Wait(); //wait while i2c is busy
void I2C_Master_Start(); //start i2c transaction
void I2C_Master_RepeatedStart(); // repeated stsrt for i2c
void I2C_Master_Stop(); //stop i2c transaction
void I2C_Master_Write(unsigned char d); //write data to i2c bus
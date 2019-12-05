//this header file is based on SSD1106 chip for 128*64 OLED
#include <stdint.h>
#include "mcc_generated_files/examples/i2c_master_example.h"
#include "mcc_generated_files/i2c_master.h"
#include "mcc_generated_files/device_config.h"
#include "mcc_generated_files/pin_manager.h"


#define OLED_ADDR		0x3C

#define LEFT	0
#define RIGHT	9999
#define CENTER	9998
#define int1 bool
#define OLED_COMMAND		0x00
#define OLED_DATA			0xC0
#define OLED_DATA_CONTINUE	0x40

#define RST_NOT_IN_USE	255

// OLED Commandset
// ------------------
// Fundamental Commands
#define SSD1306_I2C_ADDRESS  0x3C    // 011110+SA0+RW - 0x3C or 0x3D
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME  0xA4
#define SSD1306_DISPLAYALLON  0xA5
#define SSD1306_NORMALDISPLAY  0xA6
#define SSD1306_INVERTDISPLAY  0xA7
#define SSD1306_DISPLAYOFF  0xAE
#define SSD1306_DISPLAYON  0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE  0xD9
#define SSD1306_SETMULTIPLEX  0xA8
#define SSD1306_SETLOWCOLUMN  0x00
#define SSD1306_SETHIGHCOLUMN  0x10
#define SSD1306_SETSTARTLINE  0x40
#define SSD1306_MEMORYMODE  0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2
#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 32
//# Scrolling constants
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

#define ssd1306_swap(a, b) { int16_t t = a; a = b; b = t; }

uint8_t buffer[512];

uint8_t x_pos, y_pos, text_size;
bool wrap = true;


void setFont(uint8_t* font);
void transmitDATA(uint8_t *data,uint8_t datalen);
void transmitByte(uint8_t data);
void transmitCommand(uint8_t command);
void print2Screen(uint8_t row , uint8_t column,char *data );
void updateData();
void clearScreen();
void setCursorPositions(int row,int column);
void init_OLED(void);
void clearRow(uint8_t row);
void powerDown();
void powerUP();
void putChar(unsigned char a);
void oledclear();
void setCursorPosition(int row,int column);

void SSD1306_DrawPixel(uint8_t x, uint8_t y, int1 color);
void SSD1306_StartScrollRight(uint8_t start, uint8_t stop);
void SSD1306_StartScrollLeft(uint8_t start, uint8_t stop);
void SSD1306_StartScrollDiagRight(uint8_t start, uint8_t stop);
void SSD1306_StartScrollDiagLeft(uint8_t start, uint8_t stop);
void SSD1306_StopScroll(void);
void SSD1306_Dim(int1 dim);
void SSD1306_Display(void);
void SSD1306_ClearDisplay(void);
void SSD1306_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int1 color);
void SSD1306_DrawFastHLine(uint8_t x, uint8_t y, uint8_t w, int1 color);
void SSD1306_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h, bool color);
void SSD1306_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color);
void SSD1306_FillScreen(bool color);
void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r);
void SSD1306_DrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername);
void SSD1306_FillCircle(int16_t x0, int16_t y0, int16_t r, bool color );
void SSD1306_DrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername);
void SSD1306_FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, bool color);
void SSD1306_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void SSD1306_DrawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r);
void SSD1306_FillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, bool color);
void SSD1306_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void SSD1306_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool color );
void SSD1306_DrawChar(uint8_t x, uint8_t y, uint8_t c, uint8_t size);
void SSD1306_DrawText(uint8_t x, uint8_t y, char *_text, uint8_t size);
void SSD1306_TextSize(uint8_t t_size);
void SSD1306_GotoXY(uint8_t x, uint8_t y);
void SSD1306_Print(uint8_t c);
void SSD1306_PutCustomC(const uint8_t *c);
void SSD1306_SetTextWrap(bool w);
void SSD1306_InvertDisplay(bool i);

void setColAddress();
void setPageAddress();
void TransferBuffer();
void printCustomCharacter(uint8_t row , uint8_t column,char *data );
static const uint8_t customCharacter[] =
{
    0x81 , 0x81 , 0xEE , 0x3C , 0x3C,  //battery head empty 0 
    0xFF , 0xFF , 0xFF , 0x3C , 0x3C,  //battery head full 1    
    0x81 , 0x81 , 0x81 , 0x81 , 0x81,  //battery body empty slot 2
    0xFF , 0x81 , 0x81 , 0x81 , 0x81,  //last bar
    0xFF , 0xFF , 0xFF , 0xFF , 0xFF,  //battery body full    
    0x00 , 0x00 , 0x00 , 0x00 , 0x00,  //no charge signal
    0x88 , 0x4C , 0x2A , 0x19 , 0x08   //charge signal 3    
};


static const uint8_t characterFont[] =
{
        0x00, 0x00, 0x00, 0x00, 0x00 ,  //#Space
         0xFF, 0xFF, 0xFF, 0xFF, 0xFF ,  //#!
         0x00, 0x07, 0x00, 0x07, 0x00 ,  //#"
         0x12, 0x3f, 0x12, 0x3f, 0x12 ,  //#//#
         0x04, 0x25, 0x6b, 0x25, 0x10 ,  //#$
         0x46, 0x36, 0x1c, 0x36, 0x31 ,  //#%
         0x36, 0x49, 0x59, 0x26, 0x50 ,  //#&
         0x00, 0x00, 0x03, 0x00, 0x00 ,  //#'
         0x00, 0x00, 0x3e, 0x41, 0x41 ,  //#(
         0x41, 0x41, 0x3e, 0x00, 0x00 ,  //#)
         0x0a, 0x04, 0x1f, 0x04, 0x0a ,  //#*
         0x08, 0x08, 0x3e, 0x08, 0x08 ,  //#+
         0x00, 0x40, 0x30, 0x00, 0x00 ,  //#,
         0x00, 0x08, 0x08, 0x08, 0x00 ,  //#-
         0x00, 0x00, 0x40, 0x00, 0x00 ,  //#.
         0x40, 0x30, 0x1c, 0x06, 0x01 ,  //#/
         0x7f, 0x61, 0x5d, 0x43, 0x7f ,  //#0
         0x04, 0x42, 0x7f, 0x40, 0x00 ,  //#1
         0x62, 0x51, 0x49, 0x49, 0x46 ,  //#2
         0x60, 0x41, 0x49, 0x4d, 0x3b ,  //#3
         0x0f, 0x08, 0x08, 0x7e, 0x08 ,  //#4
         0x20, 0x47, 0x45, 0x25, 0x19 ,  //#5
         0x3c, 0x4a, 0x49, 0x49, 0x60 ,  //#6
         0x01, 0x71, 0x19, 0x0d, 0x07 ,  //#7
         0x36, 0x49, 0x49, 0x49, 0x36 ,  //#8
         0x06, 0x49, 0x49, 0x49, 0x3e ,  //#9
         0x00, 0x00, 0x14, 0x00, 0x00 ,  //#:
         0x00, 0x40, 0x34, 0x00, 0x00 ,  //#;
         0x00, 0x08, 0x14, 0x22, 0x00 ,  //#<
         0x00, 0x14, 0x14, 0x14, 0x00 ,  //#=
         0x00, 0x22, 0x14, 0x08, 0x00 ,  //#>
         0x02, 0x01, 0x51, 0x09, 0x06 ,  //#?
         0x2e, 0x41, 0x5d, 0x55, 0x1e ,  //#@
         0x7e, 0x09, 0x09, 0x09, 0x7e ,  //#A
         0x41, 0x7f, 0x49, 0x49, 0x36 ,  //#B
         0x3e, 0x41, 0x41, 0x41, 0x41 ,  //#C
         0x41, 0x7f, 0x41, 0x41, 0x3e ,  //#D
         0x7f, 0x49, 0x49, 0x49, 0x41 ,  //#E
         0x7f, 0x09, 0x09, 0x09, 0x01 ,  //#F
         0x3e, 0x41, 0x41, 0x49, 0x79 ,  //#G
         0x7f, 0x08, 0x08, 0x08, 0x7f ,  //#H
         0x00, 0x00, 0x7f, 0x00, 0x00 ,  //#I
         0x60, 0x40, 0x41, 0x41, 0x3f ,  //#J
         0x7f, 0x08, 0x1c, 0x36, 0x63 ,  //#K
         0x7f, 0x40, 0x40, 0x40, 0x40 ,  //#L
         0x7f, 0x06, 0x0c, 0x06, 0x7f ,  //#M
         0x7f, 0x06, 0x1c, 0x30, 0x7f ,  //#N
         0x7f, 0x41, 0x41, 0x41, 0x7f ,  //#O
         0x01, 0x7f, 0x09, 0x09, 0x06 ,  //#P
         0x3e, 0x41, 0x51, 0x21, 0x5e ,  //#Q
         0x7f, 0x09, 0x19, 0x29, 0x46 ,  //#R
         0x46, 0x49, 0x49, 0x49, 0x31 ,  //#S
         0x01, 0x01, 0x7f, 0x01, 0x01 ,  //#T
         0x3f, 0x40, 0x40, 0x40, 0x3f ,  //#U
         0x7f, 0x60, 0x38, 0x0e, 0x03 ,  //#V
         0x3f, 0x40, 0x38, 0x40, 0x3f ,  //#W
         0x63, 0x36, 0x1c, 0x36, 0x63 ,  //#X
         0x03, 0x0e, 0x78, 0x0e, 0x03 ,  //#Y
         0x41, 0x71, 0x5d, 0x47, 0x41 ,  //#Z
         0x00, 0x00, 0x7f, 0x41, 0x41 ,  //# 
         0x01, 0x06, 0x1c, 0x30, 0x40 ,  //#\/
         0x41, 0x41, 0x7f, 0x00, 0x00 ,  //# 
         0x04, 0x02, 0x01, 0x02, 0x04 ,  //#^
         0x40, 0x40, 0x40, 0x40, 0x40 ,  //#_
         0x00, 0x00, 0x01, 0x02, 0x00  };  //#`
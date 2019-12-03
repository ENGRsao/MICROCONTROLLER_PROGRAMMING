#include <stdint.h>
#include <stdbool.h>

#include "ST7789.h"
#ifndef pgm_read_byte
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
 #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
 #define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif
  uint16_t       _x1, _y1; // Coordinates of top-left corner
  uint16_t      _w, _h;
  uint8_t       _textsize_x;
  uint8_t       _textsize_y;
  uint16_t      _outlinecolor, _fillcolor, _textcolor;
  char          _label[10];

  boolean currstate, laststate;
/*!
    @brief  Call before issuing command(s) or data to display. Performs
            chip-select (if required). Required
            for all display types; not an SPI-specific function.
*/
void startWrite(void)
{
	#ifdef TFT_CS
		TFT_CS = 0;
	#endif		
}
/*!
    @brief  Call after issuing command(s) or data to display. Performs
            chip-deselect (if required). Required
            for all display types; not an SPI-specific function.
*/
void endWrite(void)
{
	#ifdef TFT_CS
		TFT_CS = 1;
	#endif
}

/*!
    @brief  Write a single command byte to the display. Chip-select and
            transaction must have been previously set -- this ONLY sets
            the device to COMMAND mode, issues the byte and then restores
            DATA mode. There is no corresponding explicit writeData()
            function -- just use SPI_transfer().
    @param  cmd  8-bit command to write.
*/

void writeCommand(uint8_t cmd)
{
	DC_COMMAND;
	SPI_START;
	SPI_transfer(cmd);
	SPI_END;
}

void writeData(uin8_t data)
{
  DC_DATA;
  SPI_START;
  SPI_transfer(cmd);
  SPI_END;
}

/*!
    @brief  Companion code to the initiliazation tables. Reads and issues
            a series of LCD commands stored in ROM byte array.
    @param  addr  Flash memory array with commands and data to send
*/
void displayInit(uint8_t *address)
{
	uint8_t numCommands, numArgs;
	uint16_t ms;
	//SPI_START;
	numCommands = *address++;   // Number of commands to follow
	while(numCommands--){
		writeCommand(*address++);
		numArgs = *address++;
		ms = numArgs & ST_CMD_DELAY;
		numArgs &= ~ST_CMD_DELAY;
		while(numArgs--){
			SPI_transfer(address);
		}
		if(ms){
			ms = *address++;
			if(ms == 255) ms = 500;
			__delay_ms(ms);
		}
	}
	//SPI_END
}

/**************************************************************************/
/*!
    @brief  Initialization code for ST7789 display
*/
/**************************************************************************/

void tft_init(void)
{
	_ystart = 0;
	_xstart = 0;
	_colstart = 0;
	_rowstart = 0;
	TFT_DC_TRIS = 0;
	
	#ifdef TFT_CS
		TFT_CS_TRIS = 0;
		TFT_CS = 1;
	#endif
	
	#ifdef TFT_RST
		TFT_RST_TRIS = 0;
		TFT_RST = 1;
		__delay_ms(100);
		TFT_RST = 0;
		__delay_ms(100);
		TFT_RST = 1;
		__delay_ms(200);
	#endif
	
	
	TFT_DC = 1;	
	_colstart = ST7789_240x240_XSTART;
	_rowstart = ST7789_240x240_YSTART;
	_height = 240;
	_width = 240;
	displayInit(cmd_240x240);
	setRotation(2);
}




/**************************************************************************/
/*!
  @brief  SPI displays set an address window rectangle for blitting pixels
  @param  x  Top left corner x coordinate
  @param  y  Top left corner x coordinate
  @param  w  Width of window
  @param  h  Height of window
*/
/**************************************************************************/
void setAddrWindow(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
  x += _xstart;
  y += _ystart;
  SPI_START;
  DC_COMMAND;
  writeCommand(ST77XX_CASET); // Column addr set
  DC_DATA;
  SPI_transfer(0);
  SPI_transfer(x);
  SPI_transfer(0);
  SPI_transfer(x+w-1);
  DC_COMMAND;
  writeCommand(ST77XX_RASET); // Row addr set
  DC_DATA;
  SPI_transfer(0);
  SPI_transfer(y);
  SPI_transfer(0);
  SPI_transfer(y+h-1);
  DC_COMMAND;
  writeCommand(ST77XX_RAMWR); // write to RAM
  SPI_END
}

/**************************************************************************/
/*!
    @brief  Set origin of (0,0) and orientation of TFT display
    @param  m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void setRotation(uint8_t m) {
  uint8_t madctl = 0;

  rotation = m & 3; // can't be higher than 3
writeCommand(ST77XX_MADCTL);
  switch (rotation) {
   case 0:
     madctl  = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
     _xstart = _colstart;
     _ystart = _rowstart;
     break;
   case 1:
     madctl  = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
     _xstart = _rowstart;
     _ystart = _colstart;
     break;
  case 2:
     madctl  = ST77XX_MADCTL_RGB;
     _xstart = _rowstart;
     _ystart = _colstart;
     break;
   case 3:
     madctl  = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
     _xstart = _rowstart;
     _ystart = _colstart;
     break;
  }
  //SPI_START;
  
  writeData(madctl);
  //SPI_END;
}


/*!
    @brief  Essentially writePixel() with a transaction around it. I don't
            think this is in use by any of our code anymore (believe it was
            for some older BMP-reading examples), but is kept here in case
            any user code relies on it. Consider it DEPRECATED.
    @param  color  16-bit pixel color in '565' RGB format.
*/
void pushColor(uint16_t color) {
    uint8_t hi = color >> 8, lo = color;
    SPI_START;
	DC_DATA;
    SPI_transfer(hi);
    SPI_transfer(lo);
    SPI_END;
}

void drawPixel(uint8_t x, uint8_t y, uint16_t color) {
	if(x<0 ||x>=_width || y<0 || y>=_height) return;
  if((x < _width) && (y < _height)) {
	  setAddrWindow(x, y, x+1, y+1);
    SPI_START;
	DC_DATA;
    SPI_transfer(color >> 8);
    SPI_transfer(color & 0xFF);
    SPI_END;
  }
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly horizontal line (this is often optimized in a subclass!)
    @param    x   Left-most x coordinate
    @param    y   Left-most y coordinate
    @param    w   Width in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void drawHLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color) {
 
 if( (x < _width) && (y < _height) && w) {     

    if((x >= _width) || (y >= _height))
      return;
  
    if((x + w - 1) >= _width)  
      w = _width  - x;
	setAddrWindow(x, y, x+w-1, y);
	uint8_t hi = color >> 8, lo = color;
    SPI_START;
	DC_DATA;
    
	uint8_t num8 = w>>3;
  while(num8--) {
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
  }
  num8 = (uint8_t)w & 7;
  while(num8--) { SPI_transfer(hi); SPI_transfer(lo); }

  CS_IDLE;
  SPI_END;
    while (w--) {
      SPI_transfer(hi);
      SPI_transfer(lo);
    }
    SPI_END;
  }
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly vertical line (this is often optimized in a subclass!)
    @param    x   Top-most x coordinate
    @param    y   Top-most y coordinate
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void drawVLine(uint8_t x, uint8_t y, uint8_t h, uint16_t color) {
 if(x>=_width || y>=_height || h<=0) return; 
 if((x < _width) && (y < _height) && h) {  
    uint8_t hi = color >> 8, lo = color;
    if((y + h - 1) >= _height) 
      h = _height - y;
   
    setAddrWindow(x, y, x, (y + h - 1));
	SPI_START;
	uint8_t num8 = h>>3;
  while(num8--) {
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
  }
  num8 = (uint8_t)h & 7;
    while (num8--) {
      SPI_transfer(hi);
      SPI_transfer(lo);
    }
    SPI_END;
  }
}

/**************************************************************************/
/*!
   @brief    Fill a rectangle completely with one color. Update in subclasses if desired!
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
  /*if(w && h) {                            // Nonzero width and height?  
    uint8_t hi = color >> 8, lo = color;
    if((x >= _width) || (y >= _height))
      return;
    if((x + w - 1) >= _width)  
      w = _width  - x;
    if((y + h - 1) >= _height) 
      h = _height - y;
    SPI_START;
    setAddrWindow(x, y, w, h);
    uint16_t px = (uint16_t)w * h;
    while (px--) {
      SPI_transfer(hi);
      SPI_transfer(lo);
    }
    SPI_END;
  }*/
  if(x>=_width || y>=_height || w<=0 || h<=0) return;
  if(x+w-1>=_width)  w=_width -x;
  if(y+h-1>=_height) h=_height-y;
  setAddrWindow(x, y, x+w-1, y+h-1);

  uint8_t hi = color >> 8, lo = color;
    
  SPI_START;
  DC_DATA;

  uint32_t num = (uint32_t)w*h;
  uint16_t num16 = num>>4;
  while(num16--) {
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
    SPI_transfer(hi); SPI_transfer(lo);
  }
  uint8_t num8 = num & 0xf;
  while(num8--) { SPI_transfer(hi); SPI_transfer(lo); }

  
  SPI_END;
}

/**************************************************************************/
/*!
   @brief    Fill the screen completely with one color. Update in subclasses if desired!
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void fillScreen(uint16_t color) {
    fillRect(0, 0, _width, _height, color);
}

/**************************************************************************/
/*!
    @brief  Invert the colors of the display (if supported by hardware).
            Self-contained, no transaction setup required.
    @param  i  true = inverted display, false = normal display.
*/
/**************************************************************************/
void invertDisplay(bool i) {
    SPI_START;
    writeCommand(i ? ST77XX_INVON : ST77XX_INVOFF);
    SPI_END;
}


void rgbWheel(int idx, uint8_t *_r, uint8_t *_g, uint8_t *_b)
{
  idx &= 0x1ff;
  if(idx < 85) { // R->Y  
    *_r = 255; *_g = idx * 3; *_b = 0;
    return;
  } else if(idx < 85*2) { // Y->G
    idx -= 85*1;
    *_r = 255 - idx * 3; *_g = 255; *_b = 0;
    return;
  } else if(idx < 85*3) { // G->C
    idx -= 85*2;
    *_r = 0; *_g = 255; *_b = idx * 3;
    return;  
  } else if(idx < 85*4) { // C->B
    idx -= 85*3;
    *_r = 0; *_g = 255 - idx * 3; *_b = 255;
    return;    
  } else if(idx < 85*5) { // B->M
    idx -= 85*4;
    *_r = idx * 3; *_g = 0; *_b = 255;
    return;    
  } else { // M->R
    idx -= 85*5;
    *_r = 255; *_g = 0; *_b = 255 - idx * 3;
   return;
  }
} 
uint16_t rgbWheel(int idx)
{
  uint8_t r,g,b;
  rgbWheel(idx, &r,&g,&b);
  return RGBto565(r,g,b);
}

void drawImage(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *img16) 
{
  if(x>=_width || y>=_height || w<=0 || h<=0) return;
  setAddrWindow(x, y, x+w-1, y+h-1);

  SPI_START;
  DC_DATA;

  uint32_t num = (uint32_t)w*h;
  uint16_t num16 = num>>3;
  uint8_t *img = (uint8_t *)img16;
  while(num16--) {
    SPI_transfer(*(img+1)); SPI_transfer(*(img+0)); img+=2;
    SPI_transfer(*(img+1)); SPI_transfer(*(img+0)); img+=2;
    SPI_transfer(*(img+1)); SPI_transfer(*(img+0)); img+=2;
    SPI_transfer(*(img+1)); SPI_transfer(*(img+0)); img+=2;
    SPI_transfer(*(img+1)); SPI_transfer(*(img+0)); img+=2;
    SPI_transfer(*(img+1)); SPI_transfer(*(img+0)); img+=2;
    SPI_transfer(*(img+1)); SPI_transfer(*(img+0)); img+=2;
    SPI_transfer(*(img+1)); SPI_transfer(*(img+0)); img+=2;
  }
  uint8_t num8 = num & 0x7;
  while(num8--) { SPI_transfer(*(img+1)); SPI_transfer(*(img+0)); img+=2; }
  
  SPI_END;
}

// ----------------------------------------------------------
// draws image from flash (PROGMEM)
/*void drawImageF(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *img16) 
{
  if(x>=_width || y>=_height || w<=0 || h<=0) return;
  setAddrWindow(x, y, x+w-1, y+h-1);

  SPI_START;
  DC_DATA;
  CS_ACTIVE;

  uint32_t num = (uint32_t)w*h;
  uint16_t num16 = num>>3;
  uint8_t *img = (uint8_t *)img16;
  while(num16--) {
    SPI_transfer(pgm_read_byte(img+1)); SPI_transfer(pgm_read_byte(img+0)); img+=2;
    SPI_transfer(pgm_read_byte(img+1)); SPI_transfer(pgm_read_byte(img+0)); img+=2;
    SPI_transfer(pgm_read_byte(img+1)); SPI_transfer(pgm_read_byte(img+0)); img+=2;
    SPI_transfer(pgm_read_byte(img+1)); SPI_transfer(pgm_read_byte(img+0)); img+=2;
    SPI_transfer(pgm_read_byte(img+1)); SPI_transfer(pgm_read_byte(img+0)); img+=2;
    SPI_transfer(pgm_read_byte(img+1)); SPI_transfer(pgm_read_byte(img+0)); img+=2;
    SPI_transfer(pgm_read_byte(img+1)); SPI_transfer(pgm_read_byte(img+0)); img+=2;
    SPI_transfer(pgm_read_byte(img+1)); SPI_transfer(pgm_read_byte(img+0)); img+=2;
  }
  uint8_t num8 = num & 0x7;
  while(num8--) { SPI_transfer(pgm_read_byte(img+1)); SPI_transfer(pgm_read_byte(img+0)); img+=2; }

  
  SPI_END;
}*/

// ----------------------------------------------------------
// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) 
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// ----------------------------------------------------------
void invertDisplay(bool mode) 
{
  writeCommand(!mode ? ST7789_INVON : ST7789_INVOFF);  // modes inverted?
}

// ----------------------------------------------------------
void partialDisplay(bool mode) 
{
  writeCommand(mode ? ST7789_PTLON : ST7789_NORON);
}

// ----------------------------------------------------------
void sleepDisplay(bool mode) 
{
  writeCommand(mode ? ST7789_SLPIN : ST7789_SLPOUT);
  delay(5);
}

// ----------------------------------------------------------
void enableDisplay(bool mode) 
{
  writeCommand(mode ? ST7789_DISPON : ST7789_DISPOFF);
}

// ----------------------------------------------------------
void idleDisplay(bool mode) 
{
  writeCommand(mode ? ST7789_IDMON : ST7789_IDMOFF);
}

// ----------------------------------------------------------
void Arduino_ST7789::resetDisplay() 
{
  writeCommand(ST7789_SWRESET);
  __delay_ms(10);
}

// ----------------------------------------------------------
void setScrollArea(uint16_t tfa, uint16_t bfa) 
{
  uint16_t vsa = 320-tfa-bfa; // ST7789 320x240 VRAM
  writeCommand(ST7789_VSCRDEF); // SETSCROLLAREA = 0x33
  writeData(tfa >> 8);
  writeData(tfa);
  writeData(vsa >> 8);
  writeData(vsa);
  writeData(bfa >> 8);
  writeData(bfa);
}

// ----------------------------------------------------------
void setScroll(uint16_t vsp) 
{
  writeCommand(ST7789_VSCRSADD); // VSCRSADD = 0x37
  writeData(vsp >> 8);
  writeData(vsp);
}

// ----------------------------------------------------------
void setPartArea(uint16_t sr, uint16_t er) 
{
  writeCommand(ST7789_PTLAR);  // SETPARTAREA = 0x30
  writeData(sr >> 8);
  writeData(sr);
  writeData(er >> 8);
  writeData(er);
}

// ----------------------------------------------------------
// doesn't work
void setBrightness(uint8_t br) 
{
  //writeCommand(ST7789_WRCACE);
  //writeData(0xb1);  // 80,90,b0, or 00,01,02,03
  //writeCommand(ST7789_WRCABCMB);
  //writeData(120);

  //BCTRL=0x20, dd=0x08, bl=0x04
  int val = 0x04;
  writeCommand(ST7789_WRCTRLD);
  writeData(val);
  writeCommand(ST7789_WRDISBV);
  writeData(br);
}

// ----------------------------------------------------------
// 0 - off
// 1 - idle
// 2 - normal
// 4 - display off
void powerSave(uint8_t mode) 
{
  if(mode==0) {
    writeCommand(ST7789_POWSAVE);
    writeData(0xec|3);
    writeCommand(ST7789_DLPOFFSAVE);
    writeData(0xff);
    return;
  }
  int is = (mode&1) ? 0 : 1;
  int ns = (mode&2) ? 0 : 2;
  writeCommand(ST7789_POWSAVE);
  writeData(0xec|ns|is);
  if(mode&4) {
    writeCommand(ST7789_DLPOFFSAVE);
    writeData(0xfe);
  }
}

void drawChar(int16_t x, int16_t y, unsigned char c,  uint16_t color, uint16_t bg, uint8_t size) {
   // drawChar(x, y, c, color, bg, size, size);
   if(!gfxFont) { // 'Classic' built-in font

        if((x >= _width)            || // Clip right
           (y >= _height)           || // Clip bottom
           ((x + 6 * size_x - 1) < 0) || // Clip left
           ((y + 8 * size_y - 1) < 0))   // Clip top
            return;

        if(!_cp437 && (c >= 176)) c++; // Handle 'classic' charset behavior

        SPI_START;
        for(uint8_t i=0; i<5; i++ ) { // Char bitmap = 5 columns
            uint8_t line = pgm_read_byte(&font[c * 5 + i]);
            for(uint8_t j=0; j<8; j++, line >>= 1) {
                if(line & 1) {
                    if(size_x == 1 && size_y == 1)
                        writePixel(x+i, y+j, color);
                    else
                        writeFillRect(x+i*size_x, y+j*size_y, size_x, size_y, color);
                } else if(bg != color) {
                    if(size_x == 1 && size_y == 1)
                        writePixel(x+i, y+j, bg);
                    else
                        writeFillRect(x+i*size_x, y+j*size_y, size_x, size_y, bg);
                }
            }
        }
        if(bg != color) { // If opaque, draw vertical line for last column
            if(size_x == 1 && size_y == 1) writeFastVLine(x+5, y, 8, bg);
            else          writeFillRect(x+5*size_x, y, size_x, 8*size_y, bg);
        }
        SPI_END;
   }
else { // Custom font

        // Character is assumed previously filtered by write() to eliminate
        // newlines, returns, non-printable characters, etc.  Calling
        // drawChar() directly with 'bad' characters of font may cause mayhem!

        c -= (uint8_t)pgm_read_byte(&gfxFont->first);
        GFXglyph *glyph  = pgm_read_glyph_ptr(gfxFont, c);
        uint8_t  *bitmap = pgm_read_bitmap_ptr(gfxFont);

        uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
        uint8_t  w  = pgm_read_byte(&glyph->width),
                 h  = pgm_read_byte(&glyph->height);
        int8_t   xo = pgm_read_byte(&glyph->xOffset),
                 yo = pgm_read_byte(&glyph->yOffset);
        uint8_t  xx, yy, bits = 0, bit = 0;
        int16_t  xo16 = 0, yo16 = 0;

        if(size_x > 1 || size_y > 1) {
            xo16 = xo;
            yo16 = yo;
        }

        // Todo: Add character clipping here

        // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
        // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
        // has typically been used with the 'classic' font to overwrite old
        // screen contents with new data.  This ONLY works because the
        // characters are a uniform size; it's not a sensible thing to do with
        // proportionally-spaced fonts with glyphs of varying sizes (and that
        // may overlap).  To replace previously-drawn text when using a custom
        // font, use the getTextBounds() function to determine the smallest
        // rectangle encompassing a string, erase the area with fillRect(),
        // then draw new text.  This WILL infortunately 'blink' the text, but
        // is unavoidable.  Drawing 'background' pixels will NOT fix this,
        // only creates a new set of problems.  Have an idea to work around
        // this (a canvas object type for MCUs that can afford the RAM and
        // displays supporting setAddrWindow() and pushColors()), but haven't
        // implemented this yet.

        SPI_START;
        for(yy=0; yy<h; yy++) {
            for(xx=0; xx<w; xx++) {
                if(!(bit++ & 7)) {
                    bits = pgm_read_byte(&bitmap[bo++]);
                }
                if(bits & 0x80) {
                    if(size_x == 1 && size_y == 1) {
                        writePixel(x+xo+xx, y+yo+yy, color);
                    } else {
                        writeFillRect(x+(xo16+xx)*size_x, y+(yo16+yy)*size_y,
                          size_x, size_y, color);
                    }
                }
                bits <<= 1;
            }
        }
        SPI_END;

    } // End classic vs custom font
}

*/
/**************************************************************************/
size_t write(uint8_t c) {
    if(!gfxFont) { // 'Classic' built-in font

        if(c == '\n') {                        // Newline?
            cursor_x  = 0;                     // Reset x to zero,
            cursor_y += textsize_y * 8;        // advance y one line
        } else if(c != '\r') {                 // Ignore carriage returns
            if(wrap && ((cursor_x + textsize_x * 6) > _width)) { // Off right?
                cursor_x  = 0;                 // Reset x to zero,
                cursor_y += textsize_y * 8;    // advance y one line
            }
            drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x, textsize_y);
            cursor_x += textsize_x * 6;          // Advance x one char
        }

    } else { // Custom font

        if(c == '\n') {
            cursor_x  = 0;
            cursor_y += (int16_t)textsize_y *
                        (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        } else if(c != '\r') {
            uint8_t first = pgm_read_byte(&gfxFont->first);
            if((c >= first) && (c <= (uint8_t)pgm_read_byte(&gfxFont->last))) {
                GFXglyph *glyph  = pgm_read_glyph_ptr(gfxFont, c - first);
                uint8_t   w     = pgm_read_byte(&glyph->width),
                          h     = pgm_read_byte(&glyph->height);
                if((w > 0) && (h > 0)) { // Is there an associated bitmap?
                    int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
                    if(wrap && ((cursor_x + textsize_x * (xo + w)) > _width)) {
                        cursor_x  = 0;
                        cursor_y += (int16_t)textsize_y *
                          (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
                    }
                    drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x, textsize_y);
                }
                cursor_x += (uint8_t)pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize_x;
            }
        }

    }
    return 1;
}

void print(uint8_t *sentence)
{
	uint16_t sentence_len = strlen(sentence);
	int i = 0;
	    int i = 0;
	for (
	i = 0; i < sentence_len; i++)
	{
		int character = ((char)(data[i]) - ' ') * 5;
		write(character);
	}
}

void setTextSize(uint8_t s_x, uint8_t s_y) {
    textsize_x = (s_x > 0) ? s_x : 1;
    textsize_y = (s_y > 0) ? s_y : 1;
}


/**************************************************************************/
/*!
    @brief Set the font to display when print()ing, either custom or default
    @param  f  The GFXfont object, if NULL use built in 6x8 font
*/
/**************************************************************************/
void setFont(const GFXfont *f) {
    if(f) {            // Font struct pointer passed in?
        if(!gfxFont) { // And no current font struct?
            // Switching from classic to new font behavior.
            // Move cursor pos down 6 pixels so it's on baseline.
            cursor_y += 6;
        }
    } else if(gfxFont) { // NULL passed.  Current font struct defined?
        // Switching from new to classic font behavior.
        // Move cursor pos up 6 pixels so it's at top-left of char.
        cursor_y -= 6;
    }
    gfxFont = (GFXfont *)f;
}

/**********************************************************************/
  /*!
    @brief  Set text cursor location
    @param  x    X coordinate in pixels
    @param  y    Y coordinate in pixels
  */
  /**********************************************************************/
  void setCursor(int16_t x, int16_t y) { cursor_x = x; cursor_y = y; }

  /**********************************************************************/
  /*!
    @brief   Set text font color with transparant background
    @param   c   16-bit 5-6-5 Color to draw text with
    @note    For 'transparent' background, background and foreground
             are set to same color rather than using a separate flag.
  */
  /**********************************************************************/
  void setTextColor(uint16_t c) { textcolor = textbgcolor = c; }

  /**********************************************************************/
  /*!
    @brief   Set text font color with custom background color
    @param   c   16-bit 5-6-5 Color to draw text with
    @param   bg  16-bit 5-6-5 Color to draw background/fill with
  */
  /**********************************************************************/
  void setTextColor(uint16_t c, uint16_t bg) {
    textcolor   = c;
    textbgcolor = bg;
  }

  /**********************************************************************/
  /*!
  @brief  Set whether text that is too long for the screen width should
          automatically wrap around to the next line (else clip right).
  @param  w  true for wrapping, false for clipping
  */
  /**********************************************************************/
  void setTextWrap(bool w) { wrap = w; }

  /**********************************************************************/
  /*!
    @brief  Enable (or disable) Code Page 437-compatible charset.
            There was an error in glcdfont.c for the longest time -- one
            character (#176, the 'light shade' block) was missing -- this
            threw off the index of every character that followed it.
            But a TON of code has been written with the erroneous
            character indices. By default, the library uses the original
            'wrong' behavior and old sketches will still work. Pass
            'true' to this function to use correct CP437 character values
            in your code.
    @param  x  true = enable (new behavior), false = disable (old behavior)
  */
  /**********************************************************************/
  void cp437(boolean x=true) { _cp437 = x; }



  /************************************************************************/
  /*!
    @brief      Get width of the display, accounting for current rotation
    @returns    Width in pixels
  */
  /************************************************************************/
  uint16_t width(void) const { return _width; };

  /************************************************************************/
  /*!
    @brief      Get height of the display, accounting for current rotation
    @returns    Height in pixels
  */
  /************************************************************************/
  uint16_t height(void) const { return _height; }

  /************************************************************************/
  /*!
    @brief      Get rotation setting for display
    @returns    0 thru 3 corresponding to 4 cardinal rotations
  */
  /************************************************************************/
  uint8_t getRotation(void) const { return rotation; }

  // get current cursor position (get rotation safe maximum values,
  // using: width() for x, height() for y)
  /************************************************************************/
  /*!
    @brief  Get text cursor X location
    @returns    X coordinate in pixels
  */
  /************************************************************************/
  uint16_t getCursorX(void) const { return cursor_x; }

  /************************************************************************/
  /*!
    @brief      Get text cursor Y location
    @returns    Y coordinate in pixels
  */
  /************************************************************************/
  uint16_t getCursorY(void) const { return cursor_y; };



// end of code.
/*
 * File:   WS2812.c
 * Author: Ayodeji Ajayi
 *
 * Created on May 21, 2019, 5:37 AM
 */
#include <xc.h>
#include "WS2812.h"
#include <stdint.h>
#include <stdbool.h>

/* Define clock frequency*/
#ifndef _XTAL_FREQ
#define _XTAL_FREQ  32000000
#endif


volatile uint8_t data @ 0x31;          
//volatile uint8_t WS2812_PinHigh @ 0x6E;
//volatile uint8_t WS2812_PinLow @ 0x6D;

//extern volatile uint8_t grb[66];

void WS2812_Write(uint16_t led, uint8_t * buff)
{
    uint16_t ledMax = led * 3;
//    WS2812_PinLow = WS2812_PORT & (~(0x01 << WS2812_Pin));
//    WS2812_PinHigh = WS2812_PORT | (0x01 << WS2812_Pin);
    bool interruptStatus1 = INTCON0bits.GIEH;
    bool interruptStatus2 = INTCON0bits.GIEL;
    INTCON0bits.GIEH = 0;         //disable interrupts
    INTCON0bits.GIEL = 0;
    for (int i = 0; i < ledMax; i++)
    {
        data = buff[i];
#asm
     movlb 63;
     bsf WS2812_PORT,WS2812_Pin,1;
     nop;
     btfss _data,7,0;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     nop;
     nop;
     nop;
     nop;
     bsf WS2812_PORT,WS2812_Pin,1;
     nop;
     btfss _data,6,0;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     nop;
     nop;
     nop;
     nop;
     bsf WS2812_PORT,WS2812_Pin,1;
     nop;
     btfss _data,5,0;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     nop;
     nop;
     nop;
     nop;
     bsf WS2812_PORT,WS2812_Pin,1;
     nop;
     btfss _data,4,0;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     nop;
     nop;
     nop;
     nop;
     bsf WS2812_PORT,WS2812_Pin,1;
     nop;
     btfss _data,3,0;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     nop;
     nop;
     nop;
     nop;
     bsf WS2812_PORT,WS2812_Pin,1;
     nop;
     btfss _data,2,0;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     nop;
     nop;
     nop;
     nop;
     bsf WS2812_PORT,WS2812_Pin,1;
     nop;
     btfss _data,1,0;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     nop;
     nop;
     nop;
     nop;
     bsf WS2812_PORT,WS2812_Pin,1;
     nop;
     btfss _data,0,0;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     bcf WS2812_PORT,WS2812_Pin,1;
     nop;
     nop;
     nop;
     nop;
     nop;

#endasm

    }
    
    INTCON0bits.GIEH = interruptStatus1;
    INTCON0bits.GIEL = interruptStatus2;
    __delay_ms(1);
}



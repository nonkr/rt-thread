/*
 * Copyright (c) 2019, Billie Soong <nonkr@hotmail.com>
 * All rights reserved.
 *
 * This file is under GPL, see LICENSE for details.
 *
 * Author: Billie Soong <nonkr@hotmail.com>
 * Datetime: 2019-03-24 16:23
 *
 */

#ifndef __OLED_ZJY_H__
#define __OLED_ZJY_H__

#include <board.h>

//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL        0x00
#define XLevelH        0x10
#define Max_Column    128
#define Max_Row        64
#define    Brightness    0xFF
#define X_WIDTH    128
#define Y_WIDTH    64


#define OLED_D0_PIN  GET_PIN(A,  0)   // D0(CLK)
#define OLED_D1_PIN  GET_PIN(A,  1)   // D1(DIN)
#define OLED_RES_PIN GET_PIN(A,  2)   // RES
#define OLED_DC_PIN  GET_PIN(A,  3)   // DC
#define OLED_CS_PIN  GET_PIN(A,  4)   // CS


#define OLED_SCLK_Clr() rt_pin_write(OLED_D0_PIN, PIN_LOW)//CLK
#define OLED_SCLK_Set() rt_pin_write(OLED_D0_PIN, PIN_HIGH)

#define OLED_SDIN_Clr() rt_pin_write(OLED_D1_PIN, PIN_LOW)//DIN
#define OLED_SDIN_Set() rt_pin_write(OLED_D1_PIN, PIN_HIGH)

#define OLED_RST_Clr() rt_pin_write(OLED_RES_PIN, PIN_LOW)//RES
#define OLED_RST_Set() rt_pin_write(OLED_RES_PIN, PIN_HIGH)

#define OLED_DC_Clr() rt_pin_write(OLED_DC_PIN, PIN_LOW)//DC
#define OLED_DC_Set() rt_pin_write(OLED_DC_PIN, PIN_HIGH)

#define OLED_CS_Clr()  rt_pin_write(OLED_CS_PIN, PIN_LOW)//CS
#define OLED_CS_Set()  rt_pin_write(OLED_CS_PIN, PIN_HIGH)

#define OLED_CMD  0    //写命令
#define OLED_DATA 1    //写数据

//OLED控制用函数
void OLED_WR_Byte(rt_uint8_t dat, rt_uint8_t cmd);

void OLED_Display_On(void);

void OLED_Display_Off(void);

void OLED_Init(void);

void OLED_Clear(void);

void OLED_DrawPoint(rt_uint8_t x, rt_uint8_t y, rt_uint8_t t);

void OLED_Fill(rt_uint8_t x1, rt_uint8_t y1, rt_uint8_t x2, rt_uint8_t y2, rt_uint8_t dot);

void OLED_ShowChar(rt_uint8_t x, rt_uint8_t y, rt_uint8_t chr);

void OLED_ShowNum(rt_uint8_t x, rt_uint8_t y, rt_uint32_t num, rt_uint8_t len, rt_uint8_t size);

void OLED_ShowString(rt_uint8_t x, rt_uint8_t y, rt_uint8_t *p);

void OLED_Set_Pos(unsigned char x, unsigned char y);

void OLED_ShowCHinese(rt_uint8_t x, rt_uint8_t y, rt_uint8_t no);

void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);


#endif //__OLED_ZJY_H__

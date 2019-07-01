#ifndef __TM1640_H
#define __TM1640_H	 

#include <board.h>

#define TM1640_GPIOPORT	GPIOA	//定义IO接口
//#define TM1640_DIN	GPIO_Pin_12	//定义IO接口
//#define TM1640_SCLK	GPIO_Pin_11	//定义IO接口

#define TM1640_DIN      GET_PIN(A,  12)
#define TM1640_SCLK     GET_PIN(A,  11)

#define TM1640_LEDPORT	0xC8	//定义IO接口


void TM1640_Init(void);//初始化
void TM1640_led(rt_uint8_t date);//
void TM1640_display(rt_uint8_t address,rt_uint8_t date);//
void TM1640_display_add(rt_uint8_t address,rt_uint8_t date);//

		 				    
#endif

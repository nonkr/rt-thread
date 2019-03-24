/*
 * Copyright (c) 2019, Billie Soong <nonkr@hotmail.com>
 * All rights reserved.
 *
 * This file is under GPL, see LICENSE for details.
 *
 * Author: Billie Soong <nonkr@hotmail.com>
 * Datetime: 2019-03-24 17:12
 *
 */

#include "devices/OLED_zjy/oled_zjy.h"
#include "devices/OLED_zjy/bmp.h"

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       256
#define THREAD_TIMESLICE        5

static void oled_entry(void *parameter)
{
    OLED_Init(); //初始化OLED

    rt_uint8_t t = ' ';
    while (1)
    {
        OLED_Clear();

        OLED_ShowCHinese(0, 0, 0);  //中
        OLED_ShowCHinese(18, 0, 1); //景
        OLED_ShowCHinese(36, 0, 2); //园
        OLED_ShowCHinese(54, 0, 3); //电
        OLED_ShowCHinese(72, 0, 4); //子
        OLED_ShowCHinese(90, 0, 5); //科
        OLED_ShowCHinese(108, 0, 6);//技
        OLED_ShowString(0, 3, (rt_uint8_t *) "1.3' OLED TEST");
        //OLED_ShowString(8,2,"ZHONGJINGYUAN");
        //	OLED_ShowString(20,4,"2014/05/01");
        OLED_ShowString(0, 6, (rt_uint8_t *) "ASCII:");
        OLED_ShowString(63, 6, (rt_uint8_t *) "CODE:");
        OLED_ShowChar(48, 6, t);//显示ASCII字符
        t++;
        if (t > '~') t = ' ';
        OLED_ShowNum(103, 6, t, 3, 16);//显示ASCII字符的码值

        rt_thread_mdelay(5000);
        OLED_Clear();

        OLED_DrawBMP(0, 0, 128, 8, BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
        rt_thread_mdelay(5000);

        OLED_DrawBMP(0, 0, 128, 8, BMP2);
        rt_thread_mdelay(5000);
    }
}

void oled_sample(void)
{
    rt_thread_t tid = NULL;

    tid = rt_thread_create("oled",
                           oled_entry, RT_NULL,
                           THREAD_STACK_SIZE,
                           THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);
}

MSH_CMD_EXPORT(oled_sample, "oled sample")

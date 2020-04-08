/*
 * Copyright (c) 2019, Billie Soong <nonkr@hotmail.com>
 * All rights reserved.
 *
 * This file is under GPL, see LICENSE for details.
 *
 * Author: Billie Soong <nonkr@hotmail.com>
 * Datetime: 2019-06-30 21:46
 *
 */

#include <rtthread.h>
#include <board.h>
#include "devices/TM1640/TM1640.h"
#include "tm1640_sample.h"

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       160
#define THREAD_TIMESLICE        5

static void tm1640_entry(void *parameter)
{
    TM1640_Init();       //TM1640初始化

    //初始显示内容
    TM1640_display(0, 20);
    TM1640_display(1, 20);
    TM1640_display(2, 20);
    TM1640_display(3, 20);
    TM1640_display(4, 20);

    rt_thread_mdelay(10);

    rt_uint32_t count = 0;
    while (1)
    {
        TM1640_display(0, (rt_uint8_t) (count % 100000 / 10000));
        TM1640_display(1, (rt_uint8_t) (count % 10000 / 1000));
        TM1640_display(2, (rt_uint8_t) (count % 1000 / 100));
        TM1640_display(3, (rt_uint8_t) (count % 100 / 10));
        TM1640_display(4, (rt_uint8_t) (count % 10));
        count++;
        rt_thread_mdelay(500);
    }
}

void tm1640_sample(void)
{
    rt_thread_t tid = NULL;

    tid = rt_thread_create("tm1640", tm1640_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);
}

MSH_CMD_EXPORT(tm1640_sample, "tm1640 sample")

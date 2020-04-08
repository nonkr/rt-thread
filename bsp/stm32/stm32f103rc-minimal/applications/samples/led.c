/*
 * Copyright (c) 2019, Billie Soong <nonkr@hotmail.com>
 * All rights reserved.
 *
 * This file is under GPL, see LICENSE for details.
 *
 * Author: Billie Soong <nonkr@hotmail.com>
 * Datetime: 2019-03-23 16:50
 *
 */

#include <rtthread.h>
#include <board.h>

#define LED0_PIN    GET_PIN(A, 8)

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       256
#define THREAD_TIMESLICE        5

static void led_entry(void *parameter)
{
    rt_uint64_t count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (count++)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(100);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(100);
    }
}

void led_sample(void)
{
    rt_thread_t tid = NULL;

    tid = rt_thread_create("led",
                           led_entry, RT_NULL,
                           THREAD_STACK_SIZE,
                           THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);
}

MSH_CMD_EXPORT(led_sample, "led sample")

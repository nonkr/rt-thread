/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

//#include "samples/led_sample.h"
//#include "samples/oled_sample.h"
//#include "samples/tm1640_sample.h"
//#include "samples/uart_sample.h"
//#include "apps/uart1uart2switch/uart1uart2switch.h"
#include "apps/uart2nixietube/uart2nixietube.h"

int main(void)
{
//    led_sample();
//    oled_sample();
//    tm1640_sample();
//    uart_sample();
//    uart1_uart2_switch();
    uart2nixietube();

    while (1)
    {
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}

/*
 * Copyright (c) 2019, Billie Soong <nonkr@hotmail.com>
 * All rights reserved.
 *
 * This file is under GPL, see LICENSE for details.
 *
 * Author: Billie Soong <nonkr@hotmail.com>
 * Datetime: 2019-06-30 22:43
 *
 */

#include <rtthread.h>
#include <stdlib.h>
#include <string.h>
#include "uart2nixietube.h"
#include "devices/TM1640/TM1640.h"

struct rx_msg
{
    rt_device_t dev;
    rt_size_t   size;
};

static rt_device_t            uart1_serial;
static rt_device_t            uart2_serial;
static struct rt_messagequeue uart1_rx_mq;
static struct rt_messagequeue uart2_rx_mq;

void display_number(char *buff, rt_uint32_t size)
{
    rt_int32_t n  = 0;
    char       *p = NULL;

    if (buff && size >= 9 && (p = rt_strstr(buff, "TRACE[")) && (n = atol(p + 6)) > 0)
    {
        if (n < 10)
        {
            TM1640_display(0, 20);
            TM1640_display(1, 20);
            TM1640_display(2, 20);
            TM1640_display(3, 20);
        }
        if (n >= 10000) TM1640_display(0, (rt_uint8_t) (n % 100000 / 10000));
        if (n >= 1000) TM1640_display(1, (rt_uint8_t) (n % 10000 / 1000));
        if (n >= 100) TM1640_display(2, (rt_uint8_t) (n % 1000 / 100));
        if (n >= 10) TM1640_display(3, (rt_uint8_t) (n % 100 / 10));
        TM1640_display(4, (rt_uint8_t) (n % 10));
    }
}

/* 接收数据回调函数 */
static rt_err_t uart1_input(rt_device_t dev, rt_size_t size)
{
    struct rx_msg msg;
    rt_err_t      result;
    msg.dev  = dev;
    msg.size = size;

    result = rt_mq_send(&uart1_rx_mq, &msg, sizeof(msg));
    if (result == -RT_EFULL)
    {
        /* 消息队列满 */
        rt_kprintf("message queue full！\n");
    }
    return result;
}

static rt_err_t uart2_input(rt_device_t dev, rt_size_t size)
{
    struct rx_msg msg;
    rt_err_t      result;
    msg.dev  = dev;
    msg.size = size;

    result = rt_mq_send(&uart2_rx_mq, &msg, sizeof(msg));
    if (result == -RT_EFULL)
    {
        /* 消息队列满 */
        rt_kprintf("message queue full！\n");
    }
    return result;
}

static void uart1_thread_entry(void *parameter)
{
    struct rx_msg msg;
    rt_err_t      result;
    rt_uint32_t   rx_length;
    static char   rx_buffer[RT_SERIAL_RB_BUFSZ + 1];

    while (1)
    {
        rt_memset(&msg, 0, sizeof(msg));
        /* 从消息队列中读取消息*/
        result = rt_mq_recv(&uart1_rx_mq, &msg, sizeof(msg), RT_WAITING_FOREVER);
        if (result == RT_EOK)
        {
            /* 从串口读取数据*/
            rx_length = rt_device_read(msg.dev, 0, rx_buffer, msg.size);
            rx_buffer[rx_length] = '\0';

            display_number(rx_buffer, rx_length);

            /* 通过串口设备 serial 输出读取到的消息 */
            rt_device_write(uart2_serial, 0, rx_buffer, rx_length);
        }
    }
}

static void uart2_thread_entry(void *parameter)
{
    struct rx_msg msg;
    rt_err_t      result;
    rt_uint32_t   rx_length;
    static char   rx_buffer[RT_SERIAL_RB_BUFSZ + 1];

    while (1)
    {
        rt_memset(&msg, 0, sizeof(msg));
        /* 从消息队列中读取消息*/
        result = rt_mq_recv(&uart2_rx_mq, &msg, sizeof(msg), RT_WAITING_FOREVER);
        if (result == RT_EOK)
        {
            /* 从串口读取数据*/
            rx_length = rt_device_read(msg.dev, 0, rx_buffer, msg.size);
            rx_buffer[rx_length] = '\0';

            display_number(rx_buffer, rx_length);

            /* 通过串口设备 serial 输出读取到的消息 */
            rt_device_write(uart1_serial, 0, rx_buffer, rx_length);
        }
    }
}

static int uart1_init()
{
    static char msg_pool[RT_SERIAL_RB_BUFSZ + 1];

    /* 查找系统中的串口设备 */
    uart1_serial = rt_device_find("uart1");
    if (!uart1_serial)
    {
        rt_kprintf("find uart1 failed!\n");
        return RT_ERROR;
    }

    rt_mq_init(&uart1_rx_mq, "uart1_rx_mq",
               msg_pool,                 /* 存放消息的缓冲区 */
               sizeof(struct rx_msg),    /* 一条消息的最大长度 */
               sizeof(msg_pool),         /* 存放消息的缓冲区大小 */
               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */

    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(uart1_serial, RT_DEVICE_FLAG_DMA_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(uart1_serial, uart1_input);

    /* 创建 uart1 线程 */
    rt_thread_t thread = rt_thread_create("uart1", uart1_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        return RT_ERROR;
    }

    return RT_EOK;
}

static int uart2_init()
{
    static char msg_pool[RT_SERIAL_RB_BUFSZ + 1];

    /* 查找系统中的串口设备 */
    uart2_serial = rt_device_find("uart2");
    if (!uart2_serial)
    {
        rt_kprintf("find uart2 failed!\n");
        return RT_ERROR;
    }

    rt_mq_init(&uart2_rx_mq, "uart2_rx_mq",
               msg_pool,                 /* 存放消息的缓冲区 */
               sizeof(struct rx_msg),    /* 一条消息的最大长度 */
               sizeof(msg_pool),         /* 存放消息的缓冲区大小 */
               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */

    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(uart2_serial, RT_DEVICE_FLAG_DMA_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(uart2_serial, uart2_input);

    /* 创建 uart2 线程 */
    rt_thread_t thread = rt_thread_create("uart2", uart2_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        return RT_ERROR;
    }

    return RT_EOK;
}

void TM1640_reset()
{
    TM1640_display(0, 20);
    TM1640_display(1, 20);
    TM1640_display(2, 20);
    TM1640_display(3, 20);
    TM1640_display(4, 20);
}

int uart2nixietube(void)
{
    TM1640_Init();       //TM1640初始化

    //初始显示内容
    TM1640_reset();

    rt_thread_mdelay(10);

    rt_uint8_t i;
    for (i = 0; i <= 3; i++)
    {
        TM1640_display(0, i);
        TM1640_display(1, i);
        TM1640_display(2, i);
        TM1640_display(3, i);
        TM1640_display(4, i);
        rt_thread_mdelay(500);
    }

    TM1640_reset();

    if (uart1_init() != RT_EOK || uart2_init() != RT_EOK)
    {
        return RT_ERROR;
    }

    return RT_EOK;
}

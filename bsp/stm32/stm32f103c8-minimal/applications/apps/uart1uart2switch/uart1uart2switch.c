/*
 * Copyright (c) 2019, Billie Soong <nonkr@hotmail.com>
 * All rights reserved.
 *
 * This file is under GPL, see LICENSE for details.
 *
 * Author: Billie Soong <nonkr@hotmail.com>
 * Datetime: 2019-06-30 22:08
 *
 */

#include <rtthread.h>
#include "uart1uart2switch.h"

/* 用于接收消息的信号量 */
static struct rt_semaphore uart1_rx_sem;
static struct rt_semaphore uart2_rx_sem;
static rt_device_t         uart1_serial;
static rt_device_t         uart2_serial;

/* 接收数据回调函数 */
static rt_err_t uart1_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&uart1_rx_sem);

    return RT_EOK;
}

static rt_err_t uart2_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&uart2_rx_sem);

    return RT_EOK;
}

static void uart1_thread_entry(void *parameter)
{
    char ch;

    while (1)
    {
        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(uart1_serial, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&uart1_rx_sem, RT_WAITING_FOREVER);
        }
        rt_device_write(uart2_serial, 0, &ch, 1);
    }
}

static void uart2_thread_entry(void *parameter)
{
    char ch;

    while (1)
    {
        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(uart2_serial, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&uart2_rx_sem, RT_WAITING_FOREVER);
        }
        rt_device_write(uart1_serial, 0, &ch, 1);
    }
}

static int uart1_init()
{
    /* 查找系统中的串口设备 */
    uart1_serial = rt_device_find("uart1");
    if (!uart1_serial)
    {
        rt_kprintf("find uart1 failed!\n");
        return RT_ERROR;
    }

    /* 初始化信号量 */
    rt_sem_init(&uart1_rx_sem, "uart1_rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(uart1_serial, RT_DEVICE_FLAG_INT_RX);
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
    /* 查找系统中的串口设备 */
    uart2_serial = rt_device_find("uart2");
    if (!uart2_serial)
    {
        rt_kprintf("find uart2 failed!\n");
        return RT_ERROR;
    }

    /* 初始化信号量 */
    rt_sem_init(&uart2_rx_sem, "uart2_rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(uart2_serial, RT_DEVICE_FLAG_INT_RX);
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

int uart1_uart2_switch(void)
{
    if (uart1_init() != RT_EOK || uart2_init() != RT_EOK)
    {
        return RT_ERROR;
    }

    return RT_EOK;
}

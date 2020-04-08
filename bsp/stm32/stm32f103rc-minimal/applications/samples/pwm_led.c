/*
 * 程序清单：这是一个 PWM 设备使用例程
 * 例程导出了 pwm_led_sample 命令到控制终端
 * 命令调用格式：pwm_led_sample
 * 程序功能：通过 PWM 设备控制 LED 灯的亮度，可以看到LED不停的由暗变到亮，然后又从亮变到暗。
*/

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define LED_PIN_NUM         GET_PIN(A, 8)
#define PWM_DEV_NAME        "pwm1"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL     1       /* PWM通道 */

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       1024
#define THREAD_TIMESLICE        5

struct rt_device_pwm *pwm_dev;      /* PWM设备句柄 */

static void led_entry(void *parameter)
{
    rt_uint32_t period, pulse, dir;

    period = 50000;     /* 周期为50us，单位为纳秒ns */
    dir    = 1;         /* PWM脉冲宽度值的增减方向 */
    pulse  = 0;         /* PWM脉冲宽度值，单位为纳秒ns */

    // nonkr
    // 此处为修改官网的例子，因为在board/CubeMX_Config/Src/stm32f1xx_hal_msp.c的HAL_TIM_MspPostInit函数中已进行了对应的PIN脚的初始化
    /* 设置LED引脚脚模式为输出 */
//    rt_pin_mode(LED_PIN_NUM, PIN_MODE_OUTPUT);
    /* 拉高LED引脚 */
//    rt_pin_write(LED_PIN_NUM, PIN_HIGH);

    /* 查找设备 */
    pwm_dev = (struct rt_device_pwm *) rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        return;
    }

    /* 设置PWM周期和脉冲宽度默认值 */
    if (rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse) != RT_EOK)
    {
        rt_kprintf("rt_pwm_set failed! device %s!\n", PWM_DEV_NAME);
        return;
    }
    /* 使能设备 */
    if (rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL) != RT_EOK)
    {
        rt_kprintf("rt_pwm_enable failed! device %s!\n", PWM_DEV_NAME);
        return;
    }

    while (1)
    {
        rt_thread_mdelay(50);
        if (dir)
        {
            pulse += 5000;      /* 从0值开始每次增加5000ns */
        }
        else
        {
            pulse -= 5000;      /* 从最大值开始每次减少5000ns */
        }
        if (pulse >= period)
        {
            dir = 0;
        }
        if (0 == pulse)
        {
            dir = 1;
        }

        /* 设置PWM周期和脉冲宽度 */
        if (rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse) != RT_EOK)
        {
            rt_kprintf("rt_pwm_set failed! device %s!\n", PWM_DEV_NAME);
            return;
        }
    }
}

int pwm_led_sample(void)
{
    rt_thread_t tid = NULL;

    tid = rt_thread_create("pwm_led",
                           led_entry, RT_NULL,
                           THREAD_STACK_SIZE,
                           THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}

INIT_APP_EXPORT(pwm_led_sample);

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(pwm_led_sample, pwm sample);

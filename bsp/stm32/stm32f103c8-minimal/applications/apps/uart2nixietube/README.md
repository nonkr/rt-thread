由于使用的stm32f103c8t6最小系统板的uart3不能使用，只有uart1和uart2可以正常使用。所以这里需要关闭finsh和console
使用scons --menuconfig
```
关闭
RT-Thread Kernel --->
    Kernel Device Object --->
        [ ] Using console for rt_kprintf
            
关闭
RT-Thread Components --->
    Command shell --->
        [ ] finsh shell
        
修改UART DMA缓存大小
RT-Thread Components --->
    Device Drivers --->
        (512) Set RX buffer size
        
打开UART2
Hardware Drivers Config --->
    On-chip Peripheral Drivers --->
        [*] Enable UART --->
            [*] Enable UART2
            [*]   Enable UART2 RX DMA
``` 

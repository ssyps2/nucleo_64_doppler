## STM32 Nucleo-64: Doppler Effect Project

#### Cautions
There are some configurations need to be added after everytime generated code according to .ioc file.

1. Add the following line to Properties->C/C++ Build->Settings->Tool Settings->MCU GCC Linker->Libraries->Libraries (-l)

    :libarm_cortexM4lf_math.a

2. Add the following line to Properties->C/C++ Build->Settings->Tool Settings->MCU GCC Linker->Libraries->Libraries search path (-L)

    ../Middlewares/ST/ARM/DSP/Lib


#### User Driver File
Under **User** Directory.

-> adc_user.c/.h
Contains three methods of acquiring ADC data: Polling, Interrupt, DMA.
Each method could be selected by comment/uncomment the **Define** in header file.

    1. Polling
    FUNCTION: get_ADC1_data()
    Run in main(), while(1) loop.

    2. Interrupt
    CALLBACK: HAL_ADC_ConvCpltCallback()
    The function would be called evertime after the ADC convertion.
    A flag (IT_contine_flag) has been set to enable/disable the callback function.
    The callback function would be disabled after acquired 1024 (FFT Array Length) data, and re-enabled after the FFT calculation.

        !! Clock Prescaler has been set to be 64 to ensure the while(1) loop in main() could run.

    3. DMA
    Started by HAL_ADC_Start_DMA() function.
    External Trigger Conversion: Trigger on Timer 3 rising edge.

-> comparator.c/.h
External Interrupt of GPIO pin has been used to detect the square wave.
The callback fuction would be called everytime the pin detect a rising edge.
Timer 7 (1MHz) has been used as a more accurate tick (in us) to calculate square_wave_freq.

    CALLBACK: _Comparator_EXTI_Callback()
    Called in main.c, HAL_GPIO_EXTI_Callback().

-> display.c/.h
Contains LCD display and RS485 8-bit serial data transmition (by UART1).

    FUNCTION: LCD_Speed_Display()
              RS485_Data_Transmit()
    Called in main(), while(1) loop.

    CALLBACK: _Display_EXTI_Callback()
    Trigger by buttons rising edge. Pin 4 for mode switch, Pin 10 for unit switch.
    Called in main.c, HAL_GPIO_EXTI_Callback().

-> lcd.c/.h
LCD Driver found online.

-> processing.c/.h
FFT calculation part based on DSP Library.
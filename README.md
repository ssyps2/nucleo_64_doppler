## STM32 Nucleo-64: Doppler Effect Project

#### Overall Description
This project involves constructing a speed measurement module based on the Doppler Effect, specifically designed for bicycles. The core components include an STM32 microcontroller utilizing the HAL library, a Doppler Radar Module, and various electronic components such as filters, amplifiers, and comparators. The central principle relies on estimating the speed of a moving object by comparing the frequency of the transmitted and reflected radar signals.

A significant aspect of this project is the design and implementation of filtering systems to minimize noise interference from the Doppler Radar. Initially, a simple RC Passive Bandpass Filter was employed to process the radar's raw output signals. However, due to suboptimal performance, a more sophisticated Butterworth Active 4th Order Bandpass Filter using the Sallen Key Topology was later adopted, yielding much improved results.

After filtering, the signals are amplified to peak values of 3.3V. The STM32 microcontroller's ADC (Analog-to-Digital Converter) is then used to collect these signals. Different ADC methods such as Polling, Interrupt, and DMA were compared for this purpose. The Fast Fourier Transform (FFT) analysis is applied to convert time-domain signals into the frequency domain, enabling the calculation of the reflected signal's frequency and, subsequently, the object's speed.

The project also explores an alternative approach for frequency detection, involving the use of a Schmitt Trigger (a comparator topology with enhanced noise immunity), External Interrupt, and Timer for time measurement. This method proved to be simpler and more stable.

The CPLD (Complex Programmable Logic Device) aspect of the project was particularly challenging, involving digital design elements like Clock Dividers, Shift Registers, UART data parsing, and driving 7-segment LED displays. The development environment used for the CPLD was Xilinx ISE.

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

#### Pictures
![Controller_Circuit_Nucleo_64](https://github.com/ssyps2/nucleo_64_doppler/assets/72872247/7daf8e9c-62d3-41f9-a902-8f01e8652278)
![Display_Circuit_CPLD](https://github.com/ssyps2/nucleo_64_doppler/assets/72872247/3af9e337-7e89-4c07-b339-11041cd97bea)

/*
 * comparator.c
 *
 *  Created on: Nov 7, 2022
 *      Author: pengyuan
 */

//---------- Includes and Declarations ---------
#include "main.h"
#include "comparator.h"

extern TIM_HandleTypeDef htim7;


//-------------------- Defines ---------------------
#define TIM7_CNT_FREQ	 1e6


//-------------------- Variables --------------------
//served for square wave capture and freq determination
uint16_t last_tim7_cnt = 0, tim7_cnt = 0;
uint16_t square_wave_period=0, square_wave_freq=0;   //time in ns, freq in Hz


//-------------------- Functions --------------------
void _Comparator_EXTI_Callback(uint16_t GPIO_Pin)
{
	//require a more precise tick in micro-seconds
	//freq range of input square wave: 150~800Hz
	last_tim7_cnt = tim7_cnt;
	tim7_cnt = __HAL_TIM_GET_COUNTER(&htim7);

	//1MHz, reach 65535 in 0.06s => actual tolerance is 17Hz ~ interrupt_max_freq
	if(last_tim7_cnt > tim7_cnt)
	{
		square_wave_period = 65535-last_tim7_cnt+tim7_cnt;	//in us
	}
	else if(last_tim7_cnt < tim7_cnt)
	{
		square_wave_period = tim7_cnt - last_tim7_cnt;
	}

	if(square_wave_period != 0){
		square_wave_freq = TIM7_CNT_FREQ/square_wave_period;
	}
}


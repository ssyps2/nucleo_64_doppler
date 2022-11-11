/*
 * adc_user.c
 *
 *  Created on: Nov 7, 2022
 *      Author: pengyuan
 */

#include "adc_user.h"

int16_t  ADC_DATA_RD[1024] = { 0 };	//adc-data array
uint32_t adc_i=0;

uint8_t  IT_contine_flag=1;
int      ADC_Conv_Tick=0;

void User_ADC_Init(ADC_HandleTypeDef *hadc)
{
#ifdef ADC_IT_METHOD
	HAL_TIM_Base_Start(&htim3);
	HAL_ADC_Start_IT(&hadc1);
#endif

#ifdef ADC_DMA_METHOD
	HAL_TIM_Base_Start(&htim3);
	HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)ADC_DATA_RD,1024);
#endif

#ifdef ADC_POLLING_METHOD

#endif
}

//for polling method
int get_ADC1_data()
{
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1);
	uint16_t raw = (uint16_t)HAL_ADC_GetValue(&hadc1);

	return raw;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc==&hadc1 && IT_contine_flag==1)
	{
		ADC_DATA_RD[adc_i++] = (uint16_t)HAL_ADC_GetValue(&hadc1);

		if(adc_i==1024){
			IT_contine_flag=0;
		}
	}
	ADC_Conv_Tick++;
}

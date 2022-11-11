/*
 * adc_user.h
 *
 *  Created on: Nov 7, 2022
 *      Author: pengyuan
 */

#ifndef ADC_USER_H_
#define ADC_USER_H_

//------ select ADC acquiring mode here ------
#define ADC_DMA_METHOD
//#define ADC_IT_METHOD
//#define ADC_POLLING_METHOD

#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_adc1;

extern int16_t  ADC_DATA_RD[1024];
extern uint32_t adc_i;
extern uint8_t  IT_contine_flag;
extern int 		ADC_Conv_Tick;

#define ADC_MAX_VAL  4096	//12-bit long


void User_ADC_Init(ADC_HandleTypeDef *hadc);
int  get_ADC1_data();
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

#endif /* ADC_USER_H_ */

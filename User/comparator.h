/*
 * comparator.h
 *
 *  Created on: Nov 7, 2022
 *      Author: pengyuan
 */

#ifndef COMPARATOR_H_
#define COMPARATOR_H_


//served for square wave capture and freq determination
extern uint16_t square_wave_freq;   //time in ns, freq in Hz

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void _Comparator_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* COMPARATOR_H_ */

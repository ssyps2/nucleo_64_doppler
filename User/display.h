/*
 * display.h
 *
 *  Created on: Nov 9, 2022
 *      Author: pengyuan
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

extern int mode; //start in comparator mode
extern int unit_value;

void LCD_Init();
void _Display_EXTI_Callback(uint16_t GPIO_Pin);
void LCD_Speed_Display();

#endif /* DISPLAY_H_ */

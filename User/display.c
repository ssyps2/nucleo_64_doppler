/*
 * display.c
 *
 *  Created on: Nov 9, 2022
 *      Author: pengyuan
 */

//---------- Includes and Declarations ---------
#include <string.h>
#include "main.h"
#include "lcd.h"
#include "comparator.h"
#include "processing.h"

extern UART_HandleTypeDef huart1;


void Float_to_String_Conv(char* _flp_str, float num);
void encode_transmit_data(int input, uint8_t* output);


//-------------------- Variables --------------------
int mode=1; //start in comparator mode
int last_unit=0, unit_value=0;

char _flp_str[5]={'0'};	//xx.xx

//variables for LCD display
Lcd_PortType ports[] = { DB4_GPIO_Port, DB5_GPIO_Port, DB6_GPIO_Port, DB7_GPIO_Port };
Lcd_PinType pins[] = {DB4_Pin, DB5_Pin, DB6_Pin, DB7_Pin};

Lcd_HandleTypeDef lcd;

float comparator_speed_ms;
float fft_speed_ms;

char top_str[16];
char bottom_str[16];

//uart transmition
int 	trans_spd;
uint8_t trans_msg[1];

//-------------------- Functions --------------------
void LCD_Init()
{
	//init the LCD object
	lcd = Lcd_create(ports,pins,RS_GPIO_Port,RS_Pin,EN_GPIO_Port,EN_Pin,LCD_4_BIT_MODE);
}

void _Display_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == Mode_Switch_Pin)	//pin 4
	{
		mode = !mode;
	}
	else if(GPIO_Pin == Unit_Switch_Pin) //pin 10
	{
		last_unit=unit_value;
		if (unit_value == 3){
			unit_value = 0;
		}else{
			unit_value++;
		}
	}
}

void LCD_Speed_Display()
{
	comparator_speed_ms = (square_wave_freq*2.99)/211.74;
	fft_speed_ms = (fft_target_freq*2.99)/211.74;

	if(last_unit!=unit_value){
		Lcd_cursor(&lcd, 1,6);
		Lcd_string(&lcd, "          ");	//10 space
	}

	if(mode == 1){
		//comparator
		sprintf(top_str, "Comparator");
		switch (unit_value)
		{
		case 0:
			Float_to_String_Conv(_flp_str,comparator_speed_ms);
			sprintf(bottom_str, "Spd: %s m/s  ", _flp_str);
			trans_spd=comparator_speed_ms;
			break;
		case 1:
		    //convert to kph and display
			Float_to_String_Conv(_flp_str,comparator_speed_ms/3.6);
			sprintf(bottom_str, "Spd: %s km/h ", _flp_str);
			trans_spd=comparator_speed_ms/3.6;
			break;
		case 2:
		    //convert to mph
			Float_to_String_Conv(_flp_str,comparator_speed_ms/2.24);
			sprintf(bottom_str, "Spd: %s mph  ", _flp_str);
			trans_spd=comparator_speed_ms/2.24;
			break;
		default:
			Float_to_String_Conv(_flp_str,comparator_speed_ms);
		    sprintf(bottom_str, "Spd: %s m/s  ", _flp_str);
		    trans_spd=comparator_speed_ms;
		    break;
		}

		Lcd_cursor(&lcd, 0,0);
		Lcd_string(&lcd, top_str);
		Lcd_cursor(&lcd, 1,0);
		Lcd_string(&lcd, bottom_str);
	}

	else if(mode == 0){
		//ADC
		sprintf(top_str, "ADC       ");
		switch (unit_value)
		{
		case 0:
			Float_to_String_Conv(_flp_str,fft_speed_ms);
			sprintf(bottom_str, "Spd: %s m/s  ", _flp_str);
			trans_spd=fft_speed_ms;
			break;
		case 1:
		    //convert to kph and display
			Float_to_String_Conv(_flp_str,fft_speed_ms/3.6);
			sprintf(bottom_str, "Spd: %s km/h ", _flp_str);
			trans_spd=fft_speed_ms/3.6;
			break;
		case 2:
		    //convert to mph
			Float_to_String_Conv(_flp_str,fft_speed_ms/2.24);
			sprintf(bottom_str, "Spd: %s mph  ", _flp_str);
			trans_spd=fft_speed_ms/2.24;
			break;
		default:
			Float_to_String_Conv(_flp_str,fft_speed_ms);
		    sprintf(bottom_str, "Spd: %s m/s  ", _flp_str);
		    trans_spd=fft_speed_ms;
		    break;
		}

		Lcd_cursor(&lcd, 0,0);
		Lcd_string(&lcd, top_str);
		Lcd_cursor(&lcd, 1,0);
		Lcd_string(&lcd, bottom_str);
	}

	last_unit=unit_value;
}

void Float_to_String_Conv(char* _flp_str, float num)
{
	sprintf(_flp_str,"%02d.%d",(int)num/1,(int)(num*100)%100);
}

void RS485_Data_Transmit()
{
	encode_transmit_data(trans_spd,trans_msg);
	HAL_UART_Transmit(&huart1,trans_msg,1,1);
}

void encode_transmit_data(int input, uint8_t* output)
{
	if(input>=0 && input<10){
		*output = input;
	}
	else if(input>9 && input<100){
		*output = (input/10)<<4 | (input%10);
	}
}

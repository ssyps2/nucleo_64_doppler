#ifndef __FFT_PROCESSING_H
#define __FFT_PROCESSING_H

#define FFT_LENGTH	1024 		//FFT length


extern float fft_inputbuf[FFT_LENGTH*2];
extern float fft_outputbuf[FFT_LENGTH];

extern uint32_t fft_i;
extern uint32_t fft_target_freq;

void FFT_deal0(short int *data, float *fft_in, int data_length);
void User_FFT_Calculation(short int* ADC_DATA_RD,float* fft_inputbuf,float* fft_outputbuf,int* task_freq);

#endif

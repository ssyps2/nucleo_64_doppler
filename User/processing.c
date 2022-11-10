/**
 * This file is for ADC acquiring and FFT processing.
 */

//---------- Includes and Declarations ---------
#include "math.h"
#include "arm_math.h"
#include "processing.h"
#include "adc_user.h"

extern const arm_cfft_instance_f32 arm_cfft_sR_f32_len1024;

extern int ADC_Conv_Freq;


//-------------------- Variables --------------------
float fft_inputbuf[FFT_LENGTH*2];	//FFT input array //ADC acquire, add 0
float fft_outputbuf[FFT_LENGTH];	//FFT output array

float highest_amplitude=0;
uint32_t fft_i=0, fft_target_freq=0;


//-------------------- Functions --------------------
void FFT_deal0(short int *data, float *fft_in, int data_length)
{
	for(int i=0; i<data_length; i++)
	{
		fft_in[2*i] = data[i];
		fft_in[2*i+1]=0;
	}
}

void User_FFT_Calculation(short int* ADC_DATA_RD, float* fft_inputbuf, float* fft_outputbuf, int* task_freq)
{
	float sampling_freq=0;

	FFT_deal0(ADC_DATA_RD,fft_inputbuf,FFT_LENGTH);	//complete imaginary part, add 0 to ADC data
	arm_cfft_f32(&arm_cfft_sR_f32_len1024,fft_inputbuf,0,1); //FFT calculation
	arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);	//get the amplitude

	//find out the highest amplitude
	fft_outputbuf[0]=0;
	for(int k=1; k<FFT_LENGTH; k++) fft_outputbuf[k] /= 512;
	arm_max_f32(fft_outputbuf, FFT_LENGTH, &highest_amplitude, &fft_i);

#ifdef ADC_IT_METHOD
	sampling_freq = ADC_Conv_Freq;
#endif
#ifdef ADC_DMA_METHOD
	sampling_freq = 73846;
#endif
#ifdef ADC_POLLING_METHOD
	sampling_freq = (float)*task_freq*1.22;
#endif
	if(sampling_freq/1024*fft_i <= 1000)
		fft_target_freq = sampling_freq/1024*fft_i;	//target frequency
}

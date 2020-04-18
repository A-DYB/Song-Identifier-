/*
 ============================================================================
 Name        : FFT finder
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Find spectrogram of a song, find a match
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "FFT.h"




typedef double complex cpl;

void show(const char * s, cpl buf[], int size) {
	printf("%s", s);

	for (int i = 0; i < size; i++)
		if (!cimag(buf[i]))
			printf("%g ,", creal(buf[i]));
		else
			printf("(%g, %g) ", creal(buf[i]), cimag(buf[i]));
}




int main(void) {
	cpl x[] = {1, 1, 1, 1, 0, 0, 0, 0};
	//cpl x[] = {0, 0.5, 1, -0.5, -1, -0.5, 0, 0.5};

	int window_size = 8;

	//TODO need to also loop over each window

	int size = sizeof(x)/sizeof(x[0]);
	cpl output[window_size];

	show("Data: ", x, size);

	//create an output array
	//x is the data in the window range
	//check if the size of the current window is a power of 2, if not add zeros
	if(size != window_size){
		int pad = window_size-size;
		for(int i=0; i<size;i++){
			output[i] = x[i];
		}
		//fill remaining elements with 0
		for(int j =0; j<pad; j++){
			output[j+size-1]=0;
		}
	}else{
		for(int h=0;h<size;h++){
			output[h]=x[h];
		}
	}

	//pass the 1024 samples to apply window function and apply fft. Result goes in "output" array
	//output has magnitude of the nth frequency. kth output corresponds to the frequency: f_s*k/N, where N is the window size
	windowed_fft(output,window_size);


	//TODO need a loop to fill each element of the filtered result for each window
	filter(output);

	int output_size = sizeof(output)/sizeof(output[0]);
	show("\nFFT : ", output, output_size);

	return 0;
}






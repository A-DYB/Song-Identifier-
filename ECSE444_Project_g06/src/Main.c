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

void show(const char * s, cpl buf[]) {
	printf("%s", s);
	for (int i = 0; i < 8; i++)
		if (!cimag(buf[i]))
			printf("%g ,", creal(buf[i]));
		else
			printf("(%g, %g) ", creal(buf[i]), cimag(buf[i]));
}




int main(void) {
	cpl x[] = {1, 1, 1, 1, 0, 0, 0, 0};

	//TODO need to also loop over each window

	int size = sizeof(x)/sizeof(x[0]);
	cpl output[size];

	show("Data: ", x);

	//create an output array
	//x is the data in the window range
	//check if the size of the current window is a power of 2, if not add zeros
	if(size != 1024){
		int pad = 1024-size;
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
	windowed_fft(output,size);


	//TODO need a loop to fill each element of the filtered result for each window
	filter(output);


	show("\nFFT : ", output);

	return 0;
}






/*
 * FFT.c
 *
 *  Created on: Apr 17, 2020
 *      Author: Adrian Dybka
 */
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "FFT.h"

double PI= atan2(1, 1) * 4;
typedef double complex cpl;

void windowed_fft(cpl *output, int window){

	//Window function
	for(int i=0;i<window;i++){
		double hamming =0.5*(1-cos(2*PI*i/(window-1)));
		output[i]=output[i]*hamming;
	}

	sample_fft(output,window);

	//compute the magnitude of the complex result
	for(int i=0;i<window;i++){
		output[i]=cabs(output[i]);
	}


}
void sample_fft(cpl *output, int n)
{
	if( n <=1)
	{
		return;
	}
	int m = n/2;
	//create arrays with half the elements
	cpl *even = malloc((m)*sizeof(cpl));
	cpl *odd = malloc((m)*sizeof(cpl));

	//fill the smaller arrays for each even and odd element of the larger array
	for(int j=0;j<n;j+=2){
		even[j/2] = output[j];
		odd[j/2] = output[j+1];
	}

	//recurse fft for halved

	sample_fft(even, m);
	sample_fft(odd, m);

	//compute fft
	for(int k=0;k<m;k++){
		cpl e = cexp(-2*I*PI*k/n)*odd[k];
		output[k]=even[k]+e;
		output[m+k] =even[k]-e;
	}
	free(even);
	free(odd);
}


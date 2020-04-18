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

	// TODO apply window function here

	//TODO fft modifies arr which points to output if it is the correct size, not sure if that is ok
	sample_fft(output,window);

	//compute the magnitude in log scale
	/*
	for(int i=0;i<window;i++){
		//output[i]=20*log10(cabs(output[i]));
		output[i]=cabs(output[i]);
	}
	*/

}
void sample_fft(cpl *output, int n)
{
	if( n <=1)
	{
		return;
	}
	int m = n/2;
	//create arrays with half the elements
	cpl even[m];
	cpl odd[m];

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

}


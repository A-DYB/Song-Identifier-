/*
 * FFT_and_filter.c
 *
 * Author: Adrian Dybka
 */
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "FFT_and_filter.h"
#include "splice.h"
#include "FFT.h"
#include "filter.h"

typedef double complex cpl;
unsigned int first_iter=1;


void compute(int * input, int size, double output[][num_bands], int num_windows){

	if(size<1024){
		printf("Size of array is too small\n");
	}

	double (*fft_results)[halved_size] = malloc(sizeof(double[num_windows][halved_size]));
	cpl *subset;
	int start =0;
	int end=0;
	//loop over the windows and perform the ffts
	for(int i=0;i<num_windows;i++){
		end = start+window_size;
		subset = slice(input ,start,end);
		start = start+window_size/overlap;

		//pass the 1024 samples to apply window function and apply fft. Result goes in "subset" array
		//the result has the magnitude of the kth frequency. kth output corresponds to the frequency: f_s*k/N, where N is the window size
		windowed_fft(subset,window_size);

		//flag for precomputing the complex exponentials
		first_iter =0;

		//only half of the results are unique, since the input is real
		for(int j=0;j<halved_size;j++){
			fft_results[i][j]=subset[j];
		}
		free(subset);

	}

	filter(fft_results, output, num_windows);

	free(fft_results);

}

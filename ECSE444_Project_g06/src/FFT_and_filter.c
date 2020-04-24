/*
 * FFT_and_filter.c
 *
 *  Created on: Apr 23, 2020
 *      Author: Adrian Dybka
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

void show(const char * str, cpl buf[], int size) {
	printf("%s", str);
	for(int i = 0; i < size; i++){
		if(cimag(buf[i]) != 0)
			printf("%g ",creal(buf[i]));
		else
			printf("(%g, %g) ",creal(buf[i]),cimag(buf[i]));
	}
}

void compute(int * input, int size, double output[][num_bands], int num_windows){

	if(size<1024){
		printf("size too small\n");
	}

	double (*fft_results)[halved_size] = malloc(sizeof(double[num_windows][halved_size]));
	cpl *subset;
	int start =0;
	int end=0;
	for(int i=0;i<num_windows;i++){
		end = start+window_size;
		subset = slice(input ,start,end);
		//show("\nsubset: ", subset, 4);
		start = start+window_size/overlap;


		//pass the 1024 samples to apply window function and apply fft. Result goes in "output" array
		//output has magnitude of the kth frequency. kth output corresponds to the frequency: f_s*k/N, where N is the window size
		windowed_fft(subset,window_size);
		//show("\nfft: ", subset, 4);

		for(int j=0;j<halved_size;j++){

			fft_results[i][j]=subset[j];

		}
		free(subset);

	}

	FILE* fft_out =fopen("E:\\ECSE 444 MicroProcessors\\out.csv","w");

	if(fft_out!=NULL){
		for(int i=0;i<num_windows;i++){
			for(int j=0;j<halved_size;j++){
				fprintf(fft_out,"%f,",fft_results[i][j]);
			}
			fprintf(fft_out,"\n");
		}
		fclose(fft_out);
	}else{
		printf("NULL");
	}

	filter(fft_results, output, num_windows);

	free(fft_results);

}

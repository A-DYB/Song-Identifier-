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
#include "filter.h"
#include "splice.h"

typedef double complex cpl;
const int window_size = 1024;
const int halved_size = 512;
const int num_bands = 6;
const int overlap =2;



void show(const char * s, cpl buf[], int size) {
	printf("%s", s);

	for (int i = 0; i < size; i++)
		if (!cimag(buf[i]))
			printf("%g ,", creal(buf[i]));
		else
			printf("(%g, %g) ", creal(buf[i]), cimag(buf[i]));
}

//halved_size or num_bands
void print_2D(double arr[][num_bands], int num_wind){
	printf("\n");
	for(int i=0;i<num_wind ;i++){
		printf("%d (",i);
		for(int j=0;j<num_bands;j++){
			printf("%f (%d), ", arr[i][j],j);
		}
		printf("), \n");
	}

}



int main(void) {
	setvbuf (stdout, NULL, _IONBF, 0);
	printf("test\n");

	//input data
	cpl x[1024];

	for(int i=0;i<1024;i++){
		if(i<4){
			x[i]=1;
		}
		else
			x[i]=0;

	}

	int size = sizeof(x)/sizeof(x[0]);
	//TODO check if below is correct
	int num_windows = size/(window_size/overlap)-1;

	if(size<1024){
		printf("size too small\n");
	}

	/////////	FFT TEST
	//windowed_fft(x,8);
	//cpl output[window_size];
	//show("Data: ", x, size);
	//int output_size = sizeof(output)/sizeof(output[0]);
	//show("\nFFT : ", output, output_size);

	////////	FFT END TEST


	double (*fft_results)[halved_size] = malloc(sizeof(double[num_windows][halved_size]));
	cpl *subset;
	int start =0;
	int end=0;
	for(int i=0;i<num_windows;i++){
		end = start+window_size;
		subset = slice(x ,start,end);
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

	//print_2D(fft_results,num_windows);

	//declare 2D array
	double (*filtered_output)[num_bands] = malloc(sizeof(double[num_windows][num_bands]));

	filter(fft_results, filtered_output, num_windows);

	print_2D(filtered_output,num_windows);

	free(fft_results);

	return 0;
}






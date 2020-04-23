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

//size of 5 second clip 11025*5= 55125
//size of the song: 1695900
const int file_size =55125;
int x[55125];

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

	//AudioArray
	//sound_clip
	FILE* song =fopen("E:\\ECSE 444 MicroProcessors\\sound_clip.csv","r");

	if(song!=NULL){
		for(int b=0;b<file_size;b++){
			fscanf(song,"%d,",&x[b]);
		}
		fclose(song);
	}else{
		printf("NULL");
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

	//declare 2D array
	double (*filtered_output)[num_bands] = malloc(sizeof(double[num_windows][num_bands]));

	filter(fft_results, filtered_output, num_windows);

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

	FILE* filter_out =fopen("E:\\ECSE 444 MicroProcessors\\filter_out.csv","w");

	if(filter_out!=NULL){
		for(int i=0;i<num_windows;i++){
			for(int j=0;j<num_bands;j++){
				fprintf(filter_out,"%f,",filtered_output[i][j]);
			}
			fprintf(filter_out,"\n");
		}
		fclose(filter_out);
	}else{
		printf("NULL filt");
	}

	free(fft_results);

	return 0;
}






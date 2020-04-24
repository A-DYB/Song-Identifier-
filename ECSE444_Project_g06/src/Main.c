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
#include <time.h>
#include "FFT.h"
#include "filter.h"
#include "splice.h"
#include "FFT_and_filter.h"

typedef double complex cpl;
const int window_size = 1024;
const int halved_size = 512;
const int num_bands = 6;
const int overlap =2;

typedef struct Array{
	int* data;
	int size;
} Array;

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

void read_file(FILE* file, Array* arr){
	int n=0;
	int val=0;
	while(fscanf(file,"%d,",&val)==1){
		n++;
	}

	arr->data = malloc(n*sizeof(int));
	arr->size =n;
	rewind(file);
	for(int i=0;i<n;i++){
		fscanf(file,"%d,",&(arr->data[i]));
	}
	fclose(file);

}

void write_file_2D(FILE* file, double arr[][num_bands], int num_windows){

	if(file!=NULL){
		for(int i=0;i<num_windows;i++){
			for(int j=0;j<num_bands;j++){
				fprintf(file,"%f,",arr[i][j]);
			}
			fprintf(file,"\n");
		}
		fclose(file);
	}else{
		printf("Cannot write to file. Please close file.");
	}

}


int main(void) {
	setvbuf (stdout, NULL, _IONBF, 0);
	int size =0;
	clock_t start_t,end_t;
	////////// WHOLE SONG

	FILE* song =fopen("E:\\ECSE 444 MicroProcessors\\song_arr.csv","r");
	Array song_arr;
	read_file(song, &song_arr);
	size =song_arr.size;

	start_t =clock();
	int num_windows = size/(window_size/overlap)-1;
	double (*song_output)[num_bands] = malloc(sizeof(double[num_windows][num_bands]));
	compute(song_arr.data, size, song_output, num_windows);
	end_t =clock();

	printf("FFT and filter of whole song took %2.5fs\n",((double)end_t-(double)start_t)/CLOCKS_PER_SEC);

	FILE* song_filter_out =fopen("E:\\ECSE 444 MicroProcessors\\song_filter_out.csv","w");
	write_file_2D(song_filter_out,song_output,num_windows);

	free(song_output);

	///////////// SOUND CLIP

	FILE* clip =fopen("E:\\ECSE 444 MicroProcessors\\clip_arr.csv","r");
	Array clip_arr;
	read_file(clip, &clip_arr);
	size =clip_arr.size;

	start_t =clock();
	num_windows = size/(window_size/overlap)-1;
	double (*clip_output)[num_bands] = malloc(sizeof(double[num_windows][num_bands]));
	compute(clip_arr.data, size, clip_output, num_windows);
	end_t =clock();

	printf("FFT and filter of song clip took %2.5fs\n",((double)end_t-(double)start_t)/CLOCKS_PER_SEC);

	FILE* clip_filter_out =fopen("E:\\ECSE 444 MicroProcessors\\clip_filter_out.csv","w");
	write_file_2D(clip_filter_out,clip_output,num_windows);

	free(clip_output);

	return 0;
}






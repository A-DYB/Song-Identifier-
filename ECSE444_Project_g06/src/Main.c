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
#include "FFT_and_filter.h"

typedef double complex cpl;
const int window_size = 1024;
const int halved_size = 512;
const int num_bands = 6;
const int overlap =2;

//size of 5 second clip 11025*5= 55125
//size of the song: 1695900
//size 2min song: 1323000
const int song_size =1323000;
int song_arr[1323000];

const int clip_size =55125;
int clip_arr[55125];


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

	////////// WHOLE SONG

	FILE* song =fopen("E:\\ECSE 444 MicroProcessors\\song_arr.csv","r");

	if(song!=NULL){
		for(int b=0;b<song_size;b++){
			fscanf(song,"%d,",&song_arr[b]);
		}
		fclose(song);
	}else{
		printf("NULL");
	}

	int size = sizeof(song_arr)/sizeof(song_arr[0]);

	int num_windows = size/(window_size/overlap)-1;

	double (*song_output)[num_bands] = malloc(sizeof(double[num_windows][num_bands]));

	compute(song_arr, size, song_output, num_windows);

	FILE* song_filter_out =fopen("E:\\ECSE 444 MicroProcessors\\song_filter_out.csv","w");
	if(song_filter_out!=NULL){
		for(int i=0;i<num_windows;i++){
			for(int j=0;j<num_bands;j++){
				fprintf(song_filter_out,"%f,",song_output[i][j]);
			}
			fprintf(song_filter_out,"\n");
		}
		fclose(song_filter_out);
	}else{
		printf("NULL song filt");
	}

	free(song_output);

	///////////// SOUND CLIP

	FILE* clip =fopen("E:\\ECSE 444 MicroProcessors\\clip_arr.csv","r");
	if(clip!=NULL){
		for(int b=0;b<clip_size;b++){
			fscanf(clip,"%d,",&clip_arr[b]);
		}
		fclose(clip);
	}else{
		printf("NULL");
	}

	size = sizeof(clip_arr)/sizeof(clip_arr[0]);

	num_windows = size/(window_size/overlap)-1;

	double (*clip_output)[num_bands] = malloc(sizeof(double[num_windows][num_bands]));

	compute(clip_arr, size, clip_output, num_windows);

	FILE* clip_filter_out =fopen("E:\\ECSE 444 MicroProcessors\\clip_filter_out.csv","w");

	if(clip_filter_out!=NULL){
		for(int i=0;i<num_windows;i++){
			for(int j=0;j<num_bands;j++){
				fprintf(clip_filter_out,"%f,",clip_output[i][j]);
			}
			fprintf(clip_filter_out,"\n");
		}
		fclose(clip_filter_out);
	}else{
		printf("NULL clip filt");
	}

	free(clip_output);

	return 0;
}






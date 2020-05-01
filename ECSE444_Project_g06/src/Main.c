/*
 ============================================================================
 Name        : Song matcher
 Author      : Adrian Dybka
 Version     :
 Copyright   :
 Description : Find spectrogram of some songs and a clip, then find a match
 	 	 	 	 Select an optimization level and a sound clip from the provided csv files
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
#include "match.h"

//*********Optimization levels*************
//0: Debug/testing mode, has some extra prints to display "score" values for each song
//1: Standard Mode
//2: Precomputed exponentials
//3: Precomputed exponentials and higher FFT base case
const unsigned int optimization = 0;

typedef double complex cpl;
const int window_size = 1024;
const int halved_size = 512;
const int num_bands = 6;
const int overlap =2;

typedef struct Array{
	int* data;
	int size;
} Array;

typedef struct Scores{
	int max_score;
	char name[30];
} Score;

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
	printf("Optimization Level: %d\n", optimization);

	int size =0;
	clock_t start_t,end_t;

	setvbuf (stdout, NULL, _IONBF, 0);

	// ********* SELECT A SOUND CLIP **********, recorded from phone microphone. 44.1 kHz down-sampled to 11.025 kHz
	// Provided files: sad_clip_arr.csv, buddy_clip_arr.csv, epic_clip_arr.csv
	FILE* clip =fopen("E:\\ECSE 444 MicroProcessors\\sad_clip_arr.csv","r");
	Array clip_arr;
	read_file(clip, &clip_arr);

	// Getting array for the "sad" song from csv file
	FILE* sad =fopen("E:\\ECSE 444 MicroProcessors\\sad_arr.csv","r");
	Array sad_arr;
	read_file(sad, &sad_arr);

	// Getting array for the "buddy" song from csv file
	FILE* buddy =fopen("E:\\ECSE 444 MicroProcessors\\buddy_arr.csv","r");
	Array buddy_arr;
	read_file(buddy, &buddy_arr);

	// Getting array for the "epic" song from csv file
	FILE* epic =fopen("E:\\ECSE 444 MicroProcessors\\epic_arr.csv","r");
	Array epic_arr;
	read_file(epic, &epic_arr);

	//Sound clip
	size =clip_arr.size;
	start_t =clock();
	int num_windows_clip = size/(window_size/overlap)-1;
	double (*clip_output)[num_bands] = malloc(sizeof(double[num_windows_clip][num_bands]));
	//Find the filtered frequency spectrum over each time interval
	compute(clip_arr.data, size, clip_output, num_windows_clip);
	end_t =clock();
	double clip_time = ((double)end_t-(double)start_t)/CLOCKS_PER_SEC;

	//Sad song
	size =sad_arr.size;
	start_t =clock();
	int num_windows_sad = size/(window_size/overlap)-1;
	double (*sad_song_output)[num_bands] = malloc(sizeof(double[num_windows_sad][num_bands]));
	//Find the filtered frequency spectrum over each time interval
	compute(sad_arr.data, size, sad_song_output, num_windows_sad);
	end_t =clock();
	double sad_time = ((double)end_t-(double)start_t)/CLOCKS_PER_SEC;

	//Buddy song
	size =buddy_arr.size;
	start_t =clock();
	int num_windows_buddy = size/(window_size/overlap)-1;
	double (*buddy_song_output)[num_bands] = malloc(sizeof(double[num_windows_buddy][num_bands]));
	//Find the filtered frequency spectrum over each time interval
	compute(buddy_arr.data, size, buddy_song_output, num_windows_buddy);
	end_t =clock();
	double buddy_time = ((double)end_t-(double)start_t)/CLOCKS_PER_SEC;

	//Epic song
	size =epic_arr.size;
	start_t =clock();
	int num_windows_epic = size/(window_size/overlap)-1;
	double (*epic_song_output)[num_bands] = malloc(sizeof(double[num_windows_epic][num_bands]));
	//Find the filtered frequency spectrum over each time interval
	compute(epic_arr.data, size, epic_song_output, num_windows_epic);
	end_t =clock();
	double epic_time = ((double)end_t-(double)start_t)/CLOCKS_PER_SEC;

	//Find the best match between the songs and the sound clip using the filtered frequency spectrums
	start_t =clock();
	Score result;
	result= match(clip_output,num_windows_clip, sad_song_output,num_windows_sad, buddy_song_output,num_windows_buddy, epic_song_output,num_windows_epic);
	end_t =clock();
	double matching_time = ((double)end_t-(double)start_t)/CLOCKS_PER_SEC;

	/*
	printf("FFT and filter of song clip took %2.5fs\n",clip_time);
	printf("FFT and filter of the song 'sad' took %2.5fs\n",sad_time);
	printf("FFT and filter of the song 'buddy' took %2.5fs\n",buddy_time);
	printf("FFT and filter of the song 'epic' took %2.5fs\n",epic_time);
	*/
	printf("Total time for the FFTs: %2.5fs\n",clip_time+sad_time+buddy_time+epic_time);
	printf("\nMatching the clip with a song took %2.5fs\n",matching_time);
	printf("Most likely song is: %s\n",result.name);
	printf("\nTotal time elapsed: %2.5fs\n",clip_time+sad_time+buddy_time+epic_time+matching_time);

	//Write all the results to csv files to be visualized in another program (MATLAB/Excel etc)
	FILE* epic_filter_out =fopen("E:\\ECSE 444 MicroProcessors\\epic_filter_out.csv","w");
	write_file_2D(epic_filter_out,epic_song_output,num_windows_epic);

	FILE* buddy_filter_out =fopen("E:\\ECSE 444 MicroProcessors\\buddy_filter_out.csv","w");
	write_file_2D(buddy_filter_out,buddy_song_output,num_windows_buddy);

	FILE* sad_filter_out =fopen("E:\\ECSE 444 MicroProcessors\\sad_filter_out.csv","w");
	write_file_2D(sad_filter_out,sad_song_output,num_windows_sad);

	FILE* clip_filter_out =fopen("E:\\ECSE 444 MicroProcessors\\clip_filter_out.csv","w");
	write_file_2D(clip_filter_out,clip_output,num_windows_clip);

	free(epic_song_output);
	free(buddy_song_output);
	free(sad_song_output);
	free(clip_output);

	return 0;
}






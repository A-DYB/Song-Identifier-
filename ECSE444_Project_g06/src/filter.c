/*
 * filter.c
 *
 *  Created on: Apr 17, 2020
 *      Author: Adrian Dybka
 */


#include "filter.h"
#include <stdio.h>


//For each window, put the 512 bins in 6 logarithmic bands
//2.For each band keep the strongest bin of frequencies
//3. Compute average of the 6 strongest bins
//4.Keep the bins (from the 6 strongest bins) that are above the mean
void filter(double fft_result[][halved_size], double output[][num_bands], int num_windows){

	//double *band_max = malloc((6)*sizeof(double));
	//contains (max magnitude of band,freq @ max,time slice)
	double band_max[num_windows][num_bands][2];

	//holds the max for each band over the whole song
	double song_max[6];

	//output is 2D array, (6,#windows)

	//fft result should be size (#windows,freq_bins) where
	//freq bins is the window size/2 and
	//windows = #samples//(window_size*(1-overlap%))



	double mean =0;
	//Initialize the max array
	band_max[0][0][0] = fft_result[0][0];
	band_max[0][1][0] = fft_result[0][10];
	band_max[0][2][0] = fft_result[0][20];
	band_max[0][3][0] = fft_result[0][40];
	band_max[0][4][0] = fft_result[0][80];
	band_max[0][5][0] = fft_result[0][160];

	band_max[0][0][1] = 0;
	band_max[0][1][1] = 0;
	band_max[0][2][1] = 0;
	band_max[0][3][1] = 0;
	band_max[0][4][1] = 0;
	band_max[0][5][1] = 0;

	song_max[0] = fft_result[0][0];
	song_max[1] = fft_result[0][10];
	song_max[2] = fft_result[0][20];
	song_max[3] = fft_result[0][40];
	song_max[4] = fft_result[0][80];
	song_max[5] = fft_result[0][160];

	//loop over each time slice (window #)
	for(int i=0; i<num_windows; i++){
		//Use log bands 0,10; 10,20; 20,40; 40,80; 80,160; 60,511
		//We have 1024 freq bins, but the magnitudes are mirrored, so only half are unique
		//Use loop unrolling for optimization?
		for(int j=0;j<halved_size;j++){
			if(j<10){
				if(fft_result[i][j]>band_max[i][0][0]){
					band_max[i][0][0] = fft_result[i][j];
					band_max[i][0][1] = j;
				}
				if(fft_result[i][j]>song_max[0]){
					song_max[0]=fft_result[i][j];
				}
			}
			else if(j<20){
				if(fft_result[i][j]>band_max[i][1][0]){
					band_max[i][1][0] = fft_result[i][j];
					band_max[i][1][1] = j;
				}
				if(fft_result[i][j]>song_max[1]){
					song_max[1]=fft_result[i][j];
				}

			}
			else if(j<40){
				if(fft_result[i][j]>band_max[i][2][0]){
					band_max[i][2][0] = fft_result[i][j];
					band_max[i][2][1] = j;
				}
				if(fft_result[i][j]>song_max[2]){
					song_max[2]=fft_result[i][j];
				}
			}
			else if(j<80){
				if(fft_result[i][j]>band_max[i][3][0]){
					band_max[i][3][0] = fft_result[i][j];
					band_max[i][3][1] = j;
				}
				if(fft_result[i][j]>song_max[3]){
					song_max[3]=fft_result[i][j];
				}

			}
			else if(j<160){
				if(fft_result[i][j]>band_max[i][4][0]){
					band_max[i][4][0] = fft_result[i][j];
					band_max[i][4][1] = j;
				}
				if(fft_result[i][j]>song_max[4]){
					song_max[4]=fft_result[i][j];
				}

			}
			else if(j<=511){
				if(fft_result[i][j]>band_max[i][5][0]){
					band_max[i][5][0] = fft_result[i][j];
					band_max[i][5][1] = j;
				}
				if(fft_result[i][j]>song_max[5]){
					song_max[5]=fft_result[i][j];
				}

			}

			}
		}

	//find mean of the max magnitudes over the whole song
	for(int k=0;k<6;k++){
		printf("songmax %d : %f\n",k,song_max[k]);
		mean = mean + song_max[k];
	}
	mean = mean/6;


	for(int i=0;i<num_windows;i++){
		for(int j=0;j<6;j++){
			if(band_max[i][j][0]>mean){
				output[i][j]=band_max[i][j][1];
				//output[j][0][i]=band_max[j][1][i];
			}
			else{
				output[i][j]=-1;
			}
		}
	}


	//free(band_max);
}





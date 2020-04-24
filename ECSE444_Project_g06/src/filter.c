/*
 * filter.c
 *
 *  Created on: Apr 17, 2020
 *      Author: Adrian Dybka
 */


#include "filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//For each window, put the 512 bins in 6 logarithmic bands
//2.For each band keep the strongest bin of frequencies
//3. Compute average of the 6 strongest bins
//4.Keep the bins (from the 6 strongest bins) that are above the mean
void filter(double fft_result[][halved_size], double output[][num_bands], int num_windows){

	//double *band_max = malloc((6)*sizeof(double));
	//contains (max magnitude of band,freq @ max,time slice)
	double band_max[num_windows][num_bands][2];
	memset( band_max, 0, num_windows*num_bands*2*sizeof(double) );

	//holds the max for each band over the whole song
	double song_max[6] = {0};

	//output is 2D array, (6,#windows)

	//fft result should be size (#windows,freq_bins) where
	//freq bins is the window size/2 and
	//windows = #samples//(window_size*(1-overlap%))

	double mean =0;
	//test for diff mean
	double mean_arr[num_windows];
	memset( mean_arr, 0, num_windows*sizeof(double) );

	//loop over each time slice (window #)
	for(int i=0; i<num_windows; i++){
		//Use log bands 0,10; 10,20; 20,40; 40,80; 80,160; 160,511
		//We have 1024 freq bins, but the magnitudes are mirrored, so only half are unique
		//Use loop unrolling for optimization?
		for(int j=0;j<halved_size;j++){
			//0 Hz -> 107.6 Hz
			if(j<10){
				if(fft_result[i][j]>band_max[i][0][0]){
					band_max[i][0][0] = fft_result[i][j];
					band_max[i][0][1] = j;
				}
				if(fft_result[i][j]>song_max[0]){
					song_max[0]=fft_result[i][j];
				}
			}
			//107.6 Hz -> 215.3 Hz
			else if(j<20){

				if(fft_result[i][j]>band_max[i][1][0]){
					band_max[i][1][0] = fft_result[i][j];
					band_max[i][1][1] = j;
				}
				if(fft_result[i][j]>song_max[1]){
					song_max[1]=fft_result[i][j];
				}
			}
			//215.3 Hz -> 430.6 Hz
			else if(j<40){
				if(fft_result[i][j]>band_max[i][2][0]){
					band_max[i][2][0] = fft_result[i][j];
					band_max[i][2][1] = j;
				}
				if(fft_result[i][j]>song_max[2]){
					song_max[2]=fft_result[i][j];
				}
			}
			//430.6 Hz -> 816.3 Hz
			else if(j<80){
				if(fft_result[i][j]>band_max[i][3][0]){
					band_max[i][3][0] = fft_result[i][j];
					band_max[i][3][1] = j;
				}
				if(fft_result[i][j]>song_max[3]){
					song_max[3]=fft_result[i][j];
				}

			}
			//816.3 Hz -> 1722.6 Hz
			else if(j<160){
				if(fft_result[i][j]>band_max[i][4][0]){
					band_max[i][4][0] = fft_result[i][j];
					band_max[i][4][1] = j;
				}
				if(fft_result[i][j]>song_max[4]){
					song_max[4]=fft_result[i][j];
				}

			}
			//1722.6 Hz -> 5512.5 Hz
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
/*
	for(int k=0;k<6;k++){
		printf("songmax %d : %f\n",k,song_max[k]);
		mean = mean + song_max[k];
	}
	mean = 0.6*mean/6;
	printf("mean : %f\n",mean);

	for(int i=0;i<num_windows;i++){
		for(int j=0;j<6;j++){
			if(band_max[i][j][0]>mean){
				output[i][j]=band_max[i][j][1];
			}
			else{
				output[i][j]=0;
			}
		}
	}
*/


	//find mean of the max magnitudes over the 0.1s window for each window

	for(int i=0;i<num_windows;i++){
		for(int k=0;k<6;k++){
			mean_arr[i] = mean_arr[i] + band_max[i][k][0]/6;
		}
	}

	for(int i=0;i<num_windows;i++){
		for(int j=0;j<6;j++){
			if(band_max[i][j][0]>mean_arr[i]){
				output[i][j]=band_max[i][j][1];
			}
			else{
				output[i][j]=0;
			}
		}
	}

	//free(band_max);
}





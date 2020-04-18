/*
 * filter.c
 *
 *  Created on: Apr 17, 2020
 *      Author: Adrian Dybka
 */


#include "filter.h"
#include <stdio.h>

//For each FFT result, put the 512 bins in 6 logarithmic bands
void filter(double * input, int num_time_slices, double * output, int out_size){

	double slice_mean;
	//loop over each time slice
	for(int i=0; i<num_time_slices; i++){
		slice_mean =0;

		//loop over bands
		for(int j = 0; j<6; j++){

		}

	}

}

/*
 * splice.c
 *
 *  Created on: Apr 17, 2020
 *      Author: Adrian Dybka
 */

#include "splice.h"
#include <stdio.h>
#include <stdlib.h>

double * slice(double * array, int start, int end){

	int arraySize = sizeof(array)/sizeof(array[0]);

	//TODO don't forget to free the memory when the caller is done with this.
	double *subset = malloc((end-start)*sizeof(double));

	for(int k=start; k<=end;k++){
		subset[k] = array[k];
	}
	//caller uses double *sub; sub = slice(...); free(sub)
	return subset;
}

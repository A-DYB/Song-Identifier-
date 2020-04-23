/*
 * splice.c
 *
 *  Created on: Apr 17, 2020
 *      Author: Adrian Dybka
 */

#include "splice.h"
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

typedef double complex cpl;

cpl *slice(int * array, int start, int end){

	//int arraySize = sizeof(array)/sizeof(array[0]);

	//TODO don't forget to free the memory when the caller is done with this.
	cpl *subset = malloc((end-start)*sizeof(cpl));

	for(int k=0; k<end-start;k++){
		subset[k] = array[k+start];
	}
	//caller uses double *sub; sub = slice(...); free(sub)
	return subset;
}

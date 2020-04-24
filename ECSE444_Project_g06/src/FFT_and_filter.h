/*
 * FFT_and_filter.h
 *
 *  Created on: Apr 23, 2020
 *      Author: Adrian Dybka
 */

#ifndef FFT_AND_FILTER_H_
#define FFT_AND_FILTER_H_

extern const int num_bands;
extern const int halved_size;
extern const int window_size;
extern const int overlap;

void compute(int * , int, double [][num_bands], int);

#endif /* FFT_AND_FILTER_H_ */

/*
 * FFT_and_filter.h
 *
 * Author: Adrian Dybka
 */

#ifndef FFT_AND_FILTER_H_
#define FFT_AND_FILTER_H_

extern const int num_bands;
extern const int halved_size;
extern const int window_size;
extern const int overlap;

typedef double complex cpl;

void compute(int * , int, double [][num_bands], int);
void show(const char * , cpl [], int );

#endif /* FFT_AND_FILTER_H_ */

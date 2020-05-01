/*
 * FFT.h
 *
 * Author: Adrian Dybka
 */

#ifndef FFT_H_
#define FFT_H_

#include <complex.h>
typedef double complex cpl;
extern unsigned int first_iter;
extern const unsigned int optimization;
extern const int window_size;

void windowed_fft(complex *, int);
void sample_fft(complex *, int);
cpl* sample_fft_base_case(cpl *, int);
void sample_fft_precomp(cpl *, int);


#endif /* FFT_H_ */

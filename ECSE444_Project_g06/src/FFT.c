/*
 * FFT.c
 *
 * Author: Adrian Dybka
 */
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "FFT.h"
#include "splice.h"
#include <string.h>

double PI= atan2(1, 1) * 4;
typedef double complex cpl;
double c_exp[1022][1022];

double dft_exp[16][16];

void windowed_fft(cpl *output, int window){

	//Window function
	for(int i=0;i<window;i++){
		double hamming = 0.5*(1-cos(2*PI*i/(window-1)));
		output[i]=output[i]*hamming;
	}

	if( optimization ==1){
		sample_fft(output,window);
	}
	else if(optimization == 0 || optimization == 2){
		sample_fft_precomp(output,window);
	}
	else if(optimization == 3){
		cpl *res =output;
		res = sample_fft_base_case(output,window);

		for(int i=0;i<window;i++){
			output[i]=cabs(res[i]);
		}
		free(res);
	}

	//compute the magnitude of the complex result
	if(optimization != 3){
		for(int i=0;i<window;i++){
			output[i]=cabs(output[i]);
		}
	}



}
void sample_fft(cpl *output, int n)
{
	if( n <=1)
	{
		return;
	}
	int m = n/2;
	//create arrays with half the elements
	cpl *even = malloc((m)*sizeof(cpl));
	cpl *odd = malloc((m)*sizeof(cpl));

	//fill the smaller arrays for each even and odd element of the larger array
	for(int j=0;j<n;j+=2){
		even[j/2] = output[j];
		odd[j/2] = output[j+1];
	}

	//recurse fft for halved

	sample_fft(even, m);
	sample_fft(odd, m);


	//compute fft
	for(int k=0;k<m;k++){
		cpl e = cexp(-2*I*PI*k/n)*odd[k];
		output[k]=even[k]+e;
		output[m+k] =even[k]-e;
	}
	free(even);
	free(odd);


}

void sample_fft_precomp(cpl *output, int n)
{
	if( n <=1)
	{
		return;
	}
	int m = n/2;
	//create arrays with half the elements
	cpl *even = malloc((m)*sizeof(cpl));
	cpl *odd = malloc((m)*sizeof(cpl));

	//fill the smaller arrays for each even and odd element of the larger array
	for(int j=0;j<n;j+=2){
		even[j/2] = output[j];
		odd[j/2] = output[j+1];
	}

	//recurse fft for halved

	sample_fft_precomp(even, m);
	sample_fft_precomp(odd, m);


	//compute fft
	if(first_iter){

		for(int k=0;k<m;k++){
			c_exp[m+k-1][0] = cos(-2*PI*k/n);
			c_exp[m+k-1][1] = sin(-2*PI*k/n);
			cpl e = (c_exp[m+k-1][0]+I*c_exp[m+k-1][1])*odd[k];

			//cpl e = cexp(-2*I*PI*k/n)*odd[k];
			output[k]=even[k]+e;
			output[m+k] =even[k]-e;
		}

	}else{
		for(int k=0;k<m;k++){
			cpl e =(c_exp[m+k-1][0]+I*c_exp[m+k-1][1])*odd[k];
			//cpl e = cexp(-2*I*PI*k/n)*odd[k];
			output[k]=even[k]+e;
			output[m+k] =even[k]-e;
		}
	}

	free(even);
	free(odd);


}



cpl * sample_fft_base_case(cpl * input, int n)
{

	cpl *out = malloc(n*sizeof(cpl));
	memset( out, 0, n*sizeof(cpl) );
	int m = n/2;
	if( n <= 4)
	{

		if(first_iter){
			for(int j=0;j<n;j++){
				for(int k=0;k<n;k++){
					dft_exp[j*n+k][0] = cos(-2*PI*k*j/n);
					dft_exp[j*n+k][1] = sin(-2*PI*k*j/n);

					out[j] = out[j]+input[k]*(dft_exp[j*n+k][0]+I*dft_exp[j*n+k][1]);
				}
			}
		}else{
			for(int j=0;j<n;j++){
				for(int k=0;k<n;k++){
					out[j] = out[j]+input[k]*(dft_exp[j*n+k][0]+I*dft_exp[j*n+k][1]);
				}
			}
		}


		return out;

		//return input;
	}

	//create arrays with half the elements
	cpl *even = malloc((m)*sizeof(cpl));
	cpl *odd = malloc((m)*sizeof(cpl));

	//fill the smaller arrays for each even and odd element of the larger array
	for(int j=0;j<n;j+=2){
		even[j/2] = input[j];
		odd[j/2] = input[j+1];
	}

	//recurse fft for halved amount
	even = sample_fft_base_case(even, m);
	odd = sample_fft_base_case(odd, m);

	//compute fft
	if(first_iter){

		for(int k=0;k<m;k++){
			c_exp[m+k-1][0] = cos(-2*PI*k/n);
			c_exp[m+k-1][1] = sin(-2*PI*k/n);
			cpl e = (c_exp[m+k-1][0]+I*c_exp[m+k-1][1])*odd[k];

			//cpl e = cexp(-2*I*PI*k/n)*odd[k];
			out[k]=even[k]+e;
			out[m+k] =even[k]-e;
		}

	}else{
		for(int k=0;k<m;k++){
			cpl e =(c_exp[m+k-1][0]+I*c_exp[m+k-1][1])*odd[k];
			//cpl e = cexp(-2*I*PI*k/n)*odd[k];
			out[k]=even[k]+e;
			out[m+k] =even[k]-e;
		}
	}
	free(even);
	free(odd);

	return out;
}


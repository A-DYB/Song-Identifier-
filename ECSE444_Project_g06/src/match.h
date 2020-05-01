/*
 * match.h
 *
 * Author: Adrian Dybka
 */

#ifndef MATCH_H_
#define MATCH_H_

#include <complex.h>
typedef double complex cpl;

typedef struct Scores Score;

extern const int num_bands;
extern const unsigned int optimization;

Score match(double [][num_bands] ,int, double[][num_bands] ,int, double[][num_bands],int ,double[][num_bands],int );

#endif /* MATCH_H_ */

/*
 * filter.h
 *
 *  Created on: Apr 17, 2020
 *      Author: Adrian Dybka
 */

#ifndef FILTER_H_
#define FILTER_H_

extern const int num_bands;
extern const int halved_size;
extern const int window_size;

void filter(double[][num_bands], double [][num_bands] , int );

#endif /* FILTER_H_ */

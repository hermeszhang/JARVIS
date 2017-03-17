#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "xcorr.h"
/*	This is an implementation to calculate the autocorrelation
	of the signal y. To be used in EITN80 as part of the Schur
	algorithm.
	*/

// Assumes data is delivered as vector


void xcorr(float* x, float* Rx, int N, int n) 
{
	// x expected to be pointer to data block of size N, n correlation parameters returned
	int tau;			//Loop counter	
	float scale = 1.0/N;
	float Rs[N];
	//Rs[] = {0}
	// Calculate autocorrelations:
	for(tau = 0; tau < n; tau++) {
		shift(x, Rs, N, tau);
		Rx[tau] =scale*dot(x, Rs, N);
	}
}

float sum(float* x, int N)
{
	//Function to calculate sum of passed vector
	int i;
	float sum = 0.0;
	//size_t N = sizeof(x)/sizeof(x[0]);
	for(i = 0; i < N; i++) {
		sum += x[i];
	}
	return sum;
}

float dot(float* x, float* y, int X){
	int i;
	float dot = 0.0;
	//size_t X = sizeof(x)/sizeof(x[0]);
	//size_t Y = sizeof(Y)/sizeof(y[0]);
	
	for(i = 0; i < X; i++) {
		dot += x[i]*y[i];
	}
	return dot;
}

void shift(float* x, float* Rs, int N,  int tau)
{
	/*
	Help function to shift vector x tau steps. Pad first tau - 1 
	steps with zeros. To be used in xcorr.
	*/
	int i,j;

	for(i = 0; i < tau; i++) {
		Rs[i] = 0.0;
	}
	for(j = tau; j < N; j++) {
		Rs[j] = x[j-tau];
	}
}

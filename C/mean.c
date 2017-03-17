#include "framework.h"
#include "math.h"
#include "stdio.h"
float mean(float tmp[DSP_BLOCK_SIZE]){
	/*
	Calculates the averge magnitude of input block tmp
	*/ 
	
	int n;
	float sum, mean;
	sum =0;
	for(n = 0; n<DSP_BLOCK_SIZE; ++n){
		sum = sum + fabs((tmp[n]));
	}
	mean = sum/320;
	
	return mean;
}

#include "framework.h"
#include "mean.h"
#include <stdio.h>


struct detect Speech(float noise_level,float samples[DSP_BLOCK_SIZE]){	
	/*
		Detects when the speech starts. returns a struct detect with 2 values: result 0 or 1, and noise_level which is the reference noise level
		Arguments: noise_level: reference noise level. samples: current block of samples.
	*/
	struct detect rmp;
	int result = 0;	
	int i;
	float tmp = mean(samples);
	
	//printf("%f\n", tmp);
	
	if (tmp>3.5*noise_level){
		result = 1;
	}
	noise_level = 0.5*(tmp+noise_level);
	
	rmp.result = result;
	rmp.noise_level = noise_level;
	
	return rmp;
	
}

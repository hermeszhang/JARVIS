#include "framework.h"
#include "mean.h"
#include "stdio.h"


struct detectEnd EndOfSpeech(float noise_level, int counter, float samples[DSP_BLOCK_SIZE]){
	/*
		Detects when the speech has ended. Returns a struct detectEnd with three values; result, speech_level and counter
		Inputs are: given noise level used as reference, counter: number of blocks below thr, and the current sample block. 
	
	*/
	float speech_level;
	float thr = 0.001; // Noise-level, may be changed
	int cnt, result; // result = 0 if speech, cnt nmbr of blocks below thr
	detectEnd r2;
	cnt = counter;
	float tmp = mean(samples);// Calculate average magnitude for given block
	result  = 0;
	speech_level = 0.5*(noise_level+tmp);
	if(tmp<2*thr){
		cnt = cnt + 1;
		result = 1;
	}
	else{
		result = 0;
		cnt = 0;
		
	}
	
	r2.result = result;
	r2.speech_level = speech_level;
	r2.counter = cnt;
	return r2;
	
}

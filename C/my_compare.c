#include "framework.h"
#include "stdio.h"
#include "schur.h"
#include "my_compare.h"

void my_compare(float (*R1)[SIZE], float (*R2)[SIZE], float (*R3)[SIZE], float (*R4)[SIZE], float (*Rx)[SIZE])
{
	/*
		Compare 3 matrices, the two in the database and Rx, which is the coeffiecent matrix for recorded audio.
		Depending on which is best match the diodes light up.
		Calculates the sum of the squared errors    
	*/
	float e1 = 0.0;
	float e2 = 0.0;
	float e3 = 0.0;
	float e4 = 0.0;
	float abs = 10.0;
	
	//float thr = 20; // Maxium error that is allowed, may be changed in the future

	e1 = my_sse(R1, Rx);
	e2 = my_sse(R2, Rx);
	e3 = my_sse(R3, Rx);
	e4 = my_sse(R4, Rx);
	
	//quot = quotient(e1, e2, e3);
	printf("e1:%f \t e2: %f \t e3: %f \t, e4: %f\n: ",e1, e2, e3, e4);
	
	//printf("Quotient = %f\n", quot);
	
	if(e1<e3 && e1<e2  && e1<e4 && e1 < abs){
		dsp_set_leds(DSP_D3);
	}else if(e2<e1 && e2<e3 && e2<e4 && e2 < abs){
		dsp_set_leds(DSP_D4);	
	}else if(e3<e1 && e3<e2 && e3<e4 && e3 < abs){
		dsp_set_leds(DSP_D5);
	}else if(e4<e1 && e4<e2 && e4 <e3 && e4 < abs){
		dsp_set_leds(DSP_D6); 
	}else {
		//No match, light all LEDs
		dsp_set_leds(DSP_D4 | DSP_D5 | DSP_D6);
		//dsp_set_leds(DSP_D5);
		//dsp_set_leds(DSP_D6);
	}

}

float my_sse(float (*R)[SIZE], float (*Rx)[SIZE])
{	
	int k, i;
	float error = 0.0;
	for(i = 0; i<SIZE; i++){
		for(k = 0; k<SIZE; k++){
			error += (R[i][k]-Rx[i][k])*(R[i][k]-Rx[i][k]); // Calculates the sum of the squared errors
		}
	}
	return error;
}

float quotient(float e1, float e2, float e3)
{
	//Decide quotient between best and next-to-best matches
	float runnerup = 0.0;
	float winner = 0.0;
	if (e1 < e2 && e1 <e3) {
		winner = e1;
		if(e2<e3){
			runnerup = e2;
		}else{
			runnerup = e3;
		}
		return winner/runnerup;
	}
	if (e2 < e1 && e2 <e3) {
		winner = e2;
		if(e1<e3){
			runnerup = e1;
		}else{
			runnerup = e3;
		}
		return winner/runnerup;
	}
	winner = e3;
	if(e2<e1){
		runnerup = e2;
	} else{
		runnerup = e1;
	}
	return winner/runnerup;
}

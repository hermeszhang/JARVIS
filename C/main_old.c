#include <processor_include.h>
#include <signal.h>
#include <string.h>
#include <filter.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "framework.h"
#include "speechRecognition.h"
#include "xcorr.h"
#include "schur.h"
#include "reflect.h"
#include "pre_emph.h"
#include "EndofSpeech.h"
#include "Speech.h"
#include "mean.h"
#include "buff.h"
#include "my_compare.h"
//#include "database.h"


static float mic1[DSP_BLOCK_SIZE];
static float mic2[DSP_BLOCK_SIZE];
static float mic3[DSP_BLOCK_SIZE];
static float mic4[DSP_BLOCK_SIZE];
static float y[DSP_BLOCK_SIZE];
//static int state;
static float old_coeff[SIZE];
static float older_coeff[SIZE];
static float oldest_coeff[SIZE];
static float coeff[SIZE];
static float vec[SIZE];
static float R[SIZE][SIZE];
static float K[SIZE][MAX_BLOCK_SIZE];
static float database_matrix[SIZE][SIZE];
static float NOISE_LEVEL = 0.001;
static float thr = 0.001;
static int k;
static int n;
static int col;
detect r;
detectEnd r2;
static float last_data;
int cnt;
static int presscounter;
static int keypress;
static float q;
static int nbrPresses;
float mikael 		[SIZE][SIZE] = {-0.629521, -0.706726, -0.561511, -0.296399, -0.315744, -0.289648, -0.396475, -0.503993, -0.367182, -0.080755,0.299623, 0.296269, 0.256748, 0.334069, 0.251734, 0.260876, 0.465382, 0.358148, 0.233829, 0.238536, 0.020643, -0.120655, -0.256025, -0.126121, 0.013553, -0.064460, -0.043885, -0.073663, -0.150086, -0.028370, 0.171099, 0.267801, 0.130993, 0.160923, 0.185253, 0.158891, 0.156835, 0.083664, 0.072471, 0.181032, -0.292263, -0.281784, -0.218563, -0.185570, -0.105280, -0.110342, -0.139629, -0.148786, -0.159273, -0.132148, 0.067609, 0.062117, 0.096656, 0.090160, 0.117925, 0.137304, 0.112723, 0.094831, 0.087829, 0.146581, -0.110229, -0.167387, -0.151803, -0.103760, -0.140963, -0.112035, -0.078726, -0.043209, -0.074038, -0.046067, 0.084522, 0.123158, 0.069640, 0.056990, 0.085195, 0.126209, 0.096180, 0.156794, 0.107043, 0.079073, -0.135317, -0.160410, -0.079748, -0.119120, -0.139127, -0.083814, -0.165517, -0.127157, -0.022252, -0.083308, -0.045988, 0.062093, 0.076865, -0.007722, 0.007920, 0.068299, -0.007171, 0.104534, 0.151215, 0.079392};
float swartling 	[SIZE][SIZE] = {0};
float hts			[SIZE][SIZE] = {0};



void process(int sig)
{
    sample_t *u30 = dsp_get_audio_u30();    /* line 2 in without mic bias, no out */
    sample_t *u31 = dsp_get_audio_u31();    /* line 1 in with mic bias, no out */
    sample_t *u32 = dsp_get_audio_u32();    /* mic 1 and 2 in, headset out */
    sample_t *u33 = dsp_get_audio_u33();    /* mic 3 and 4 in, no out */
	
    
    for(n=0; n<DSP_BLOCK_SIZE; ++n) {
        mic1[n] = u32[n].right;
        mic2[n] = u32[n].left;
        mic3[n] = u33[n].right;
        mic4[n] = u33[n].left;
    }
    
    for(n=0; n<DSP_BLOCK_SIZE; ++n) {
        u32[n].right = mic1[n] + mic2[n];
        u32[n].left  = mic3[n] + mic4[n];
    }
    
   
    if (r.result == 0 && keypress == 1) { // State 0, no speech 
    	last_data = pre_emph(mic1, DSP_BLOCK_SIZE, last_data, y);
    	circ_buff(coeff, old_coeff, older_coeff, oldest_coeff);
   		schur(y,coeff, DSP_BLOCK_SIZE, SIZE); // calculate the reflection coefficients
   		r = Speech(NOISE_LEVEL, y);
   		dsp_set_leds(DSP_D3);
   		NOISE_LEVEL = thr;
   		//printf("\n NOISE_LEVEL %f",NOISE_LEVEL);
   		col = 0;
   	} 
    
   	
   	if(r.result == 1 && keypress == 1) { // state 1, speech detected
   		dsp_set_leds(DSP_D4); //signals speech detected	
   		++k;			// Increase the counter
   		if(k==1){
   			fill_K(K,old_coeff, older_coeff, oldest_coeff);
   			//print_matrix(10,10,150,K);
   			col = 3;
   		}
   		last_data = pre_emph(mic1, DSP_BLOCK_SIZE, last_data, y);
   		schur(y,coeff, DSP_BLOCK_SIZE, SIZE); // calculate the reflections coefficients.
   		for(n=0;n<SIZE;++n) {
	   		K[n][col] = coeff[n];
   		}
   		++col;
   		r2 = EndOfSpeech(NOISE_LEVEL,cnt, y); // Check if there is still speech
   		NOISE_LEVEL = r2.speech_level;
   		cnt = r2.counter; 
   		if((cnt>5 && r2.result == 1 && k>45) || k>149){ // conditions for when it should go to next state, when the speech has ended
   			r.result = 2;
   			cnt = 0;
   			r2.counter = 0;
   			NOISE_LEVEL= thr;
   			
   			//printf("r:%d \n", r.result);
   		}
   	}
   	
    if(r.result == 2 && keypress == 1)
    {	
    	//dsp_set_leds(3);
    	int j; //column count
    	int row;
    	int col;
    	reflect(k,K,R);
    	//print_matrix(10,10,10,R);
    	dsp_set_leds(DSP_D6);
    	for(row = 0; row <SIZE; ++row){
   			for(col = 0; col<SIZE; ++col){
   				database_matrix[row][col] += R[row][col];
   			}
   		}
		
   		if(presscounter == nbrPresses){
   			NOISE_LEVEL = 1000; // DO THIS WHEN BUILDING DATABASE
    		thr = 1000; //DO THIS WHEN BUILDING DATABASE
   			print_data(database_matrix, q);	//Print matrix to data base
   		}
   		
    	//stretch to 10x10 matrix, compare in database, show result
    	r.result = 0;
    	keypress = 0;
    	k=0; //Reset the counter
    	
    	//print_matrix(SIZE, SIZE, SIZE, mikael);
    	my_compare(mikael, swartling, hts, R);
    	keypress = 0;
    
    	
    		
    }
    
   	
    
   	
}

static void keyboard(int sig)
{
    unsigned int keys = dsp_get_keys();
    unsigned int leds = 0;
	
    //Set timer interrupt, handle keyboard interrupt in timer interrupt

    
    keypress = 1;  // To make sure that it doesn't record when we don't want it to 
	++presscounter;
	printf("%d \n", presscounter);
    
	if(keys & DSP_SW1) {
        leds = 0;
    } else if(keys & DSP_SW2) {
        leds = DSP_D1;
    } else if(keys & DSP_SW3) {
        leds = DSP_D2;
    } else if(keys & DSP_SW4) {
        leds = DSP_D1|DSP_D2;
    }
    
    dsp_set_leds(leds);
}

static void timer(int sig)
{
}

void main()
{   
    /*
    Setup the DSP framework.
    */
    dsp_init();
    //dsp_set_leds(DSP_D1 | DSP_D2);
    dsp_set_leds(DSP_D1);
    
    //state s (0 = listening, 1 = audio detected, computing reflection coeffs, 2 = end of audio, process phase)
	last_data = 0;
	col = 0;
	k = 0; //counter for iterations when speech is detected
	cnt = 0;
	r.result = 0;
	nbrPresses = 24;
	q = 1.0/nbrPresses;
		

   	/*
   	int c = 1, d = 1, e= 1;
 	printf("\n start");
   	for ( c = 1 ; c <= 32700 ; c++ )
       	for ( d = 1 ; d <= 20000 ; d++ )
       	{
       		for(e=1;e<20000;e++)
       		{}
       	}
 
   	
	*/
	
    /*
    Register interrupt handlers:
    SIG_SP1: the audio callback
    SIG_USR0: the keyboard callback
    SIG_TMZ: the timer callback
    */
    interrupt(SIG_SP1, process);
    interrupt(SIG_USR0, keyboard);
    interrupt(SIG_TMZ, timer);
    timer_set(9830400, 9830400);
    /* timer_on(); */

    /*
    Start the framework.
    */
    dsp_start();
    
    /*
    Everything is handled by the interrupt handlers, so just put an empty
    idle-loop here. If not, the program falls back to an equivalent idle-loop
    in the run-time library when main() returns.
    */
    for(;;) {
        idle();
    }
}


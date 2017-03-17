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
#include "print_data.h"
#include "database.h"


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
    
   
    if (r.result == 0) { // State 0, no speech 
    	last_data = pre_emph(mic1, DSP_BLOCK_SIZE, last_data, y);
    	circ_buff(coeff, old_coeff, older_coeff, oldest_coeff);
   		schur(y,coeff, DSP_BLOCK_SIZE, SIZE); // calculate the reflection coefficients
   		r = Speech(NOISE_LEVEL, y);
   		dsp_set_leds(DSP_D1);
   		NOISE_LEVEL = thr;
   		//printf("\n NOISE_LEVEL %f",NOISE_LEVEL);
   		col = 0;
   	} 
    
   	
   	if(r.result == 1) { // state 1, speech detected
   		dsp_set_leds(DSP_D2); //signals speech detected	
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
   	
    if(r.result == 2 )
    {	
    	//dsp_set_leds(3);
    	int j; //column count
    	int row;
    	int col;
    	
    	//stretch to 10x10 matrix, compare in database, show result
    	reflect(k,K,R);
    	
    	
    	
    	/* Use for database build  	
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
   		
   		*/
   		//Check for match
   		my_compare(mikael, swartling, screen, bb, R);
    	
    	//Show result on LED array
		
    	interrupt(SIG_SP1, SIG_IGN);
    	timer_set(100000000, 100000000);
		timer_on();
		
		
		//Reset conditions
    	r.result = 0;
    	keypress = 0;
    	k=0;  		
    }
}

static void keyboard(int sig)
{
	//Set timer to avoid debouncing of mechanical buttons
	timer_set(100000000,100000000);
	timer_on(); 
	printf("Keypress\n");
	  
}

static void timer(int sig)
{
    
    // Use this to build data base
    /*timer_off(); 	//Turn timer off when handling button press
    
    unsigned int keys = dsp_get_keys();
    unsigned int leds = 0;
	
    //Set timer interrupt, handle keyboard interrupt in timer interrupt
	printf("timer \n");
    
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
    
    //End use this to build database
	
    */ 
    //Use below to run speech recognition
    timer_off(); 	//Turn timer off when handling button press
    interrupt(SIG_SP1, process); //Re-enable sound processing  
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
	
	/*use below to build database*/
	nbrPresses = 9;
	q = 1.0/nbrPresses;
		

    /*
    Register interrupt handlers:
    SIG_SP1: the audio callback
    SIG_USR0: the keyboard callback
    SIG_TMZ: the timer callback
    */
    interrupt(SIG_SP1, process);
    interrupt(SIG_USR0, keyboard);
    interrupt(SIG_TMZ, timer);
   
    //timer_on();
   

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


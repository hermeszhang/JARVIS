#ifndef framework_h_included
#define framework_h_included

/* Configuration */
#define DSP_SAMPLE_RATE         16000
#define DSP_BLOCK_SIZE          320
#define DSP_INPUT_GAIN          20 //20 dB originally
#define DSP_OUTPUT_ATTENUATION  0

//Own definitions
#define SIZE					10
#define MAX_BLOCK_SIZE			150
/* Constants */
#define DSP_FREQUENCY           (12288000*16/2)
#define DSP_D1                  0x00000001
#define DSP_D2                  0x00000002
#define DSP_D3					0x00000004
#define DSP_D4					0x00000008
#define DSP_D5					0x00000010
#define DSP_D6					0x00000020

#define DSP_SW1                 0x00000008
#define DSP_SW2                 0x00000004
#define DSP_SW3                 0x00000002
#define DSP_SW4                 0x00000001

/* Sample frame */
typedef struct {
    _Fract left;
    _Fract right;
} sample_t;

typedef struct detectEnd{
	int result;
	int counter;
	float speech_level;
} detectEnd;
typedef struct detect{
	int result;
	float noise_level;
} detect;

/* Functions */
void dsp_init(void);
void dsp_start(void);
void dsp_stop(void);
sample_t *dsp_get_audio_u30(void);
sample_t *dsp_get_audio_u31(void);
sample_t *dsp_get_audio_u32(void);
sample_t *dsp_get_audio_u33(void);
unsigned int dsp_get_keys(void);
unsigned int dsp_get_cycles(void);
void dsp_set_timer_period(unsigned int value);
void dsp_set_leds(unsigned int value);

#endif

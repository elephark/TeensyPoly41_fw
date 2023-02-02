#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce2.h>
#include <Math.h>

#include "custom_waveforms.h"

// How many voices does this synth have?
#define NUM_VOICES      16
// How many indicator LEDs are we working with?
#define NUM_LEDS        8
// How many buttons are there?
#define NUM_BUTTONS     8

// We mix things down with AudioMixer4 units. To make sure we have enough, we have to deal with
// the silliness of integer division. We might have to do something similar again if we go to
// more than 16 voices.
#define NUM_MIXERS      ((NUM_VOICES + 3) / 4)


void muxRead();

// debug lol
String str;

class Voice {
  public:
  AudioSynthWaveform      lfoA;
  AudioEffectEnvelope     lfoAenv;
  AudioSynthWaveform      lfoB;
  AudioMixer4             modMix;
  AudioSynthWaveformModulated sub;
  AudioSynthWaveformModulated vcoA;
  AudioSynthWaveformModulated vcoB;
  AudioSynthWaveformModulated vcoC;
  AudioMixer4             voiceMix;
  AudioSynthWaveformDc    dc;
  AudioEffectEnvelope     filterEnv;
  AudioMixer4             filterMix;
  AudioFilterStateVariable filter;
  AudioMixer4             filterMode;
  AudioEffectEnvelope     env;

  AudioAmplifier          lfoAamp;
  AudioMixer4             lfoAmpMixer;
  AudioSynthWaveformDc    dcPositive;
  AudioEffectMultiply     voiceOut;
  
  AudioConnection*        patchCord1;
  AudioConnection*        patchCord2;
  AudioConnection*        patchCord3;
  AudioConnection*        patchCord4;
  AudioConnection*        patchCord5;
  AudioConnection*        patchCord6;
  AudioConnection*        patchCord8;
  AudioConnection*        patchCord9;
  AudioConnection*        patchCord10;
  AudioConnection*        patchCord56;
  AudioConnection*        patchCord57;
  AudioConnection*        patchCord58;
  AudioConnection*        patchCord59;
  AudioConnection*        patchCord60;
  AudioConnection*        patchCord86;
  AudioConnection*        patchCord92;
  AudioConnection*        patchCord98;
  AudioConnection*        patchCord104;
  AudioConnection*        patchCord110;
  AudioConnection*        patchCord111;
  AudioConnection*        patchCord122;

  AudioConnection*        patchCord123;
  AudioConnection*        patchCord124;
  AudioConnection*        patchCord125;
  AudioConnection*        patchCord126;
  AudioConnection*        patchCord127;

  bool envOn = false;
  int noteFreq;

  Voice() {
    patchCord1 = new AudioConnection(lfoA, lfoAenv);
    patchCord2 = new AudioConnection(lfoAenv, 0, modMix, 1);
    patchCord3 = new AudioConnection(lfoAenv, 0, vcoB, 0);
    patchCord4 = new AudioConnection(lfoAenv, 0, vcoC, 0);
    patchCord5 = new AudioConnection(lfoAenv, 0, sub, 0);
    patchCord6 = new AudioConnection(lfoAenv, 0, filterMix, 1);
    patchCord8 = new AudioConnection(lfoB, 0, vcoA, 1);
    patchCord9 = new AudioConnection(lfoB, 0, vcoB, 1);
    patchCord10 = new AudioConnection(modMix, 0, vcoA, 0);    
    patchCord56 = new AudioConnection(sub, 0, voiceMix, 3);
    patchCord57 = new AudioConnection(vcoA, 0, voiceMix, 0);
    patchCord58 = new AudioConnection(vcoB, 0, voiceMix, 1);
    patchCord59 = new AudioConnection(vcoB, 0, modMix, 0);
    patchCord60 = new AudioConnection(vcoC, 0, voiceMix, 2);
    patchCord86 = new AudioConnection(voiceMix, 0, filter, 0);
    patchCord92 = new AudioConnection(dc, filterEnv);
    patchCord98 = new AudioConnection(filterEnv, 0, filterMix, 0);
    patchCord104 = new AudioConnection(filterMix, 0, filter, 1);
    patchCord110 = new AudioConnection(filter, 0, filterMode, 0);
    patchCord111 = new AudioConnection(filter, 1, filterMode, 1);
    patchCord122 = new AudioConnection(filterMode, env);

    patchCord123 = new AudioConnection(lfoA, lfoAamp);
    patchCord124 = new AudioConnection(lfoAamp, 0, lfoAmpMixer, 0);
    patchCord125 = new AudioConnection(dcPositive, 0, lfoAmpMixer, 1);
    

    patchCord126 = new AudioConnection(lfoAmpMixer, 0, voiceOut, 0);
    patchCord127 = new AudioConnection(env, 0, voiceOut, 1);


    lfoAamp.gain(0);
    dcPositive.amplitude(1);
  }
};

Voice voice[NUM_VOICES];

// This is where we keep track of voice allocation in polyphonic mode.
uint8_t voiceOrder[NUM_VOICES];


AudioAnalyzeRMS          lfoAread1;           //xy=320.53334045410156,248.9333620071411
AudioMixer4              mix[NUM_MIXERS];

AudioMixer4              finalMix; //xy=1795.9338989257812,854.7668190002441
AudioFilterStateVariable dlyFiltR; //xy=2003.8004722595215,1181.3002490997314
AudioFilterStateVariable dlyFiltL;        //xy=2015.1337928771973,942.633547782898
AudioEffectDelay         dlyL;         //xy=2128.244888305664,1040.9224643707275
AudioEffectDelay         dlyR; //xy=2127.77779006958,1274.4259929656982
AudioMixer4              dlyMixR; //xy=2128.934093475342,1174.700201034546
AudioMixer4              dlyMixL; //xy=2139.48970413208,936.1446781158447
AudioEffectFreeverb      reverb;      //xy=2204.4674758911133,620.5891056060791
AudioMixer4              fxL; //xy=2379.023365020752,785.255838394165
AudioMixer4              fxR; //xy=2379.023235321045,907.2558078765869
AudioOutputI2S           i2s1;           //xy=2549.5332946777344,853.2666549682617


AudioConnection          patchCord12(voice[0].lfoAenv, lfoAread1);

AudioConnection*         voiceOutPatchCord[NUM_VOICES];

AudioConnection*         finalMixPatchCord[NUM_MIXERS];


AudioConnection          patchCord136(finalMix, 0, fxL, 0);
AudioConnection          patchCord137(finalMix, reverb);
AudioConnection          patchCord138(finalMix, 0, fxR, 0);
AudioConnection          patchCord139(finalMix, 0, dlyMixL, 0);
AudioConnection          patchCord140(finalMix, 0, dlyMixR, 0);
AudioConnection          patchCord141(dlyFiltR, 0, dlyMixR, 1);
AudioConnection          patchCord142(dlyFiltL, 0, dlyMixL, 1);
AudioConnection          patchCord143(dlyL, 0, dlyFiltL, 0);
AudioConnection          patchCord144(dlyR, 0, dlyFiltR, 0);
AudioConnection          patchCord145(dlyMixR, 0, fxR, 2);
AudioConnection          patchCord146(dlyMixR, dlyR);
AudioConnection          patchCord147(dlyMixL, 0, fxL, 2);
AudioConnection          patchCord148(dlyMixL, dlyL);
AudioConnection          patchCord149(reverb, 0, fxL, 1);
AudioConnection          patchCord150(reverb, 0, fxR, 1);
AudioConnection          patchCord151(fxL, 0, i2s1, 0);
AudioConnection          patchCord152(fxR, 0, i2s1, 1);
// GUItool: end automatically generated code



#include <MIDI.h>
//#include <EEPROMex.h>
#include <EEPROM.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

//////// Multiplexer //////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

const int MUX_A = 8;
const int MUX_B = 9;
const int MUX_C = 10;

const int MUX0_X = A0;
const int MUX1_X = A1;
const int MUX2_X = A2;
const int MUX3_X = A3;
const int MUX4_X = 5;  // switches
const int MUX5_X = 6;  // switches
const int MUX6_X = 22; // mem buttons
const int MUX7_X = 23; // LED outputs

//const int muxPots = A15;
//float wait = 1; //mux read delay

int led[NUM_LEDS] = {0,0,0,0,0,0,0,0};

// There are also two other LEDs that don't go through the mux.
const int LED_MEM_ACTIVE = 11;
const int LED_MEM_STORE = 12;

int memActive = 0;
int memStore = 0;

unsigned long muxInterval = 3;
unsigned long prevMuxTimer;
int curMux = 0;

Bounce* btns[NUM_BUTTONS];

// Stuff read in from the multiplexers

int pot_mainVol;
int pot_portTime;
int pot_oscAVol;
int pot_crossMod;
int pot_subVol;
int pot_oscBVol;
int pot_oscBTune;
int pot_oscCVol;
int pot_oscCShape;
int pot_oscCTune;
int pot_filtCutoff;
int pot_filtRes;
int pot_filtEnvAmt;
int pot_filtA;
int pot_filtD;
int pot_filtS;
int pot_filtR;
int pot_envA;
int pot_envD;
int pot_envS;
int pot_envR;
int pot_lfoFreq;
int pot_lfoDepth;
int pot_lfoA;
int pot_lfoD;
int pot_lfoS;
int pot_pwmFreq;
int pot_pwmDepth;
int pot_revAmt;
int pot_revSize;
int pot_dlyAmt;
int pot_dlyRate;

int sw_mainOctHi;
int sw_mainOctLo;
int sw_oscAShapeHi;
int sw_oscAShapeLo;
int sw_oscBOctHi;
int sw_oscBOctLo;
int sw_filtMode;
int sw_isMonophonic;
int sw_oscCOctHi;
int sw_oscCOctLo;
int sw_oscBShapeHi;
int sw_oscBShapeLo;
int sw_lfoShapeHi;
int sw_lfoShapeLo;
int sw_lfoDestHi;
int sw_lfoDestLo;

int btn_memActive;
int btn_memStore;
int btn_mem6;      // lol maybe array this
int btn_mem5;
int btn_mem4;
int btn_mem3;
int btn_mem2;
int btn_mem1;
int btn_mem[6];    // lol magic number

bool memJustLoaded;

uint8_t lastMemLoaded = 0;


// MIDI channel selection pins
const int ChSel1 = 29;
const int ChSel2 = 30;
const int ChSel4 = 31;
const int ChSel8 = 32;

int midiChannel;

///// notes, frequencies, voices /////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const float noteFreqs[128] = {
  8.176, 8.662, 9.177, 9.723, 10.301, 10.913, 11.562, 12.25, 
  12.978, 13.75, 14.568, 15.434, 16.352, 17.324, 18.354, 19.445, 
  20.602, 21.827, 23.125, 24.5, 25.957, 27.5, 29.135, 30.868, 
  32.703, 34.648, 36.708, 38.891, 41.203, 43.654, 46.249, 48.999, 
  51.913, 55, 58.27, 61.735, 65.406, 69.296, 73.416, 77.782, 
  82.407, 87.307, 92.499, 97.999, 103.826, 110, 116.541, 123.471, 
  130.813, 138.591, 146.832, 155.563, 164.814, 174.614, 184.997, 195.998, 
  207.652, 220, 233.082, 246.942, 261.626, 277.183, 293.665, 311.127, 
  329.628, 349.228, 369.994, 391.995, 415.305, 440, 466.164, 493.883, 
  523.251, 554.365, 587.33, 622.254, 659.255, 698.456, 739.989, 783.991, 
  830.609, 880, 932.328, 987.767, 1046.502, 1108.731, 1174.659, 1244.508, 
  1318.51, 1396.913, 1479.978, 1567.982, 1661.219, 1760, 1864.655, 1975.533, 
  2093.005, 2217.461, 2349.318, 2489.016, 2637.02, 2793.826, 2959.955, 3135.963, 
  3322.438, 3520, 3729.31, 3951.066, 4186.009, 4434.922, 4698.636, 4978.032, 
  5274.041, 5587.652, 5919.911, 6271.927, 6644.875, 7040, 7458.62, 7902.133, 
  8372.018, 8869.844, 9397.273, 9956.063, 10548.08, 11175.3, 11839.82, 12543.85
};


int voices;


// For monophonic mode, we'll be doing last-note priority today.
// To do this, we use a stack sorta thingy.
#define MONO_STACK_SIZE 64 // How many notes can we stack?
//uint8_t unisonVoiceCount = 1; // How many voices do we use in unison mode?
const int8_t MONO_OFF = -1;

int8_t monoStack[MONO_STACK_SIZE];
int8_t monoSP = 0;




//int vol of vcos.
float vcoVol = 0.07; // lol should be const
float mainVol;




float pitchBend = 1.0;

typedef struct { // lol pack this better, maybe also change types

  //voice mixer
  float vcoAvol = 0.5;
  float vcoBvol = 0.5;
  float vcoCvol = 0.5;
  float Subvol = 0.5;
  
  float crossMod;
  
  //Tunings, octaves etc.
  float octave = 1;
  float octaveB = 1;
  float octaveC = 1;
  float tuneB = 1;
  float tuneC = 1;
  
  //vco shapes
  int shapeA;
  int shapeB;
  int shapeC = 1;
  
  
  //filter
  int cut = 2000;
  float res = 1;
  int filtAtt;
  int filtDec;
  int filtRel;
  float filtAmt;
  
  int filterMode;
  
  
  //envelope
  int envAtt;
  int envDec;
  int envRel;
  float envSus; // lol why is this float and not the others?
  
  //lfo A
  float lfoAamp;
  float lfoAfreq;
  int lfoAdel;
  int lfoAatt;
  int lfoAdec;
  int lfoArel;
  float lfoAsus;
  
  int lfoAshape;
  int lfoAdest;
  
  //lfo B
  float lfoBamp;
  float lfoBfreq;
  
  
  //delay
  float dlyAmt;
  float dlyTimeL;
  float dlyTimeR;
  
  //reverb
  float revMix;
  float revSize;

  int isMonophonic = 0;
  int portTime;

} VoiceSettings;

VoiceSettings vs;



//tremolo
float ampMod;
unsigned long tremTimerInterval = 2;
unsigned long tremTimer;


float outGain = 0.8; // lol make this const?



//turncheck
///////////////////////////////////////////////////////////////////////////////////////////////////////////
float thresh = 10;
float thresh2 = 20;

unsigned long timer = 6;
unsigned long prevTimer;


int tuneBpot;
int oldTuneBpot;
int tuneCpot;
int oldTuneCpot;

int octBsw;
int oldOctBsw;
int octCsw;
int oldOctCsw;

int shapeAsw;
int oldShapeAsw;
int shapeBsw;
int oldShapeBsw;
int shapeCpot;
int oldShapeCpot;


int crossModpot;
int oldCrossModpot;

int volApot;
int oldVolApot;
int volBpot;
int oldVolBpot;
int volCpot;
int oldVolCpot;
int volSubpot;
int oldVolSubpot;

int cutpot;
int oldCutpot;
int respot;
int oldRespot;

int filtModesw;
int oldFiltModesw;

int fAttpot;
int oldFAttpot;
int fDecpot;
int oldFDecpot;
int fRelpot;
int oldFRelpot;
int fAmtpot;
int oldFAmtpot;


int attpot;
int oldAttpot;
int decpot;
int oldDecpot;
float suspot;
float oldSuspot; // why float?
int relpot;
int oldRelpot;


int lfoAmppot;
int oldLfoAmppot;
int lfoFreqpot;
int oldLfoFreqpot;
int lfoAttpot;
int oldLfoAttpot;
int lfoDecpot;
int oldLfoDecpot;
int lfoSuspot;
int oldLfoSuspot;

int lfoShapesw;
int oldLfoShapesw;
int lfoDestsw;
int oldLfoDestsw;

int lfoBAmppot;
int oldLfoBAmppot;
int lfoBFreqpot;
int oldLfoBFreqpot;

int dlyAmtpot;
int oldDlyAmtpot;
int dlyTimepot;
int oldDlyTimepot;

int revMixpot;
int oldRevMixpot;
int revSizepot;
int oldRevSizepot;

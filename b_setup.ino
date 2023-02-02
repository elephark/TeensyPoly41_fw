void setup() {
  Serial.begin(9600);
  Serial.println("Serial port is up.");

  // Let's reserve a block of memory for the audio engine.
  // The highest usage I've seen so far is 484, but I'd rather have extra room than not enough.
  // If it's too low, we'll start hearing glitches if the delay is set too long.
  AudioMemory(1024);

  //Midi setup
//  usbMIDI.begin();
//  usbMIDI.setHandleNoteOn(myNoteOn);
//  usbMIDI.setHandleNoteOff(myNoteOff);
  MIDI.setHandleNoteOn(myNoteOn);
  MIDI.setHandleNoteOff(myNoteOff);
  MIDI.setHandlePitchBend(myPitchBend);
  MIDI.begin(MIDI_CHANNEL_OMNI);



  //Mux setup
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);
  pinMode(MUX_C, OUTPUT);

  digitalWrite(MUX_A, 0);
  digitalWrite(MUX_B, 0);
  digitalWrite(MUX_C, 0);

  pinMode(MUX4_X, INPUT_PULLUP);
  pinMode(MUX5_X, INPUT_PULLUP);
  pinMode(MUX6_X, INPUT_PULLUP);
  pinMode(MUX7_X, OUTPUT);
  
  digitalWrite(MUX7_X, 0);

  // Set up debouncing
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    btns[i] = new Bounce();
    btns[i]->attach(MUX6_X, INPUT_PULLUP);
    btns[i]->interval(20);
  }

  pinMode(LED_MEM_ACTIVE, OUTPUT);
  pinMode(LED_MEM_STORE, OUTPUT);

  digitalWrite(LED_MEM_ACTIVE, 0);
  digitalWrite(LED_MEM_STORE, 0);

// lol debug stuff
  analogReadAveraging(32);

  // This seems to be necessary on the Teensy 4.1 to prevent weird read issues.
  pinMode(14, INPUT_DISABLE);
  pinMode(15, INPUT_DISABLE);
  pinMode(16, INPUT_DISABLE);
  pinMode(17, INPUT_DISABLE);


  // Set up MIDI channel select pins. Note that this requires a Teensy 4.1 (ie not a 4.0).
  pinMode(ChSel1, INPUT_PULLUP);
  pinMode(ChSel2, INPUT_PULLUP);
  pinMode(ChSel4, INPUT_PULLUP);
  pinMode(ChSel8, INPUT_PULLUP);


  // Initialize the array we use to allocate voices.
  for (uint8_t i = 0; i < NUM_VOICES; i++) { voiceOrder[i] = i; }

  // Initialize the stack we use to keep track of notes in monophonic mode.
  for (uint8_t i = 0; i < MONO_STACK_SIZE; i++) { monoStack[i] = MONO_OFF; }

  //vco setup

  
  // Hook up the mixers
  for (uint8_t m = 0; m < NUM_MIXERS; m++) {
    finalMixPatchCord[m] = new AudioConnection(mix[m], 0, finalMix, m);
  }

  // The following loop should replace (most of) the stuff below it. lol
  for (uint8_t v = 0; v < NUM_VOICES; v++) {
    // Connect each voice output to its respective mixer4
    voiceOutPatchCord[v] = new AudioConnection(voice[v].voiceOut, 0, mix[v / 4], (v % 4));

    voice[v].vcoA.begin(vcoVol, 150, WAVEFORM_SAWTOOTH);
    voice[v].vcoB.begin(vcoVol, 150, WAVEFORM_SQUARE);
    voice[v].vcoC.begin(vcoVol * 1.5, 150, WAVEFORM_ARBITRARY);
    voice[v].sub.begin(vcoVol * 1.5, 150, WAVEFORM_TRIANGLE);

    voice[v].filter.octaveControl(7);
    voice[v].filterEnv.sustain(0);
    voice[v].lfoA.begin(WAVEFORM_SINE);
    voice[v].lfoB.begin(0.5, 1, WAVEFORM_TRIANGLE);

    // Let's go with phase modulation instead of frequency modulation.
    // At least in this application, set up how it is, it seems to be more musical.
    voice[v].vcoA.phaseModulation(1440);

    // LFO destination disconnect
    voice[v].patchCord2->disconnect(); //vcoA
    voice[v].patchCord3->disconnect(); //vcoB
    voice[v].patchCord4->disconnect(); //vcoC
    voice[v].patchCord5->disconnect(); //sub
    voice[v].patchCord6->disconnect(); //filter
//    voice[v].patchCord125->disconnect(); // amp


  }


  //dly
  dlyFiltL.frequency(4000);
  dlyFiltR.frequency(3000);

  dlyMixL.gain(0, 0.75);
  dlyMixL.gain(0, 0.75);


  dlyL.disable(1);
  dlyL.disable(2);
  dlyL.disable(3);
  dlyL.disable(4);
  dlyL.disable(5);
  dlyL.disable(6);
  dlyL.disable(7);

  dlyR.disable(1);
  dlyR.disable(2);
  dlyR.disable(3);
  dlyR.disable(4);
  dlyR.disable(5);
  dlyR.disable(6);
  dlyR.disable(7);


  //reverb
  reverb.roomsize(0.9);
  reverb.damping(0.8);


  Serial.println("Setup function complete.");
}

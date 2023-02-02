
for (uint8_t v = 0; v < NUM_VOICES; v++)
{
  // filter
  voice[v].filter.frequency(vs.cut);
  voice[v].filter.resonance(vs.res);

  // filter env
  voice[v].filterEnv.attack(vs.filtAtt);
  voice[v].filterEnv.decay(vs.filtDec);
  voice[v].filterEnv.release(vs.filtRel);
  voice[v].dc.amplitude(vs.filtAmt);

  // filter mode
  if (vs.filterMode == 1)
  {
    voice[v].filterMode.gain(0, 1);
    voice[v].filterMode.gain(1, 0);
  }
  else // (filterMode == 0)
  {
    voice[v].filterMode.gain(0, 0);
    voice[v].filterMode.gain(1, 1);
  }

  // LFO A params
  voice[v].lfoA.amplitude(vs.lfoAamp);
  voice[v].lfoA.frequency(vs.lfoAfreq);
  voice[v].lfoAenv.delay(vs.lfoAdel);
  voice[v].lfoAenv.attack(vs.lfoAatt);
  voice[v].lfoAenv.decay(vs.lfoAdec);
  voice[v].lfoAenv.release(vs.lfoArel);
  voice[v].lfoAenv.sustain(vs.lfoAsus);

  // LFO shape switch
  if (vs.lfoAshape == 2)      { voice[v].lfoA.begin(WAVEFORM_SAMPLE_HOLD); }
  else if (vs.lfoAshape == 1) { voice[v].lfoA.begin(WAVEFORM_SAWTOOTH_REVERSE); }
  else                     { voice[v].lfoA.begin(WAVEFORM_SINE); } // (lfoAShape == 0)

  // LFO B params
  voice[v].lfoB.amplitude(vs.lfoBamp);
  voice[v].lfoB.frequency(vs.lfoBfreq);
}

//lfo A read
unsigned long currTime = millis();
if (lfoAread1.available() && currTime - tremTimer >= tremTimerInterval) {
  ampMod = lfoAread1.read();
  tremTimer = currTime;
}
finalMix.gain(0, (1 - (ampMod * 3.2)));
finalMix.gain(1, (1 - (ampMod * 3.2))); // aaaaa yourmom


// LFO A destination
if (vs.lfoAdest == 0) { // lfo - pitch
  for (uint8_t v = 0; v < NUM_VOICES; v++) {
    voice[v].patchCord2->connect(); //vcoA
    voice[v].patchCord3->connect(); //vcoB
    voice[v].patchCord4->connect(); //vcoC
    voice[v].patchCord5->connect(); //sub
    voice[v].patchCord6->disconnect(); //filter
  }
  patchCord12.disconnect(); //lfoAread
  
} else if (vs.lfoAdest == 1) { // lfo - filter
    for (uint8_t v = 0; v < NUM_VOICES; v++) {
    voice[v].patchCord2->disconnect(); //vcoA
    voice[v].patchCord3->disconnect(); //vcoB
    voice[v].patchCord4->disconnect(); //vcoC
    voice[v].patchCord5->disconnect(); //sub
    voice[v].patchCord6->connect(); //filter
  }
  patchCord12.disconnect(); //lfoAread
  
} else if (vs.lfoAdest == 2) { // lfo - amp
    for (uint8_t v = 0; v < NUM_VOICES; v++) {
    voice[v].patchCord2->disconnect(); //vcoA
    voice[v].patchCord3->disconnect(); //vcoB
    voice[v].patchCord4->disconnect(); //vcoC
    voice[v].patchCord5->disconnect(); //sub
    voice[v].patchCord6->disconnect(); //filter
  }
  patchCord12.connect(); //lfoAread
  
}

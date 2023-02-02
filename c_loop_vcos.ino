void loop() {
//  usbMIDI.read();
  MIDI.read();

  if (voices < 0) {
    voices = 0;
  }

  for (uint8_t v = 0; v < NUM_VOICES; v++)
  {
    // cross mod
    voice[v].modMix.gain(0, vs.crossMod);

    // voice frequencies
    // todo: pitchBend should be ignored (ie frozen in place) for voices in release stage
    voice[v].vcoA.frequency(noteFreqs[voice[v].noteFreq] * vs.octave * pitchBend);
    voice[v].vcoB.frequency(noteFreqs[voice[v].noteFreq] * vs.octave * vs.octaveB * vs.tuneB * pitchBend);
    voice[v].vcoC.frequency(noteFreqs[voice[v].noteFreq] * vs.octave * vs.octaveC * vs.tuneC * pitchBend);
    voice[v].sub.frequency(noteFreqs[voice[v].noteFreq] / 2 * vs.octave * pitchBend);

    // VCO mixer
    voice[v].voiceMix.gain(0, vs.vcoAvol * mainVol);
    voice[v].voiceMix.gain(1, vs.vcoBvol * mainVol);
    voice[v].voiceMix.gain(2, vs.vcoCvol * mainVol);
    voice[v].voiceMix.gain(3, vs.Subvol * mainVol);

    // VCO A shape
    if (vs.shapeA == 2)
    {
      voice[v].vcoA.begin(WAVEFORM_TRIANGLE_VARIABLE);
      voice[v].vcoA.amplitude(vcoVol * 1.5);
    }
    else if (vs.shapeA == 1)
    {
      voice[v].vcoA.begin(WAVEFORM_SAWTOOTH);
      voice[v].vcoA.amplitude(vcoVol);
    }
    else // (shapeA == 0)
    {
      voice[v].vcoA.begin(WAVEFORM_PULSE);
    }

    // VCO B shape
    if (vs.shapeB == 2)
    {
      voice[v].vcoB.begin(WAVEFORM_TRIANGLE_VARIABLE);
      voice[v].vcoB.amplitude(vcoVol * 1.5);
    }
    else if (vs.shapeB == 1)
    {
      voice[v].vcoB.begin(WAVEFORM_SAWTOOTH);
      voice[v].vcoB.amplitude(vcoVol);
    }
    else // (shapeA == 0)
    {
      voice[v].vcoB.begin(WAVEFORM_PULSE);
    }

      //Vco C shapes
    switch (vs.shapeC) {
      case 0 ... 32:
        voice[v].vcoC.arbitraryWaveform(wave1, 2000);
        break;
      case 37 ... 69:
        voice[v].vcoC.arbitraryWaveform(wave2, 2000);
        break;
      case 73 ... 105:
        voice[v].vcoC.arbitraryWaveform(wave3, 2000);
        break;
      case 109 ... 141:
        voice[v].vcoC.arbitraryWaveform(wave4, 2000);
        break;
      case 145 ... 177:
        voice[v].vcoC.arbitraryWaveform(wave5, 2000);
        break;
      case 181 ... 212:
        voice[v].vcoC.arbitraryWaveform(wave6, 2000);
        break;
      case 217 ... 248 :
        voice[v].vcoC.arbitraryWaveform(wave7, 2000);
        break;
      case 253 ... 285:
        voice[v].vcoC.arbitraryWaveform(wave8, 2000);
        break;
      case 289 ... 320:
        voice[v].vcoC.arbitraryWaveform(wave9, 2000);
        break;
      case 325 ... 357:
        voice[v].vcoC.arbitraryWaveform(wave10, 2000);
        break;
      case 361 ... 393:
        voice[v].vcoC.arbitraryWaveform(wave11, 2000);
        break;
      case 397 ... 429:
        voice[v].vcoC.arbitraryWaveform(wave12, 2000);
        break;
      case 433 ... 465:
        voice[v].vcoC.arbitraryWaveform(wave13, 2000);
        break;
      case 469 ... 500:
        voice[v].vcoC.arbitraryWaveform(wave14, 2000);
        break;
      case 505 ... 537:
        voice[v].vcoC.arbitraryWaveform(wave15, 2000);
        break;
      case 541 ... 573:
        voice[v].vcoC.arbitraryWaveform(wave16, 2000);
        break;
      case 577 ... 609:
        voice[v].vcoC.arbitraryWaveform(wave17, 2000);
        break;
      case 613 ... 645:
        voice[v].vcoC.arbitraryWaveform(wave18, 2000);
        break;
      case 649 ... 680:
        voice[v].vcoC.arbitraryWaveform(wave19, 2000);
        break;
      case 685 ... 717:
        voice[v].vcoC.arbitraryWaveform(wave20, 2000);
        break;
      case 721 ... 752:
        voice[v].vcoC.arbitraryWaveform(wave21, 2000);
        break;
      case 757 ... 789:
        voice[v].vcoC.arbitraryWaveform(wave22, 2000);
        break;
      case 793 ... 825:
        voice[v].vcoC.arbitraryWaveform(wave23, 2000);
        break;
      case 829 ... 860:
        voice[v].vcoC.arbitraryWaveform(wave24, 2000);
        break;
      case 865 ... 896:
        voice[v].vcoC.arbitraryWaveform(wave25, 2000);
        break;
      case 901 ... 933:
        voice[v].vcoC.arbitraryWaveform(wave26, 2000);
        break;
      case 937 ... 966:
        voice[v].vcoC.arbitraryWaveform(wave27, 2000);
        break;
      case 970 ... 1023:
        voice[v].vcoC.arbitraryWaveform(wave28, 2000);
        break;
      default:
        // do nothing, I guess
        break;
    }
  }

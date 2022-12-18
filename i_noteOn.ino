uint8_t voiceOrder[NUM_VOICES] = {0, 1, 2, 3, 4, 5}; // make sure this is initialized!

bool voiceOrder_Add(uint8_t whichVoice) {
  // If we have room, add the requested voice to the end of the order.
  if (voices >= 0 && voices < NUM_VOICES) {
    voiceOrder[voices] = whichVoice;
    voices++;

    // lol debug
//    Serial.println(str + "voiceOrder_Add(" + whichVoice + ")");
//    Serial.println(str + "voiceOrder[] = {" + voiceOrder[0] + ", " + voiceOrder[1] + ", " + voiceOrder[2] + ", " + voiceOrder[3] + ", " + voiceOrder[4] + ", " + voiceOrder[5] + "}");
//    Serial.println(str + "voices = " + voices);
    
    return true;
  }
  
  // If we're here, we don't have room, or there's some other serious error.
  return false;
}

bool voiceOrder_Remove(uint8_t whichVoice) {
  bool retVal = false;
  // If the voice is in use, remove it from the order and shift the other values over.
  if (voices > 0 && voices <= NUM_VOICES) {
    for (uint8_t i = 0; i < NUM_VOICES; i++) {
      if (voiceOrder[i] == whichVoice) {
        for (uint8_t j = i; j < NUM_VOICES - 1; j++) {
          voiceOrder[j] = voiceOrder[j + 1];
        }
        voiceOrder[NUM_VOICES - 1] = whichVoice;
        voices--;
//        retVal = true;
//        Serial.println(str + "voiceOrder_Remove(" + whichVoice + ")");
//        Serial.println(str + "voiceOrder[] = {" + voiceOrder[0] + ", " + voiceOrder[1] + ", " + voiceOrder[2] + ", " + voiceOrder[3] + ", " + voiceOrder[4] + ", " + voiceOrder[5] + "}");
//        Serial.println(str + "voices = " + voices);
        return true;
      }
    }
  }
  return retVal;
}

uint8_t voiceOrder_Steal(void) {
  // Steal the oldest voice from the order and return it.
  if (voices > 0) {
    uint8_t oldestVoice = voiceOrder[0];
    voiceOrder_Remove(oldestVoice);
    Serial.println(str + "voiceOrder_Steal(" + oldestVoice + ")");
    return oldestVoice;
  }
  return 0;
}

void doNoteOn(uint8_t v, byte note) {

  #ifdef YAY_ARRAYS

  voice[v].noteFreq = note;
  voice[v].env.noteOn();
  voice[v].filterEnv.noteOn();
  voice[v].lfoAenv.noteOn();
  voice[v].envOn = true;
  voiceOrder_Add(v);

  #else // YAY_ARRAYS is not defined
  
  if (voice == 0) {
    note1freq = note;
    env1.noteOn();
    filterEnv1.noteOn();
    lfoAenv1.noteOn();
    env1on = true;
    voiceOrder_Add(0);
  } else if (voice == 1) {
    note2freq = note;
    env2.noteOn();
    filterEnv2.noteOn();
    lfoAenv2.noteOn();
    env2on = true;
    voiceOrder_Add(1);
  } else if (voice == 2) {
    note3freq = note;
    env3.noteOn();
    filterEnv3.noteOn();
    lfoAenv3.noteOn();
    env3on = true;
    voiceOrder_Add(2);
  } else if (voice == 3) {
    note4freq = note;
    env4.noteOn();
    filterEnv4.noteOn();
    lfoAenv4.noteOn();
    env4on = true;
    voiceOrder_Add(3);
  } else if (voice == 4) {
    note5freq = note;
    env5.noteOn();
    filterEnv5.noteOn();
    lfoAenv5.noteOn();
    env5on = true;
    voiceOrder_Add(4);
  } else if (voice == 5) {
    note6freq = note;
    env6.noteOn();
    filterEnv6.noteOn();
    lfoAenv6.noteOn();
    env6on = true;
    voiceOrder_Add(5);
  }

  #endif
}

void myNoteOn(byte channel, byte note, byte velocity) {
  // We only care about one channel.
  if (channel != midiChannel) { return; }

  if (vs.isMonophonic == 0) { //POLYPHONIC mode
//    AudioEffectEnvelope *tmpEnv[NUM_VOICES] = {&env1, &env2, &env3, &env4, &env5, &env6};
//    bool envOn[NUM_VOICES] = {env1on, env2on, env3on, env4on, env5on, env6on};
    switch (voices) {
      case 0 ... 5: {
        bool voiceAllocSuccess = false;
        for (uint8_t i = 0; (i < NUM_VOICES && voiceAllocSuccess == false); i++) {
//          if (!(tmpEnv[voiceOrder[i]]->isActive())) {
//          if (!envOn[voiceOrder[i]]) {
          if (!(voice[voiceOrder[i]].envOn)) {
            doNoteOn(voiceOrder[i], note);
            voiceAllocSuccess = true; // break out
          }
        }
        // If that wasn't enough, steal an off note that still has a running envelope (probably in the release stage)
        if (!voiceAllocSuccess) {
          for (uint8_t i = 0; (i < NUM_VOICES && voiceAllocSuccess == false); i++) {
//            if (!(tmpEnv[voiceOrder[i]]->isActive())) {
            if (!(voice[voiceOrder[i]].env.isActive())) {
//            if (!envOn[voiceOrder[i]]) {
              doNoteOn(voiceOrder[i], note);
              voiceAllocSuccess = true; // break out
            }
          }
        }
        break;
      }
      case 6: {
      Serial.println(str + "case " + voices);
        uint8_t newVoice = voiceOrder_Steal();
        doNoteOn(newVoice, note);
        break;
      }
    }


  } else if (vs.isMonophonic == 1) { //MONOPHONIC mode
//    note1freq = note;
//    env1.noteOn();
//    filterEnv1.noteOn();
//    lfoAenv1.noteOn();
//    voices++;
  }
}


void doNoteOff(uint8_t v) {

  #ifdef YAY_ARRAYS

  voice[v].env.noteOff();
  voice[v].filterEnv.noteOff();
  voice[v].lfoAenv.noteOff();
  voice[v].envOn = false;
  voiceOrder_Remove(v);

  #else // YAY_ARRAYS is not defined
  
  if (voice == 0) {
    env1.noteOff();
    filterEnv1.noteOff();
    lfoAenv1.noteOff();
    env1on = false;
    voiceOrder_Remove(0);
  }  if (voice == 1) {
    env2.noteOff();
    filterEnv2.noteOff();
    lfoAenv2.noteOff();
    env2on = false;
    voiceOrder_Remove(1);
  }  if (voice == 2) {
    env3.noteOff();
    filterEnv3.noteOff();
    lfoAenv3.noteOff();
    env3on = false;
    voiceOrder_Remove(2);
  }  if (voice == 3) {
    env4.noteOff();
    filterEnv4.noteOff();
    lfoAenv4.noteOff();
    env4on = false;
    voiceOrder_Remove(3);
  }  if (voice == 4) {
    env5.noteOff();
    filterEnv5.noteOff();
    lfoAenv5.noteOff();
    env5on = false;
    voiceOrder_Remove(4);
  }  if (voice == 5) {
    env6.noteOff();
    filterEnv6.noteOff();
    lfoAenv6.noteOff();
    env6on = false;
    voiceOrder_Remove(5);
  }

  #endif
}


void myNoteOff(byte channel, byte note, byte velocity) {
  // We only care about one channel.
  if (channel != midiChannel) { return; }
  
  if (vs.isMonophonic == 0) { //POLYPHONIC mode
    switch (voices) {
      case 1 ... 6:
//        int freqs[NUM_VOICES] = {note1freq, note2freq, note3freq, note4freq, note5freq, note6freq}; 
        for (uint8_t i = 0; i < voices; i++) { // look through voices in the order they're used
//          if (freqs[voiceOrder[i]] == note) {
          if (voice[voiceOrder[i]].noteFreq == note) {
            doNoteOff(voiceOrder[i]);
          }
        }
        break;

    }



  } else if (vs.isMonophonic == 1) { //MONOPHONIC mode
//    if (note1freq == note) {
//      env1.noteOff();
//      filterEnv1.noteOff();
//      lfoAenv1.noteOff();
//      voices--;
//    }
  }
}

// Handler for when someone presses a numbered memory button.
void memButtonPress(uint8_t whichMemButton) {
  if (memStore) {
    // Store a preset.
    patchWrite(whichMemButton);
    memStore = 0;
  } else {
    // Load a preset.
    patchRead(whichMemButton);
    // If we weren't in memory mode, we are now.
    memActive = 1;
  }
  // Let anyone else know who cares to know.
  memJustLoaded = true;
//  lastMemLoaded = whichMemButton;
}

// First, read what's on the mux.
// Then, set the mux to the next channel.
// Then, output the appropriate LED state.
void muxRead() {

  switch (curMux) {
  case 0:
    // X0
//    digitalWrite(MUX_A, 0); //A
//    digitalWrite(MUX_B, 0); //B
//    digitalWrite(MUX_C, 0); //C
//    
//    digitalWrite(MUX7_X, (led[0] ? 1 : 0));
//    delay(wait);
    
    pot_mainVol = analogRead(MUX0_X);
    pot_oscCShape = analogRead(MUX1_X);
    pot_filtR = analogRead(MUX2_X);
    pot_lfoD = analogRead(MUX3_X);
    sw_mainOctLo = digitalRead(MUX4_X);
    sw_oscCOctLo = digitalRead(MUX5_X);
//    btn_memActive = digitalRead(MUX6_X) ? 0 : 1;
    btns[0]->update();
    if (btns[0]->fell()) {
      if (memActive) { // memory mode was on, turn it off
        led[0] = 0;
        memActive = 0;
      } else { // memory mode was off, turn it on
        led[0] = 1;
        memActive = 1;
        patchRead(lastMemLoaded); // 
        memJustLoaded = true;
      }
//      led[0] ^= 1;
//      memActive ^= 1;
    }
    
    
    // X1
    digitalWrite(MUX_A, 1);
    digitalWrite(MUX_B, 0);
    digitalWrite(MUX_C, 0);
    
    digitalWrite(MUX7_X, (led[1] ? 1 : 0));
//    delay(wait);
    break;

  case 1:
    pot_portTime = analogRead(MUX0_X);
    pot_oscCTune = analogRead(MUX1_X);
    pot_envA = analogRead(MUX2_X);
    pot_lfoS = analogRead(MUX3_X);
    sw_oscAShapeHi = digitalRead(MUX4_X);
    sw_oscBShapeHi = digitalRead(MUX5_X);
//    btn_memStore = digitalRead(MUX6_X) ? 0 : 1;
    btns[1]->update();
    if (btns[1]->fell()) {
      led[1] ^= 1;
      memStore ^= 1;
    }
    
    // X2
    digitalWrite(MUX_A, 0);
    digitalWrite(MUX_B, 1);
    digitalWrite(MUX_C, 0);
    
    digitalWrite(MUX7_X, (led[2] ? 1 : 0));
//    delay(wait);
    break;

  case 2:
    pot_oscAVol = analogRead(MUX0_X);
    pot_filtCutoff = analogRead(MUX1_X);
    pot_envD = analogRead(MUX2_X);
    pot_pwmFreq = analogRead(MUX3_X);
    sw_oscAShapeLo = digitalRead(MUX4_X);
    sw_oscBShapeLo = digitalRead(MUX5_X);
//    btn_mem[5] = digitalRead(MUX6_X) ? 0 : 1;
    btns[2]->update();
    if (btns[2]->fell()) {
//      led[7] ^= 1;

      dumpVoiceSettings();
    }
    
    
    // X3
    digitalWrite(MUX_A, 1);
    digitalWrite(MUX_B, 1);
    digitalWrite(MUX_C, 0);
    
    digitalWrite(MUX7_X, (led[3] ? 1 : 0));
//    delay(wait);
    break;

  case 3:
    pot_crossMod = analogRead(MUX0_X);
    pot_filtRes = analogRead(MUX1_X);
    pot_envS = analogRead(MUX2_X);
    pot_pwmDepth = analogRead(MUX3_X);
    sw_mainOctHi = digitalRead(MUX4_X);
    sw_oscCOctHi = digitalRead(MUX5_X);
//    btn_mem[4] = digitalRead(MUX6_X) ? 0 : 1;
    btns[3]->update();
    if (btns[3]->fell()) { 
//      led[6] ^= 1; 
      memButtonPress(4);
    }
    
    
    // X4
    digitalWrite(MUX_A, 0);
    digitalWrite(MUX_B, 0);
    digitalWrite(MUX_C, 1);
    
    digitalWrite(MUX7_X, (led[4] ? 1 : 0));
//    delay(wait);
    break;

  case 4:
    pot_subVol = analogRead(MUX0_X);
    pot_filtEnvAmt = analogRead(MUX1_X);
    pot_envR = analogRead(MUX2_X);
    pot_revAmt = analogRead(MUX3_X);
    sw_oscBOctLo = digitalRead(MUX4_X);
    sw_lfoShapeLo = digitalRead(MUX5_X);
//    btn_mem[3] = digitalRead(MUX6_X) ? 0 : 1;
    btns[4]->update();
    if (btns[4]->fell()) { 
//      led[5] ^= 1; 
      memButtonPress(3);
    }
    
    
    // X5
    digitalWrite(MUX_A, 1);
    digitalWrite(MUX_B, 0);
    digitalWrite(MUX_C, 1);
    
    digitalWrite(MUX7_X, (led[5] ? 1 : 0));
//    delay(wait);
    break;

  case 5:
    pot_oscBVol = analogRead(MUX0_X);
    pot_filtA = analogRead(MUX1_X);
    pot_lfoFreq = analogRead(MUX2_X);
    pot_revSize = analogRead(MUX3_X);
    sw_isMonophonic = digitalRead(MUX4_X);
    sw_lfoDestHi = digitalRead(MUX5_X);
//    btn_mem[2] = digitalRead(MUX6_X) ? 0 : 1;
    btns[5]->update();
    if (btns[5]->fell()) { 
//      led[4] ^= 1; 
      memButtonPress(2);
    }
    
    
    vs.isMonophonic = sw_isMonophonic;
    
    
    // X6
    digitalWrite(MUX_A, 0);
    digitalWrite(MUX_B, 1);
    digitalWrite(MUX_C, 1);
    
    digitalWrite(MUX7_X, (led[6] ? 1 : 0));
//    delay(wait);
    break;

  case 6:
    pot_oscBTune = analogRead(MUX0_X);
    pot_filtD = analogRead(MUX1_X);
    pot_lfoDepth = analogRead(MUX2_X);
    pot_dlyAmt = analogRead(MUX3_X);
    sw_oscBOctHi = digitalRead(MUX4_X);
    sw_lfoShapeHi = digitalRead(MUX5_X);
//    btn_mem[1] = digitalRead(MUX6_X) ? 0 : 1;
    btns[6]->update();
    if (btns[6]->fell()) {
      memButtonPress(1);
////      led[3] ^= 1;
//      if (memStore) {
////        Serial.println(str + "Calling patchWrite(1)");
//        patchWrite(1);
//      } else {
////        Serial.println(str + "Calling patchRead(1)");
//        patchRead(1);
//      }
//      memStore = 0;
//      memActive = 1;
//      memJustLoaded = true;
    }    
    
    
    // X7
    digitalWrite(MUX_A, 1);
    digitalWrite(MUX_B, 1);
    digitalWrite(MUX_C, 1);
    
    digitalWrite(MUX7_X, (led[7] ? 1 : 0));
//    delay(wait);
    break;

  case 7:
    pot_oscCVol = analogRead(MUX0_X);
    pot_filtS = analogRead(MUX1_X);
    pot_lfoA = analogRead(MUX2_X);
    pot_dlyRate = analogRead(MUX3_X);
    sw_filtMode = digitalRead(MUX4_X);
    sw_lfoDestLo = digitalRead(MUX5_X);
//    btn_mem[0] = digitalRead(MUX6_X) ? 0 : 1;
    btns[7]->update();
    if (btns[7]->fell()) {
      memButtonPress(0);
//      led[2] ^= 1;
//      if (memStore) {
//        Serial.println(str + "Calling patchWrite(0)");
//        patchWrite(0);
//      } else {
//        Serial.println(str + "Calling patchRead(0)");
//        patchRead(0);
//      }
//      memStore = 0;
//      memActive = 1;
//      memJustLoaded = true;
    }
    

    digitalWrite(MUX_A, 0); //A
    digitalWrite(MUX_B, 0); //B
    digitalWrite(MUX_C, 0); //C

    digitalWrite(MUX7_X, (led[0] ? 1 : 0));
    break;

  default:
    curMux = 7;
    break;
  }

  curMux++;
  if (curMux > 7) { curMux = 0; }
}

uint8_t bendRange = 2; // in semitones


// Handler for when we receive a MIDI pitch bend message.
void myPitchBend(byte channel, int bend) {
  // We only care about one channel.
  if (channel != midiChannel) { return; }
//  Serial.println(str + "myPitchBend(" + channel + ", " + bend + ")");
  pitchBend = pow(2, ((((float)bend / 8192.0) * bendRange) / 12));
}

if (millis() - prevTimer > timer) {

  // Main volume
  mainVol = (float)pot_mainVol / 1024;


  //main octave
  if (sw_mainOctLo == 0) {
    vs.octave = 0.5;
  } else if (sw_mainOctLo == 1 && sw_mainOctHi == 1) {
    vs.octave = 1;
  } else if (sw_mainOctHi == 0) {
    vs.octave = 2;
  }


  // Read the MIDI channel select pins.
  // Do we need to do this quite so often? Probably not.
  // Do I care? Also probably not.
  midiChannel = 1; // They come in 1-indexed.
  midiChannel += (digitalRead(ChSel1) ? 0 : 1);
  midiChannel += (digitalRead(ChSel2) ? 0 : 2);
  midiChannel += (digitalRead(ChSel4) ? 0 : 4);
  midiChannel += (digitalRead(ChSel8) ? 0 : 8);
  



  ///////////////  EDIT MODE EDIT ///////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (!memActive) {

    if (sw_oscBOctLo == 0) {
      vs.octaveB = 0.5;
    } else if (sw_oscBOctLo == 1 && sw_oscBOctHi == 1) {
      vs.octaveB = 1;
    } else if (sw_oscBOctHi == 0) {
      vs.octaveB = 2;
    }

    if (sw_oscCOctLo == 0) {
      vs.octaveC = 0.5;
    } else if (sw_oscCOctLo == 1 && sw_oscCOctHi == 1) {
      vs.octaveC = 1;
    } else if (sw_oscCOctHi == 0) {
      vs.octaveC = 2;
    }


    if (sw_oscAShapeLo == 0) {
      vs.shapeA = 2;
    } else if (sw_oscAShapeLo == 1 && sw_oscAShapeHi == 1) {
      vs.shapeA = 1;
    } else if (sw_oscAShapeHi == 0) {
      vs.shapeA = 0;
    }

    if (sw_oscBShapeLo == 0) {
      vs.shapeB = 2;
    } else if (sw_oscBShapeLo == 1 && sw_oscBShapeHi == 1) {
      vs.shapeB = 1;
    } else if (sw_oscBShapeHi == 0) {
      vs.shapeB = 0;
    }

    vs.shapeC = pot_oscCShape;

    if (pot_oscBTune < 512) {
      vs.tuneB = ((float)pot_oscBTune / 1023) + 0.5;
    } else {
      vs.tuneB = ((float)pot_oscBTune / 510);
    }

    if (pot_oscCTune < 512) {
      vs.tuneC = ((float)pot_oscCTune / 1023) + 0.5;
    } else {
      vs.tuneC = ((float)pot_oscCTune / 510);
    }

    vs.crossMod = (float)pot_crossMod / 512;

    vs.vcoAvol = (float)pot_oscAVol / 1023;
    vs.vcoBvol = (float)pot_oscBVol / 1023;
    vs.vcoCvol = (float)pot_oscCVol / 1023;
    vs.Subvol = (float)pot_subVol / 1023;

//    vs.cut = 15000 * (float)pot_filtCutoff / 1023 + 15; // lol make this exponential or something eh
    vs.cut = 15000 * (pow((float)pot_filtCutoff / 1023, 3)) + 15;
    vs.res = 4.5 * (float)pot_filtRes / 1023 + 1.1;
    vs.filtAtt = (3000 * (float)pot_filtA / 1023);
    vs.filtDec = (3000 * (float)pot_filtD / 1023);
    vs.filtRel = (5000 * (float)pot_filtR / 1023);
    vs.filtAmt = (float)pot_filtEnvAmt / 512 - 1;
    if (sw_filtMode == 0) {
      vs.filterMode = 1;
    } else if (sw_filtMode == 1) {
      vs.filterMode = 0;
    }

    vs.envAtt = 3000 * (float)pot_envA / 1023;
    vs.envDec = 5000 * (float)pot_envD / 1023;
    vs.envRel = 5000 * (float)pot_envR / 1023;
    vs.envSus = (float)pot_envS / 1023;

    if (vs.lfoAdest == 0 && vs.lfoAshape != 2) {
      vs.lfoAamp = (float)pot_lfoDepth / 1024 / 10;
    } else {
      vs.lfoAamp = (float)pot_lfoDepth / 1024 / 3;
    }
    vs.lfoAfreq = 20 * (float)pot_lfoFreq / 1024 + 0.1;
    vs.lfoAdel = 2000 * (float)pot_lfoA / 1024;
    vs.lfoAatt = 3000 * (float)pot_lfoA / 1024;
    vs.lfoAdec = 4000 * (float)pot_lfoD / 1024;
    vs.lfoArel = 4000 * (float)pot_lfoD / 1024;
    vs.lfoAsus = (float)pot_lfoS / 1024;

    if (sw_lfoDestLo == 0) { //lfo - pitch
      vs.lfoAdest = 2;
    } else if ( sw_lfoDestLo == 1 && sw_lfoDestHi == 1) { //lfo - filter
      vs.lfoAdest = 1;
    } else if (sw_lfoDestHi == 0) { //lfo - amp
      vs.lfoAdest = 0;
    }

    if (sw_lfoShapeLo == 0) {
      vs.lfoAshape = 0;
    } else if ( sw_lfoShapeLo == 1 && sw_lfoShapeHi == 1) {
      vs.lfoAshape = 1;
    } else if (sw_lfoShapeHi == 0) {
      vs.lfoAshape = 2;
    }

    vs.lfoBamp = (float)pot_pwmDepth / 1023;
    vs.lfoBfreq = 5 * (float)pot_pwmFreq / 1023 + 0.1;

    vs.dlyAmt = (float)pot_dlyAmt / 1100 - 0.1;
    vs.dlyTimeL = pot_dlyRate / 2.5;
    vs.dlyTimeR = pot_dlyRate / 1.25;
    vs.revMix = ((float)pot_revAmt / 1024 / 1.5);
    vs.revSize = ((float)pot_revSize / 1024 - 0.01);

    if (vs.dlyAmt < 0) {
      vs.dlyAmt = 0;
    }


  } else { // memory mode is active


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // PRESET MODE ///////// PRESET MODE //////// PRESET MODE //////////// PRESET MODE /////// PRESET MODE ////
    // PRESET MODE ///////// PRESET MODE //////// PRESET MODE //////////// PRESET MODE /////// PRESET MODE ////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////


    
    ///////////////  OCTAVES OCTAVES //////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////


    //octave vco B
    if (sw_oscBOctLo == 0) {
      octBsw = 0;
    } else if (sw_oscBOctLo == 1 && sw_oscBOctHi == 1) {
      octBsw = 1;
    } else if (sw_oscBOctHi == 0) {
      octBsw = 2;
    }
    
    if (memJustLoaded) { oldOctBsw = octBsw; }
    
    if (oldOctBsw < octBsw || oldOctBsw > octBsw) {
      if (sw_oscBOctLo == 0) {
        vs.octaveB = 0.5;
      } else if (sw_oscBOctLo == 1 && sw_oscBOctHi == 1) {
        vs.octaveB = 1;
      } else if (sw_oscBOctHi == 0) {
        vs.octaveB = 2;
      }
      oldOctBsw = octBsw;
      Serial.println("octave B switch");
    }

    //octave vco C
    if (sw_oscCOctLo == 0) {
      octCsw = 0;
    } else if (sw_oscCOctLo == 1 && sw_oscCOctHi == 1) {
      octCsw = 1;
    } else if (sw_oscCOctHi == 0) {
      octCsw = 2;
    }
    
    if (memJustLoaded) { oldOctCsw = octCsw; }
    
    if (oldOctCsw < octCsw || oldOctCsw > octCsw) {
      if (sw_oscCOctLo == 0) {
        vs.octaveC = 0.5;
      } else if (sw_oscCOctLo == 1 && sw_oscCOctHi == 1) {
        vs.octaveC = 1;
      } else if (sw_oscCOctHi == 0) {
        vs.octaveC = 2;
      }
      oldOctCsw = octCsw;
      Serial.println("octave C switch");
    }



    //////////// SHAPES SHAPES ////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Shape A
    if (sw_oscAShapeLo == 0) {
      shapeAsw = 0;
    } else if (sw_oscAShapeLo == 1 && sw_oscAShapeHi == 1) {
      shapeAsw = 1;
    } else if (sw_oscAShapeHi == 0) {
      shapeAsw = 2;
    }

    if (memJustLoaded) { oldShapeAsw = shapeAsw; }
    
    if (oldShapeAsw < shapeAsw || oldShapeAsw > shapeAsw) {
      if (sw_oscAShapeLo == 0) {
        vs.shapeA = 0;
      } else if (sw_oscAShapeLo == 1 && sw_oscAShapeHi == 1) {
        vs.shapeA = 1;
      } else if (sw_oscAShapeHi == 0) {
        vs.shapeA = 2;
      }
      oldShapeAsw = shapeAsw;
      Serial.println("shape A switch");
    }

    //Shape B
    if (sw_oscBShapeLo == 0) {
      shapeBsw = 0;
    } else if (sw_oscBShapeLo == 1 && sw_oscBShapeLo == 1) {
      shapeBsw = 1;
    } else if (sw_oscBShapeLo == 0) {
      shapeBsw = 2;
    }

    if (memJustLoaded) { oldShapeBsw = shapeBsw; }

    if (oldShapeBsw < shapeBsw || oldShapeBsw > shapeBsw) {
      if (sw_oscBShapeLo == 0) {
        vs.shapeB = 0;
      } else if (sw_oscBShapeLo == 1 && sw_oscBShapeLo == 1) {
        vs.shapeB = 1;
      } else if (sw_oscBShapeLo == 0) {
        vs.shapeB = 2;
      }
      oldShapeBsw = shapeBsw;
      Serial.println("shape B switch");
    }

    //Vco C shape
    shapeCpot = pot_oscCShape;
    
    if (memJustLoaded) { oldShapeCpot = shapeCpot; }

    if (oldShapeCpot + thresh2 < shapeCpot || oldShapeCpot - thresh2 > shapeCpot) {
      vs.shapeC = pot_oscCShape;
      oldShapeCpot = shapeCpot; // + thresh2 / 2;
      Serial.println("shape C turn");
    }



    /////////// TUNINGS TUNINGS //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    //tuneB
    tuneBpot = pot_oscBTune;
        
    if (memJustLoaded) { oldTuneBpot = tuneBpot; }

    if (oldTuneBpot + thresh < tuneBpot || oldTuneBpot - thresh > tuneBpot) {
      if (pot_oscBTune < 512) {
        vs.tuneB = ((float)pot_oscBTune / 1023) + 0.5;
      } else {
        vs.tuneB = ((float)pot_oscBTune / 510);
      }
      oldTuneBpot = tuneBpot; // + thresh / 2;
      Serial.println("tuneB turn");
    }

    //tuneC
    tuneCpot = pot_oscCTune;
        
    if (memJustLoaded) { oldTuneCpot = tuneCpot; }

    if (oldTuneCpot + thresh < tuneCpot || oldTuneCpot - thresh > tuneCpot) {
      if (pot_oscCTune < 512) {
        vs.tuneC = ((float)pot_oscCTune / 1023) + 0.5;
      } else {
        vs.tuneC = ((float)pot_oscCTune / 510);
      }
      oldTuneCpot = tuneCpot; // + thresh / 2;
      Serial.println("tuneC turn");
    }

    //Cross mod
    crossModpot = pot_crossMod;
        
    if (memJustLoaded) { oldCrossModpot = crossModpot; }

    if (oldCrossModpot + thresh < crossModpot || oldCrossModpot - thresh > crossModpot) {
      vs.crossMod = (float)pot_crossMod / 512;
      oldCrossModpot = crossModpot; // + thresh / 2;
      Serial.println("crossmod turn");
    }



    ///////////// VOLUMES VOLUMES /////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    volApot = pot_oscAVol;
        
    if (memJustLoaded) { oldVolApot = volApot; }

    if (oldVolApot + thresh2 < volApot || oldVolApot - thresh2 > volApot) {
      vs.vcoAvol = (float)pot_oscAVol / 1023;
      oldVolApot = volApot; // + thresh2 / 2;
      Serial.println("volA turn");
    }

    volBpot = pot_oscBVol;
        
    if (memJustLoaded) { oldVolBpot = volBpot; }

    if (oldVolBpot + thresh2 < volBpot || oldVolBpot - thresh2 > volBpot) {
      vs.vcoBvol = (float)pot_oscBVol / 1023;
      oldVolBpot = volBpot; // + thresh2 / 2;
      Serial.println("volB turn");
    }

    volCpot = pot_oscCVol;
        
    if (memJustLoaded) { oldVolCpot = volCpot; }

    if (oldVolCpot + thresh2 < volCpot || oldVolCpot - thresh2 > volCpot) {
      vs.vcoCvol = (float)pot_oscCVol / 1023;
      oldVolCpot = volCpot; // + thresh2 / 2;
      Serial.println("volC turn");
    }

    volSubpot = pot_subVol;
        
    if (memJustLoaded) { oldVolSubpot = volSubpot; }

    if (oldVolSubpot + thresh2 < volSubpot || oldVolSubpot - thresh2 > volSubpot) {
      vs.Subvol = (float)pot_subVol / 1023;
      oldVolSubpot = volSubpot; // + thresh2 / 2;
      Serial.println("vol sub turn");
    }



    //////////// FILTER FILTER ////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Filter
    cutpot = pot_filtCutoff;
        
    if (memJustLoaded) { oldCutpot = cutpot; }

    if (oldCutpot + thresh < cutpot || oldCutpot - thresh > cutpot) {
//      vs.cut = 15000 * (float)pot_filtCutoff / 1023 + 15;             /////cut
      vs.cut = 15000 * (pow((float)pot_filtCutoff / 1023, 3)) + 15;
      oldCutpot = cutpot; // + thresh / 2;
      Serial.println("cut turn");
    }

    respot = pot_filtRes;
        
    if (memJustLoaded) { oldRespot = respot; }

    if (oldRespot + thresh2 < respot || oldRespot - thresh2 > respot) {
      vs.res = 4.5 * (float)pot_filtRes / 1023 + 1.1;
      oldRespot = respot; // + thresh2 / 2;
      Serial.println("res turn");
    }

    //Filter Env

    fAttpot = pot_filtA;
        
    if (memJustLoaded) { oldFAttpot = fAttpot; }

    if (oldFAttpot + thresh2 < fAttpot || oldFAttpot - thresh2 > fAttpot) {
      vs.filtAtt = (3000 * (float)pot_filtA / 1023);
      oldFAttpot = fAttpot; // + thresh2 / 2;
      Serial.println("filter attack turn");
    }

    fDecpot = pot_filtD;
        
    if (memJustLoaded) { oldFDecpot = fDecpot; }

    if (oldFDecpot + thresh2 < fDecpot || oldFDecpot - thresh2 > fDecpot) {
      vs.filtDec = (3000 * (float)pot_filtD / 1023);
      oldFDecpot = fDecpot; // + thresh2 / 2;
      Serial.println("filter decay turn");
    }

    fRelpot = pot_filtR;
        
    if (memJustLoaded) { oldFRelpot = fRelpot; }

    if (oldFRelpot + thresh2 < fRelpot || oldFRelpot - thresh2 > fRelpot) {
      vs.filtRel = (5000 * (float)pot_filtR / 1023);
      oldFRelpot = fRelpot; // + thresh2 / 2;
      Serial.println("filter release turn");
    }

    fAmtpot = pot_filtEnvAmt;
        
    if (memJustLoaded) { oldFAmtpot = fAmtpot; }

    if (oldFAmtpot + thresh2 < fAmtpot || oldFAmtpot - thresh2 > fAmtpot) {
      vs.filtAmt = (float)pot_filtEnvAmt / 512 - 1;
      oldFAmtpot = fAmtpot; // + thresh2 / 2;
      Serial.println("filter Amt turn");
    }

    //FilterMode
    if (sw_filtMode == 0) {
      filtModesw = 1;
    } else if (sw_filtMode == 1) {
      filtModesw = 0;
    }
        
    if (memJustLoaded) { oldFiltModesw = filtModesw; }

    if (oldFiltModesw < filtModesw || oldFiltModesw > filtModesw) {
      if (sw_filtMode == 0) {
        vs.filterMode = 1;
      } else if (sw_filtMode == 1) {
        vs.filterMode = 0;
      }
      oldFiltModesw = filtModesw;
      Serial.println("filter mode switch");
    }



    /////////////// MAIN ENVELOPE ENVELOPE ////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    attpot = pot_envA;
        
    if (memJustLoaded) { oldAttpot = attpot; }

    if (oldAttpot + thresh2 < attpot || oldAttpot - thresh2 > attpot) {
      vs.envAtt = 3000 * (float)pot_envA / 1023;
      oldAttpot = attpot; // + thresh2 / 2;
      Serial.println("Attack turn");
    }

    decpot = pot_envD;
        
    if (memJustLoaded) { oldDecpot = decpot; }

    if (oldDecpot + thresh2 < decpot || oldDecpot - thresh2 > decpot) {
      vs.envDec = 5000 * (float)pot_envD / 1023;
      oldDecpot = decpot; // + thresh2 / 2;
      Serial.println("Decay turn");
    }

    suspot = pot_envS;
        
    if (memJustLoaded) { oldSuspot = suspot; }

    if (oldSuspot + thresh2 < suspot || oldSuspot - thresh2 > suspot) {
      vs.envSus = (float)pot_envS / 1024;
      oldSuspot = suspot; // + thresh2 / 2;
      Serial.println("Sustain turn");
    }

    relpot = pot_envR;
        
    if (memJustLoaded) { oldRelpot = relpot; }

    if (oldRelpot + thresh2 < relpot || oldRelpot - thresh2 > relpot) {
      vs.envRel = 5000 * (float)pot_envR / 1023;
      oldRelpot = relpot; // + thresh2 / 2;
      Serial.println("Release turn");
    }



    ////////////// LFO A   LFO A  /////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    lfoAmppot = pot_lfoDepth;
        
    if (memJustLoaded) { oldLfoAmppot = lfoAmppot; }

    if (oldLfoAmppot + thresh2 < lfoAmppot || oldLfoAmppot - thresh2 > lfoAmppot) {
      if (vs.lfoAdest == 0 && vs.lfoAshape != 2) {
        vs.lfoAamp = (float)pot_lfoDepth / 1024 / 10;
      } else {
        vs.lfoAamp = (float)pot_lfoDepth / 1024 / 3;
      }
      oldLfoAmppot = lfoAmppot; // + thresh2 / 2;
      Serial.println("Lfo A amp turn");
    }

    lfoFreqpot = pot_lfoFreq;
        
    if (memJustLoaded) { oldLfoFreqpot = lfoFreqpot; }

    if (oldLfoFreqpot + thresh2 < lfoFreqpot || oldLfoFreqpot - thresh2 > lfoFreqpot) {
      vs.lfoAfreq = 20 * (float)pot_lfoFreq / 1024 + 0.1;
      oldLfoFreqpot = lfoFreqpot; // + thresh2 / 2;
      Serial.println("Lfo A Freq turn");
    }

    lfoAttpot = pot_lfoA;
        
    if (memJustLoaded) { oldLfoAttpot = lfoAttpot; }

    if (oldLfoAttpot + thresh2 < lfoAttpot || oldLfoAttpot - thresh2 > lfoAttpot) {
      vs.lfoAdel = 2000 * (float)pot_lfoA / 1024;
      vs.lfoAatt = 3000 * (float)pot_lfoA / 1024;
      oldLfoAttpot = lfoAttpot; // + thresh2 / 2;
      Serial.println("Lfo A Att turn");
    }

    lfoDecpot = pot_lfoD;
        
    if (memJustLoaded) { oldLfoDecpot = lfoDecpot; }

    if (oldLfoDecpot + thresh2 < lfoDecpot || oldLfoDecpot - thresh2 > lfoDecpot) {
      vs.lfoAdec = 4000 * (float)pot_lfoD / 1024;
      vs.lfoArel = 4000 * (float)pot_lfoD / 1024;
      oldLfoDecpot = lfoDecpot; // + (thresh2 / 2);
      Serial.println("Lfo A Dec turn");
    }

    lfoSuspot = pot_lfoS;
        
    if (memJustLoaded) { oldLfoSuspot = lfoSuspot; }

    if (oldLfoSuspot + thresh2 * 2 < lfoSuspot || oldLfoSuspot - thresh2 * 2 > lfoSuspot) {
      vs.lfoAsus = (float)pot_lfoS / 1024;
      oldLfoSuspot = lfoSuspot; // + ((thresh2 * 2) / 2);
      Serial.println("Lfo A Sus turn");
    }


    //lfoA destination
    if (sw_lfoDestLo == 0) { //lfo - pitch
      lfoDestsw = 0;
    } else if ( sw_lfoDestLo == 1 && sw_lfoDestHi == 1) { //lfo - filter
      lfoDestsw = 1;
    } else if (sw_lfoDestHi == 0) { //lfo - amp
      lfoDestsw = 2;
    }
        
    if (memJustLoaded) { oldLfoDestsw = lfoDestsw; }

    if (oldLfoDestsw < lfoDestsw || oldLfoDestsw > lfoDestsw) {
      if (sw_lfoDestLo == 0) { //lfo - pitch
        vs.lfoAdest = 0;
      } else if ( sw_lfoDestLo == 1 && sw_lfoDestHi == 1) { //lfo - filter
        vs.lfoAdest = 1;
      } else if (sw_lfoDestHi == 0) { //lfo - amp
        vs.lfoAdest = 2;
      }
      oldLfoDestsw = lfoDestsw;
      Serial.println("Lfo dest switch");
    }


    //lfoA shape
    if (sw_lfoShapeLo == 0) {
      lfoShapesw = 0;
    } else if ( sw_lfoShapeLo == 1 && sw_lfoShapeHi == 1) {
      lfoShapesw = 1;
    } else if (sw_lfoShapeHi == 0) {
      lfoShapesw = 2;
    }
        
    if (memJustLoaded) { oldLfoShapesw = lfoShapesw; }

    if (oldLfoShapesw < lfoShapesw || oldLfoShapesw > lfoShapesw) {
      if (sw_lfoShapeLo == 0) {
        vs.lfoAshape = 0;
      } else if ( sw_lfoShapeLo == 1 && sw_lfoShapeHi == 1) {
        vs.lfoAshape = 1;
      } else if (sw_lfoShapeHi == 0) {
        vs.lfoAshape = 2;
      }
      oldLfoShapesw = lfoShapesw;
      Serial.println("Lfo shape switch");
    }



    ///////////// LFO B    LFO B //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    lfoBAmppot = pot_pwmDepth;
        
    if (memJustLoaded) { oldLfoBAmppot = lfoBAmppot; }

    if (oldLfoBAmppot + thresh2 < lfoBAmppot || oldLfoBAmppot - thresh2 > lfoBAmppot) {
      vs.lfoBamp = (float)pot_pwmDepth / 1023;
      oldLfoBAmppot = lfoBAmppot; // + thresh2 / 2;
      Serial.println("Lfo B amp turn");
    }

    lfoBFreqpot = pot_pwmFreq;
        
    if (memJustLoaded) { oldLfoBFreqpot = lfoBFreqpot; }

    if (oldLfoBFreqpot + thresh2 < lfoBFreqpot || oldLfoBFreqpot - thresh2 > lfoBFreqpot) {
      vs.lfoBfreq = 5 * (float)pot_pwmFreq / 1023 + 0.1;
      oldLfoBFreqpot = lfoBFreqpot; // + thresh2 / 2;
      Serial.println("Lfo B freq turn");
    }



    //////////////// FX FX FX FX //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Delay
    dlyAmtpot = pot_dlyAmt;
        
    if (memJustLoaded) { oldDlyAmtpot = dlyAmtpot; }

    if (oldDlyAmtpot + thresh2 < dlyAmtpot || oldDlyAmtpot - thresh2 > dlyAmtpot) {
      vs.dlyAmt = (float)pot_dlyAmt / 1100 - 0.1;
      if (vs.dlyAmt < 0) {
        vs.dlyAmt = 0;
      }
      oldDlyAmtpot = dlyAmtpot; // + thresh2 / 2;
      Serial.println("Dly amt turn");
    }


    dlyTimepot = pot_dlyRate;
        
    if (memJustLoaded) { oldDlyTimepot = dlyTimepot; }

    if (oldDlyTimepot + thresh2 < dlyTimepot || oldDlyTimepot - thresh2 > dlyTimepot) {
      vs.dlyTimeL = pot_dlyRate / 2.5;
      vs.dlyTimeR = pot_dlyRate / 1.25;
      oldDlyTimepot = dlyTimepot; // + thresh2 / 2;
      Serial.println("Dly time turn");
    }


    //Reverb
    revMixpot = pot_revAmt;
        
    if (memJustLoaded) { oldRevMixpot = revMixpot; }

    if (oldRevMixpot + thresh2 < revMixpot || oldRevMixpot - thresh2 > revMixpot) {
      vs.revMix = ((float)pot_revAmt / 1024 / 1.2);
      oldRevMixpot = revMixpot; // + thresh2 / 2;
      Serial.println("Rev mix turn");
    }


    revSizepot = pot_revSize;
        
    if (memJustLoaded) { oldRevSizepot = revSizepot; }

    if (oldRevSizepot + thresh2 < revSizepot || oldRevSizepot - thresh2 > revSizepot) {
      vs.revSize = ((float)pot_revSize / 1024 - 0.01);
      oldRevSizepot = revSizepot; // + thresh2 / 2;
      Serial.println("Rev size turn");
    }
  }

  if (memJustLoaded) {
    // Note: If we just entered memory mode, we need to reset our memory of what the knob positions were.
    // Otherwise we wind up with goofy glitching of a sort I'm not really in the mood for.
    memJustLoaded = false;
  }
  
  prevTimer = millis();
}

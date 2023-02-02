
} // End of loop()



void dumpVoiceSettings(void) {
  Serial.println(str + 
    "octave = " + vs.octave + "\n" +
    "octaveB = " + vs.octaveB + "\n" +
    "octaveC = " + vs.octaveC + "\n" +
    "shapeA = " + vs.shapeA + "\n" +
    "shapeB = " + vs.shapeB + "\n" +
    "shapeC = " + vs.shapeC + "\n" +
    "tuneB = " + vs.tuneB + "\n" +
    "tuneC = " + vs.tuneC + "\n" +
    "crossMod = " + vs.crossMod + "\n" +
    "vcoAvol = " + vs.vcoAvol + "\n" +
    "vcoBvol = " + vs.vcoBvol + "\n" +
    "vcoCvol = " + vs.vcoCvol + "\n" +
    "Subvol = " + vs.Subvol + "\n" +
    "cut = " + vs.cut + "\n" +
    "res = " + vs.res + "\n" +
    "filtAtt = " + vs.filtAtt + "\n" +
    "filtDec = " + vs.filtDec + "\n" +
    "filtRel = " + vs.filtRel + "\n" +
    "filtAmt = " + vs.filtAmt + "\n" +
    "filterMode = " + vs.filterMode + "\n" +
    "envAtt = " + vs.envAtt + "\n" +
    "envDec = " + vs.envDec + "\n" +
    "envRel = " + vs.envRel + "\n" +
    "envSus = " + vs.envSus + "\n" +
    "lfoAamp = " + vs.lfoAamp + "\n" +
    "lfoAfreq = " + vs.lfoAfreq + "\n" +
    "lfoAdel = " + vs.lfoAdel + "\n" +
    "lfoAatt = " + vs.lfoAatt + "\n" +
    "lfoAdec = " + vs.lfoAdec + "\n" +
    "lfoArel = " + vs.lfoArel + "\n" +
    "lfoAsus = " + vs.lfoAsus + "\n" +
    "lfoBamp = " + vs.lfoBamp + "\n" +
    "lfoBfreq = " + vs.lfoBfreq + "\n" +
    "dlyAmt = " + vs.dlyAmt + "\n" +
    "dlyTimeL = " + vs.dlyTimeL + "\n" +
    "dlyTimeR = " + vs.dlyTimeR + "\n" +
    "revMix = " + vs.revMix + "\n" +
    "revSize = " + vs.revSize + "\n" +
    "lfoAdest = " + vs.lfoAdest + "\n" +
    "lfoAshape = " + vs.lfoAshape + "\n" +
    "isMonophonic = " + vs.isMonophonic + "\n" +
    "portTime = " + vs.portTime
    );
}



// Reserve a bit of memory for global stuff, dunno if that's enough but w/e rn
#define PATCH_MEM_START_ADDR 8


void patchWrite(uint8_t whichPatch) {
  Serial.println(str + "patchWrite(" + whichPatch + ")");
  uint16_t memAddr = PATCH_MEM_START_ADDR + (whichPatch * sizeof(VoiceSettings));
//  Serial.println(str + "VoiceSettings is " + sizeof(VoiceSettings) + " bytes.");
  Serial.println(str + "Writing to addr " + memAddr + ".");
  EEPROM.put(memAddr, vs);
  lastMemLoaded = whichPatch;
  Serial.println("Finished.");
}


void patchRead(uint8_t whichPatch) {
  Serial.println(str + "patchRead(" + whichPatch + ")");
  uint16_t memAddr = PATCH_MEM_START_ADDR + (whichPatch * sizeof(VoiceSettings));
  Serial.println(str + "Reading from addr " + memAddr + ".");
//  Serial.println(str + "patchRead(" + vs);
  EEPROM.get(memAddr, vs);
  lastMemLoaded = whichPatch;
  Serial.println("Finished.");
//  Serial.println(str + "midiChannel = " + midiChannel);
}

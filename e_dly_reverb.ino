//Main ENVELOPE

for (uint8_t v = 0; v < NUM_VOICES; v++) {
  voice[v].env.attack(vs.envAtt);
  voice[v].env.decay(vs.envDec);
  voice[v].env.sustain(vs.envSus);
  voice[v].env.release(vs.envRel);
}


//delay
dlyL.delay(0, vs.dlyTimeL);
dlyMixL.gain(1, vs.dlyAmt * 0.9);

dlyR.delay(0, vs.dlyTimeR);
dlyMixR.gain(1, (vs.dlyAmt / 1.4 ) * 0.9);


//reverb
fxL.gain(1, vs.revMix);
fxR.gain(1, vs.revMix);

reverb.roomsize(vs.revSize);


//output gain reduction
fxL.gain(0, outGain - vs.revMix / 1.6);
fxL.gain(2, outGain - vs.revMix / 1.6);

fxR.gain(0, outGain - vs.revMix / 1.6);
fxR.gain(2, outGain - vs.revMix / 1.6);

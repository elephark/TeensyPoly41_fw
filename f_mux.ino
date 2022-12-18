
// lol debug

//for (uint8_t v = 0; (v < NUM_VOICES && v < NUM_LEDS); v++) {
//  led[v] = voice[v].env.isActive() ? 1 : 0;
//}

for (uint8_t v = 0; (v < NUM_VOICES && v < NUM_LEDS); v++) {
  led[v] = ((memActive) && (v == lastMemLoaded)) ? 1 : 0;
}

//if (btns[0]->fell()) { led[0] ^= 1; }

//led[0] = btn_memActive;
//led[1] = btn_memStore;
//led[2] = btn_mem[0];
//led[3] = btn_mem[1];
//led[4] = btn_mem[2];
//led[5] = btn_mem[3];
//led[6] = btn_mem[4];
//led[7] = btn_mem[5];


digitalWrite(LED_MEM_ACTIVE, memActive);
digitalWrite(LED_MEM_STORE, memStore);

//Multiplexer read

if (millis() - prevMuxTimer > muxInterval) {
  muxRead();
  prevMuxTimer = millis();
}


//// X0
//digitalWrite(MUX_A, 0); //A
//digitalWrite(MUX_B, 0); //B
//digitalWrite(MUX_C, 0); //C
//
//digitalWrite(MUX7_X, (led[0] ? 1 : 0));
//delay(wait);
//
//pot_mainVol = analogRead(MUX0_X);
//pot_oscCShape = analogRead(MUX1_X);
//pot_filtR = analogRead(MUX2_X);
//pot_lfoD = analogRead(MUX3_X);
//sw_mainOctLo = digitalRead(MUX4_X);
//sw_oscCOctLo = digitalRead(MUX5_X);
//btn_memActive = digitalRead(MUX6_X);
//
//
//// X1
//digitalWrite(MUX_A, 1);
//digitalWrite(MUX_B, 0);
//digitalWrite(MUX_C, 0);
//
//digitalWrite(MUX7_X, (led[1] ? 1 : 0));
//delay(wait);
//
//pot_portTime = analogRead(MUX0_X);
//pot_oscCTune = analogRead(MUX1_X);
//pot_envA = analogRead(MUX2_X);
//pot_lfoS = analogRead(MUX3_X);
//sw_oscAShapeHi = digitalRead(MUX4_X);
//sw_oscBShapeHi = digitalRead(MUX5_X);
//btn_memStore = digitalRead(MUX6_X);
//
//
//// X2
//digitalWrite(MUX_A, 0);
//digitalWrite(MUX_B, 1);
//digitalWrite(MUX_C, 0);
//
//digitalWrite(MUX7_X, (led[2] ? 1 : 0));
//delay(wait);
//
//pot_oscAVol = analogRead(MUX0_X);
//pot_filtCutoff = analogRead(MUX1_X);
//pot_envD = analogRead(MUX2_X);
//pot_pwmFreq = analogRead(MUX3_X);
//sw_oscAShapeLo = digitalRead(MUX4_X);
//sw_oscBShapeLo = digitalRead(MUX5_X);
//btn_mem6 = digitalRead(MUX6_X);
//
//
//// X3
//digitalWrite(MUX_A, 1);
//digitalWrite(MUX_B, 1);
//digitalWrite(MUX_C, 0);
//
//digitalWrite(MUX7_X, (led[3] ? 1 : 0));
//delay(wait);
//
//pot_crossMod = analogRead(MUX0_X);
//pot_filtRes = analogRead(MUX1_X);
//pot_envS = analogRead(MUX2_X);
//pot_pwmDepth = analogRead(MUX3_X);
//sw_mainOctHi = digitalRead(MUX4_X);
//sw_oscCOctHi = digitalRead(MUX5_X);
//btn_mem5 = digitalRead(MUX6_X);
//
//
//// X4
//digitalWrite(MUX_A, 0);
//digitalWrite(MUX_B, 0);
//digitalWrite(MUX_C, 1);
//
//digitalWrite(MUX7_X, (led[4] ? 1 : 0));
//delay(wait);
//
//pot_subVol = analogRead(MUX0_X);
//pot_filtEnvAmt = analogRead(MUX1_X);
//pot_envR = analogRead(MUX2_X);
//pot_revAmt = analogRead(MUX3_X);
//sw_oscBOctLo = digitalRead(MUX4_X);
//sw_lfoShapeLo = digitalRead(MUX5_X);
//btn_mem4 = digitalRead(MUX6_X);
//
//
//// X5
//digitalWrite(MUX_A, 1);
//digitalWrite(MUX_B, 0);
//digitalWrite(MUX_C, 1);
//
//digitalWrite(MUX7_X, (led[5] ? 1 : 0));
//delay(wait);
//
//pot_oscBVol = analogRead(MUX0_X);
//pot_filtA = analogRead(MUX1_X);
//pot_lfoFreq = analogRead(MUX2_X);
//pot_revSize = analogRead(MUX3_X);
//sw_isMonophonic = digitalRead(MUX4_X);
//sw_lfoDestHi = digitalRead(MUX5_X);
//btn_mem3 = digitalRead(MUX6_X);
//
//
//isMonophonic = sw_isMonophonic;
//
//
//// X6
//digitalWrite(MUX_A, 0);
//digitalWrite(MUX_B, 1);
//digitalWrite(MUX_C, 1);
//
//digitalWrite(MUX7_X, (led[6] ? 1 : 0));
//delay(wait);
//
//pot_oscBTune = analogRead(MUX0_X);
//pot_filtD = analogRead(MUX1_X);
//pot_lfoDepth = analogRead(MUX2_X);
//pot_dlyAmt = analogRead(MUX3_X);
//sw_oscBOctHi = digitalRead(MUX4_X);
//sw_lfoShapeHi = digitalRead(MUX5_X);
//btn_mem2 = digitalRead(MUX6_X);
//
//
//// X7
//digitalWrite(MUX_A, 1);
//digitalWrite(MUX_B, 1);
//digitalWrite(MUX_C, 1);
//
//digitalWrite(MUX7_X, (led[7] ? 1 : 0));
//delay(wait);
//
//pot_oscCVol = analogRead(MUX0_X);
//pot_filtS = analogRead(MUX1_X);
//pot_lfoA = analogRead(MUX2_X);
//pot_dlyRate = analogRead(MUX3_X);
//sw_filtMode = digitalRead(MUX4_X);
//sw_lfoDestLo = digitalRead(MUX5_X);
//btn_mem1 = digitalRead(MUX6_X);

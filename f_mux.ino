
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

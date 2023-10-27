# TeensyPoly41_fw
Firmware for TeensyPoly41 PCB

TeensyPoly41 is a polyphonic synthesizer built on the [Teensy 4.1](https://www.pjrc.com/store/teensy41.html) platform, using the [Teensy Audio](https://github.com/PaulStoffregen/Audio) library. 

This is originally based on the [TeensyPoly6](https://github.com/albnys/TeensyPoly6) codebase, but with many changes of various significance:

* It runs on the [TeensyPoly41 RevA](https://github.com/elephark/TeensyPoly41) hardware, with all the supporting changes that necessitates.
* Many sections have been rewritten to use arrays rather than 6 explicit copies of everything.
* It now has 16 voices, and could be expanded to more without much effort. 16 is more than enough for anything that would (normally) be asked of this sort of synth, though.
* Voice allocation, while still probably not optimal, is smoother and more musical.
* I added one of my favorite uncommon features: pitch bend doesn't affect voices in the release stage.

As of this writing there remain some unfinished features and known bugs, and the code is pretty messy. Even so, it'll still definitely make music if you ask it nicely (using the right MIDI messages).

This is a USB-powered, Teensy-based interface that has 16 CV outputs and 12 gate outputs. 
USB MIDI channels 1 - 8 are mapped to CV outputs 1 - 16 for pitch and velocity, and gates 1 - 8 for note on and off events. 
Gates 9 - 12 are mapped to note on and off events only on channel 9, 
and also will send a sync and transport signal based on MIDI clock messages if received. 
CV is 12 bit from 0V to 4V, gates 1 - 8 are 0 and 5V, and gates 9 - 12 are 0 and 3.3v.

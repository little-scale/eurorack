This Teensy-based project has 12 gate outputs that are set using note messages over USB MIDI. 

Each note in the scale (C, C#, D, D# etc) corresponds to a gate output labelled G1 - G12, and will set high from a note on message and low from a note off message. 

If a USB MIDI clock sync is sent, Gate 12 will output a short trigger upon sequencer START or CONTINUE. 
Gate 11 will output a sync signal at a rate of 12PPQN. 
Gate 10 will output a sync signal at a rate of 16th notes. 
Gate 9 will output a sync signal at a rate of 8th notes. 

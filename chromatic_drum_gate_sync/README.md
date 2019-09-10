This Teensy-based project has 12 gate outputs that are set using note messages over USB MIDI. 

For USB MIDI Channel 1: 

Each note in the scale (C, C#, D, D# etc) corresponds to a gate output labelled G1 - G12, and will set high from a note on message and low from a note off message. This is useful for controlling twelve drum modules using the gate input on each module. 

For USB MIDI Channel 2: 

The first six notes in the scale (C, C#, D, D# etc) corresponds to a gate output labelled G1 - G6, and will set high from a note on message and low from a note off message. A velocity above 99 will also set a gate output labelled G7 - G12 high, with a velocity below or equal to 99 will also set a gate output labelled G7 - G12 low. This is useful for controlling six drum modules using both the gate input and accent inputs on each module. 

For USB MIDI Clock: 

If a USB MIDI clock sync is sent, Gate 12 will output a short trigger upon sequencer START or CONTINUE. 
Gate 11 will output a sync signal at a rate of 12PPQN. 
Gate 10 will output a sync signal at a rate of 16th notes. 
Gate 9 will output a sync signal at a rate of 8th notes. 

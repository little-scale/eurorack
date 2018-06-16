# Exo Sequencer

A sixteen step sequencer with CV and trigger out. 

The step order is selected via a number of ways: 

• A gate signal sent to the INC input will increment the sequencer

• A gate signal sent to the DEC input will increment the sequencer

• An address as a four bit binary value (from 0000 to 1111) sent to A0, A1, A2 and A3 will select that step

• A voltage value sent to the STEP input will select that step

These methods of step selection are not mutually exclusive, and will operate simulatenously. 

The length control sets the length of the pattern. This can be modulated using a control voltage. 
The offset control sets the offset of the pattern. This can be modulated using a control voltage. 

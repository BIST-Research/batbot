.. _chirp-operation:

###############
Chirp Operation
###############

**********************
Single Chirp Operation
**********************

**************************
Continuous Chirp Operation
**************************

*****************
Component Testing
*****************

It is the nature of lab work and engineering in general that components will fail, often without warning. It is important to be able to test individual components of the sonar system
to ensure that they are working properly and to be able to diagnose and identify issues when they arise. Most components can be tested individually, as detailed in this section. It is very
rare that multiple components will fail at once, so much of the testing will be done using other components as part of the test. 

Microphone and Transducer
^^^^^^^^^^^^^^^^^^^^^^^^^

Testing of the microphone and transducer can be done using the SDG 1032X function generator within the lab. Set the function generator to output a sine wave at your desired frequency (50kHz is a good baseline),
and connect the output of the function generator to the JP4 pins, removing the jumper pins which reside on these pins. Run a single chirp, and you should see a strong signal at the frequency you set on the function generator.
It should appear as a horizontal line on the spectrogram. If this line appears, the microphone and transducer are both working properly, and the amplifier is very likely to also be working properly. If this line does not appear,
first test the microphone by disconnecting the transducer wires from the system, and playing a sound at a known frequency (Youtube often has videos playing a specific frequency), being sure to adjust your spectrogram bounds to include your chosen frequency.
If you see a line at the frequency of the sound you played, the microphone is working properly. If you do not see a line, the microphone is likely faulty and should be replaced. If the microphone is working properly and the original test did not show a line,
the transducer or amplifier is likely faulty. To test the transducer, simply swap it out for a known working transducer (potentially multiple others if needed) and repeat the original test. If a line still does not appear, the amplifer is likely the issue.

Amplifier
^^^^^^^^^

The easiest way to tell if the amplifier is faulty is to observe the power draw from the power supply. If the draw is <0.05A and/or the fan on top is not spinning, the amplifier may be faulty. If it is drawing >0.15A, the amplifier is likely working properly. 
Refer to the :ref:`setup-and-assembly` section for more details on amplifier positioning and power draw, as it is very common for a working amplifier to be misdiagnosed as faulty due to poor contact with the PCB.

ItsyBitsy M4
^^^^^^^^^^^^

TODO: explain how the easiest method testing itsybitsy is to print confirmation after sending chirp signal.

PCB Board
^^^^^^^^^

There is no easy way to test the PCB board itself, but if all other components have been tested and are working properly, the board is likely the issue. One way to spot a faulty board 
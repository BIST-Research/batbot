.. _hardware-setup:

Hardware Setup
=====================

Sonar System
-------------

TODO

Tendon Actuation System
-----------------------

Prerequisites
^^^^^^^^^^^^^^^^^^^^

The tendon actuation system consists of a pair of identical bat ear modules. A single module is built with the following components:
- 5x Pololu micro metal gear motors and encoders
- 6-pin JST connectors for the motors and encoders
- 1x Adafruit Grand Central M4 Express microcontroller
- 1x BIST custom motor control PCB shield
- 3D printed components for the ear, tendon pulleys, and motor mounts

Assembly
^^^^^^^^^
The following steps outline the assembly of a single bat ear module (the steps are identical for both ears):

1. Preparing the Motor Shield
    It is recommended to order the BIST custom motor control PCB shield preassembled. If you opt to assemble it 
    by hand, please refer to the design schematic and BOM available in the BIST Fusion 360 repository. Otherwise,
    the preassembled shield can simply be mounted onto the Grand Central M4 Express microcontroller. The shield 
    provides a set of JST connectors for the motors and encoders, as well as two power input connectors for the battery.
    
    .. note::
        Due to pin selection of the current PCB, the motor control software only allows for control of the 5 motor connections highlighted below:
        
        TODO: Add an image

2. Preparing Motor Encoder assembly
    Pololu provides a set of micro metal gear motors with attached encoders. However, the lab also has a set of motors 
    without encoders. If you are using the latter, you will need to manually solder the encoder PCB to the motors like so:

    TODO: Add an image

    The encoder PCBs have 6-pin JST headers which can be hooked up to the motor connectors specified in step 1 via the 6-pin JST connector cables.

3. Preparing the Tendon Pulleys
    TODO: add a description

4. Assembling the Ear
    TODO: add a description
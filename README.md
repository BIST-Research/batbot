# batbot 

Welcome to the batbot repository!

This repository contains the sources for the following packages:

- `batbot_bringup` - Applications for interfacing with sonar and motor boards from a host PC
- `batbot_tendon_controller` - PlatformIO application for controlling tendons
- `batbot_sonar` - PlatformIO application for the sonar board(s)

### Tendon Control

On the motor board:

Build and upload the `batbot7_tendon_controller` application to your motor board using PlatformIO.

On the host computer:

`python bb_gui.py` - this will launch the GUI used to control the tendons.

Alternatively, `pinnae.py` maybe used to bringup a GUI that ONLY sends motor control commands.

### Sonar

On the sonar board:

Build and upload the `batbot7_sonar` application. You will need to specify the PlatformIO environment to upload. The environment options are the following:
- teensy41-emit
- teensy41-listen

On the host computer:

`python bb_gui.py` - this will launch the GUI used to send and receive chirps.

Alternatively, `receive.py` maybe used to view the microphone data received by the sonar board.


.. _sonar-software:

####################
Sonar Software Guide
####################

The software for the sonar system is mainly housed within the `fieldbot <https://github.com/BIST-Research/fieldbot>`__ repository within the BIST Research Github.
The sonar software provides the ability to send and recieve sonar "chirps", and visualize the 
data in a spectrogram and waveform display. A chirp is a frequency sweep from high to low frequency over a small period of time,
which in our lab is designed to replicate the echolocation abilities of bats. 

In order to use the sonar software, the following steps must be followed:

- Install PlatformIO IDE extension in VSCode and ensure that you are able to establish connection with the ItsyBitsy M4 microcontroller.
- This often will not work at first, and may require additonal steps. Refer to the `PlatformIO documentation <https://docs.platformio.org/en/latest/integration/ide/vscode.html#ide-vscode>`__ for more details. 
- Install Python to your computer from `python.org <https://www.python.org/downloads/>`__ and ensure that it is added to your system PATH.
- While in the fieldbot repository, run the following command in the terminal to install the required Python packages:
``pip install -r requirements.txt``

Once these steps have been taken, you should be able to run the sonar software. Operation instructions can be found in
:ref:`chirp-operation`.

*********************
Sonar System Overview
*********************

Sending A Chirp
===============

TODO: Explain the process of sending a chirp, including how the signal travels through the board, amp, etc.

Recieiving A Chirp
==================

TODO: Explain the process of recieving a chirp

Processing Chirp Datas
=====================

TODO: Explain how the data is processed, including filtering, FFT, etc.
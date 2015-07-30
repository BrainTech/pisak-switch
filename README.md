##pisak-switch

Pisak-switch is a software created for Wattuino Nanite 85 (https://github.com/watterott/wattuino).
It contains a subfolder "usbdrv" from V-USB library (http://www.obdev.at/avrusb/)
with configuration files customized to fit the hardware requirements.
The software is based on tutorial written by Joonas Pihlajamaa (http://codeandlife.com/).

To put the software on your Nanite use the following command:
>`$ make flash`

After changes, you can compile and put your software on the Nanite by typing:
>`$ make clean` <br />
>`$ make main.hex` <br />
>`$ make flash`

Be sure to have a copy of Micronucleus (https://github.com/micronucleus/micronucleus)
installed on your device! After typing "make flash" command just plug in your Nanite 85
to a USB port. If needed, press the reset button on the microcontroller.

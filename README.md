Pisak-switch is a software created for Wattuino Nanite 85 (https://github.com/watterott/wattuino). It contains a subfolder "usbdrv" from V-USB library (http://www.obdev.at/avrusb/) with configuration files customized to fit the hardware requirements. The software is based on tutorial written by Joonas Pihlajamaa (http://codeandlife.com/).

To put the software on your Nanite use the following command:
$ micronucleus --run main.hex

After changes, you can compile and put your software on the Nanite by typing:
$ make clean
$ make main.hex
$ micronucleus --run main.hex

Be sure to have a copy of Micronucleus (https://github.com/micronucleus/micronucleus) installed on your device!

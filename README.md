##pisak-switch

Pisak-switch is a firmware created for Wattuino Nanite 85 (https://github.com/watterott/wattuino).
By connecting microswitch to PB1 it's possible to create a simple switch HID (eg. button)
that can be used to control interfaces such as in PISAK project.
The firmware contains a subfolder "usbdrv" from V-USB library (http://www.obdev.at/avrusb/)
with configuration files customized to fit the hardware requirements.
The tutorials written by Joonas Pihlajamaa (http://codeandlife.com/) were very helpful
during developing this firmware.

To put the software on your Nanite use the following command:
>`$ make flash`

After changes, you can compile and put your software on the Nanite by typing:
>`$ make clean` <br />
>`$ make main.hex` <br />
>`$ make flash`

Be sure to have a copy of Micronucleus (https://github.com/micronucleus/micronucleus)
installed on your device! After typing "make flash" command just plug in your Nanite 85
to a USB port. If needed, press the reset button on the microcontroller.


## License

PISAK is licensed under the terms of the GNU GPL version 3.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


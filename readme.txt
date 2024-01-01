--------------------------------------------------------------------------------
Test for desoldered Gotek's LED screens
Copyright (c) 2024 Wierzbowsky [RBSC]
Version 1.0
--------------------------------------------------------------------------------

About
-----

This project was created to test Gotek's 3-digit LED screens after desoldering them from the original boards. It appears
that desoldering might damage the screen and make one or more segments unusable.


Assembling
----------

Take the orginal Gotek LED screen board and desolder the LED screen, the controller chip and the capacitor. It's better
to use the desoldering station (hot air) or you can use infrared reworking station. Then solder the chip and the capacitor
onto the small board (you can order it from jlcpcb.com factory using the provided Gerber files).

The board's sources and Gerber files can be found in this project:

https://github.com/Wierzbowsky/GotekLEDScreenClock

Solder 6-pin single row female round pin headers onto the board from the screen side. Those will be used to insert tested
screens. Finally solder an angled 4-pin single row header to the right or left side of one assembled board.


Connections
-----------

Take any Arduino Atmega328P board (Nano, Uno, Mini, etc) and connect the pins of the small board with jumper wires as
follows:

 VCC -> 3.3v pin of Arduino board
 GND -> GND pin of Arduino board
 CLK -> D3 pin of Arduino board
 DIO -> D2 pin of Arduino board

Open the Arduino project in the IDE environment. Set up the IDE for your board (board type, port, etc.). Upload the firmware
into the Arduino board. Once uploaded, the firmware will light up all segments of the Gotek's LED screen except for the dots.
Then it will test the brightness levels and finally initiate a countdown to zero. Finally, the screen will be switched off.
You can remove the tested screen once it is no longer lit.


Operation and test videos
-------------------------

You can see the videos of the screen testing on YouTube:

 - https://youtu.be/1x14cKezyLI


IMPORTANT!
----------

I provide all files and information for free, without any liability (see the disclaimer.txt file). The provided information,
software or hardware must not be used for commercial purposes unless permitted by the owner. Producing a small amount of
bare boards for personal projects and selling the rest of the batch is allowed without the permission.

When the sources files are used to create alternative projects, please always mention the original source and the copyright!


Contact information
-------------------

The members of RBSC group Tnt23, Wierzbowsky, Pyhesty, Ptero, GreyWolf, SuperMax, VWarlock and DJS3000 can be contacted via the group's e-mail
address:

info@rbsc.su

The group's coordinator could be reached via this e-mail address:

admin@rbsc.su

The group's website can be found here:

https://rbsc.su/
https://rbsc.su/ru

The RBSC's hardware repository can be found here:

https://github.com/rbsc

The RBSC's 3D model repository can be found here:

https://www.thingiverse.com/groups/rbsc/things

-= ! MSX FOREVER ! =-

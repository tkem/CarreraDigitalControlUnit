# CarreraDigitalControlUnit  [![Build Status](https://travis-ci.com/tkem/CarreraDigitalControlUnit.svg?branch=master)](https://travis-ci.com/tkem/CarreraDigitalControlUnit/)

This is a cross-platform library for Arduino and mbed OS for
interfacing a microcontroller board with the Carrera® DIGITAL 124/132
slot car racing system.  Data transmission uses a single pair of wires
connected directly to the slot car tracks.  Be aware, though, that the
Carrera® Control Unit provides 14.8V (D132) or 18V (D124), so you have
to convert that to 5V or 3.3V, depending on your platform, or you will
most certainly damage your hardware.  For starters, a voltage divider
made of two resistors and an (optional but recommended) reverse
polarity protection diode will do:

| ![Wiring](http://www.wasserstoffe.de/carrera-hacks/protocol-decode/carrera-decode-Steckplatine.png) |
|:---:|
| Image by [Peter Niehues](http://www.wasserstoffe.de/carrera-hacks/) [[CC BY-NC-SA](https://creativecommons.org/licenses/by-nc-sa/3.0/)] |

This project is still at an early stage and documentation leaves a lot
to be desired, so you best have a look at the existing
[examples](examples) for now.  To make the most out of this, you
might also want to make yourself somewhat familiar with the [CU data
protocol](http://slotbaer.de/carrera-digital-124-132/9-cu-daten-protokoll.html)
(available in German only).


## License

Copyright (c) 2017, 2021 Thomas Kemmer.

Licensed under the [Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0).

Carrera® is a registered trademark of [Carrera Toys GmbH](https://carrera-toys.com/).

This is not an official Carrera® product, and is not affiliated with
or endorsed by Carrera Toys GmbH.

Thanks to [Stephan Heß](http://www.slotbaer.de/) and [Peter
Niehues](http://www.wasserstoffe.de/carrera-hacks/) for doing all the
hard work.

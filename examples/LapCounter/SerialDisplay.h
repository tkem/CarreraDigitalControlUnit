/*
   Copyright 2017 Thomas Kemmer

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef SERIAL_DISPLAY_H
#define SERIAL_DISPLAY_H

#include <mbed.h>

class SerialDisplay {
    RawSerial _serial;

public:

    SerialDisplay(PinName tx, PinName rx, long baud = 9600);

    void clear();

    void show(int n);
};

#endif

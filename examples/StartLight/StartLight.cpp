/*
   Copyright 2017, 2021 Thomas Kemmer

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
#ifndef ARDUINO

#include "CarreraDigitalControlUnit.h"

#include <mbed.h>

// set digital pin #2 as input - make sure it does not deliver more
// than 3.3V!
CarreraDigitalControlUnit cu(D2);

// set digital pins 3 to 7 as outputs (connected to LEDs)
mbed::DigitalOut led1(D3);
mbed::DigitalOut led2(D4);
mbed::DigitalOut led3(D5);
mbed::DigitalOut led4(D6);
mbed::DigitalOut led5(D7);

int main() {
    cu.start();

    while (true) {
        uint8_t prog[3];
        int data = cu.read();
        if (cu.parse_prog(data, prog)) {
            // prog := { command, value, address }
            if (prog[0] == 16 && prog[2] == 7) {
                led1 = prog[1] >= 1;
                led2 = prog[1] >= 2;
                led3 = prog[1] >= 3;
                led4 = prog[1] >= 4;
                led5 = prog[1] >= 5;
            }
        }
    }
}

#endif

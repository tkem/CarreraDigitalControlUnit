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
#include <mbed.h>

#include "CarreraDigitalControlUnit.h"

// set digital pin 2 as input - make sure it does not deliver more
// than 5V or 3.3V, depending on platform!
CarreraDigitalControlUnit cu(D2);

// set digital pins 3 to 7 as outputs (connected to LEDs)
DigitalOut led1(D3);
DigitalOut led2(D4);
DigitalOut led3(D5);
DigitalOut led4(D6);
DigitalOut led5(D7);

// TODO: more efficient implementation?
inline uint8_t lsb3(int v) {
    return ((v & 0x1) << 2) | (v & 0x2) | ((v & 0x4) >> 2);
}

inline uint8_t lsb4(int v) {
    return ((v & 0x1) << 3) | (v & 0x2) << 1 | (v & 0x4) >> 1 | ((v & 0x8) >> 3);
}

inline uint8_t lsb5(int v) {
    return ((v & 0x1) << 4) | (v & 0x2) << 2 | (v & 0x4) | (v & 0x8) >> 2 | ((v & 0x10) >> 4);
}

void setup() {
    cu.start();
}

void loop() {
    int data = cu.read();
    if (data & 0x1000) {
        uint8_t w = lsb4(data >> 8);
        uint8_t b = lsb5(data >> 3);
        uint8_t r = lsb3(data);

        if (b == 16 && r == 7) {
            led1 = w >= 1;
            led2 = w >= 2;
            led3 = w >= 3;
            led4 = w >= 4;
            led5 = w >= 5;
        }
    }
}

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

mbed::Serial pc(USBTX, USBRX);

int lap = 0;

void setup() {
    cu.start();
}

void loop() {
    uint8_t prog[3];
    int data = cu.read();
    if (cu.split_programming_word(data, prog)) {
        // prog = { value, command, address }
        const uint8_t value = prog[0];
        const uint8_t command = prog[1];
        const uint8_t address = prog[2];

        switch (command) {
        case 6:
            if (value == 9 && address == 0) {
                pc.puts("New race\r\n");
                lap = 0;
            } else {
                pc.printf("#%d: Position %d\r\n", address, value);
            }
            break;
        case 8:
            pc.printf("#%d: New fastest lap\r\n", address);
            break;
        case 9:
            pc.printf("#%d: Lap finished\r\n", address);
            break;
        case 11:
            pc.printf("#%d: False start\r\n", address);
            break;
        case 17:
            lap = (lap & 0x0f) | (value << 4);
            break;
        case 18:
            lap = (lap & 0xf0) | value;
            pc.printf("Lap %d\r\n", lap);
            break;
        }
    }
}

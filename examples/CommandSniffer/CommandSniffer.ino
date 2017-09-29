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
#include "CarreraDigitalControlUnit.h"

#include <mbed.h>

// set digital pin 2 as input - make sure it does not deliver more
// than 5V or 3.3V, depending on platform!
CarreraDigitalControlUnit cu(D2);

mbed::Serial pc(USBTX, USBRX);

bool show_normal_operation = true;

void setup() {
    cu.start();
}

void loop() {
    uint8_t prog[3];
    int data = cu.read();
    if (cu.parse_prog(data, prog)) {
        // prog := { command, value, address }
        const uint8_t command = prog[0];
        const uint8_t value = prog[1];
        const uint8_t address = prog[2];

        switch (command) {
        case 0:
            pc.printf("#%d: Set speed: %d\r\n", address, value);
            break;
        case 1:
            pc.printf("#%d: Set brake: %d\r\n", address, value);
            break;
        case 2:
            pc.printf("#%d: Set fuel: %d\r\n", address, value);
            break;
        case 4:
            if (value >= 8) {
                pc.printf("#%d: Blinking (value=%d)\r\n", address, value);
            } else if (show_normal_operation) {
                pc.printf("#%d: Operational (value=%d)\r\n", address, value);
            }
            break;
        case 5:
            pc.printf("%d: %s pit (value=%d)\r\n", address, value ? "Enter" : "Exit", value);
            break;
        case 6:
            if (value == 9) {
                pc.printf("#%d: Reset lap count and positions\r\n", address);
            } else {
                pc.printf("#%d: Position %d\r\n", address, value);
            }
            break;
        case 7:
            pc.printf("#%d: Race finished (value=%d)\r\n", address, value);
            break;
        case 8:
            pc.printf("#%d: New fastest lap (value=%d)\r\n", address, value);
            break;
        case 9:
            pc.printf("#%d: Lap finished (value=%d)\r\n", address, value);
            break;
        case 10:
            if (value == 15) {
                pc.printf("#%d: Turn off/reset fuel display\r\n", address);
            } else {
                pc.printf("#%d: Fuel: %d\r\n", address, value);
            }
            break;
        case 11:
            pc.printf("#%d: False start (value=%d)\r\n", address, value);
            break;
        case 16:
            pc.printf("#%d: Startlight: %d\r\n", address, value);
            break;
        case 17:
            pc.printf("#%d: Lap high: %d\r\n", address, value);
            break;
        case 18:
            pc.printf("#%d: Lap low: %d\r\n", address, value);
            break;
        case 19:
            pc.printf("#%d: Reset (value=%d)\r\n", address, value);
            break;
        case 20:
            if (value == 15) {
                pc.printf("#%d: Pit lane test\r\n", address);
            } else {
                pc.printf("#%d: Pit lane mode: %d\r\n", address, value);
            }
            break;
        default:
            pc.printf("#%d: ??? (command=%d, value=%d)\r\n", address, command, value);
        }
    }
}

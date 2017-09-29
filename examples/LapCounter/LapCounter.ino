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

volatile uint8_t lap = 0;
volatile bool valid = false;

// this should be short and fast enough to be called from an ISR
void update(int data) {
    static uint8_t tmp = 0;
    uint8_t prog[3];
    if (cu.parse_prog(data, prog)) {
        // prog := { command, value, address }
        switch (prog[0]) {
        case 6:
            if (prog[1] == 9 && prog[2] == 0) {
                valid = false;
            }
            break;
        case 17:
            tmp = prog[1] << 4;
            break;
        case 18:
            lap = prog[1] | tmp;
            valid = true;
            break;
        }
    }
}

void setup() {
    cu.attach(update);
    cu.start();
}

void loop() {
    static int previous = -2;
    int current = valid ? lap : -1;
    if (current != previous) {
        // update your display here (which may take more than 7.5ms)
        if (current >= 0) {
            pc.printf("Lap %d\r\n", current);
        } else {
            pc.printf("Lap n/a\r\n");
        }
        previous = current;
    }
}

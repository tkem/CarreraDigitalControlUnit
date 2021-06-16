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
#include "CarreraDigitalControlUnit.h"

// set digital pin #2 as input - make sure it does not deliver more
// than 5V or 3.3V, depending on platform!
CarreraDigitalControlUnit cu(2);

// connect digital pins #3 to #7 to LEDs
const int led1 = 3;
const int led2 = 4;
const int led3 = 5;
const int led4 = 6;
const int led5 = 7;

void setup() {
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    pinMode(led5, OUTPUT);

    cu.start();
}

void loop() {
    uint8_t prog[3];
    int data = cu.read();
    if (cu.parse_prog(data, prog)) {
        // prog := { command, value, address }
        if (prog[0] == 16 && prog[2] == 7) {
            digitalWrite(led1, prog[1] >= 1);
            digitalWrite(led2, prog[1] >= 2);
            digitalWrite(led3, prog[1] >= 3);
            digitalWrite(led4, prog[1] >= 4);
            digitalWrite(led5, prog[1] >= 5);
        }
    }
}

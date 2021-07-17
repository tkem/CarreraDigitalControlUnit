/*
   Copyright 2021 Thomas Kemmer

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

#if !defined(ARDUINO)
PinName cuPin = D2;
#elif defined(ARDUINO_ARCH_MBED)
PinName cuPin = p2;
REDIRECT_STDOUT_TO(Serial);
#elif defined(ARDUINO_ARCH_ESP8266)
int cuPin = D2;
#else
int cuPin = 2;
#define printf(...) { char buf[80]; sprintf(buf, __VA_ARGS__); Serial.print(buf); }
#endif

// set digital pin #2 as input - make sure it does not deliver more
// than 5V or 3.3V, depending on platform!
CarreraDigitalControlUnit cu(cuPin);

int currentLap = -1;
int previousLap = -2;
uint8_t lapHigh = 0;

void setup() {
#ifdef ARDUINO
    Serial.begin(115200);
#endif
    cu.start();
}

void loop() {
    if (CarreraCommandPacket packet = cu.read()) {
        switch (packet.command()) {
        case 6:
            if (packet.value() == 9 && packet.address() == 0) {
                currentLap = -1;  // reset lap counter on START key
            }
            break;
        case 17:
            // higher nibble (4 bits) of lap count, sent before lower nibble
            lapHigh = packet.value() << 4;
            break;
        case 18:
            // lower nibble (4 bits) of lap count, sent after higher nibble
            currentLap = lapHigh | packet.value();
            break;
        }
        if (currentLap != previousLap) {
            if (currentLap >= 0) {
                printf("Lap %d\r\n", currentLap);
            } else {
                printf("Lap n/a\r\n");
            }
            previousLap = currentLap;
        }
    }
}

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
#include "SerialDisplay.h"
#include "TM1637Display.h"

#include <mbed.h>

// uncomment to use TM1637 4-digit LED display
//#define USE_TM1637_DISPLAY 1

#ifdef USE_TM1637_DISPLAY
TM1637Display display(D4, D5);
#else
SerialDisplay display(USBTX, USBRX);
#endif

// set digital pin 2 as input - make sure it does not deliver more
// than 5V or 3.3V, depending on platform!
CarreraDigitalControlUnit cu(D2, false /* set true for logically inverted input */);

// pwm output for showing safety car phase, etc.
PwmOut led(D10);

// set by interrupt handler
static volatile struct {
    uint8_t lap;
    bool set;
    bool stopped;
    bool pacecar;
} device;

// this should be short and fast enough to be called from an ISR
void update(int data) {
    static uint8_t tmp = 0;
    uint8_t values[4];
    if (cu.parse_prog(data, values)) {
        // values := { command, value, address }
        switch (values[0]) {
        case 6:  // (re)set lap counter/position tower
            device.set = (values[1] == 9 && values[2] == 0) ? false : device.set;
            break;
        case 17:  // current lap high nibble
            tmp = values[1] << 4;
            break;
        case 18:  // current lap low nibble
            device.lap = values[1] | tmp;
            device.set = true;
            break;
        case 19:  // reset
            device.set = false;
            break;
        }
    } else if (cu.parse_pace(data, values)) {
        // values := { stop, return, active, fuel }
        device.stopped = values[0];
        device.pacecar = values[2];
    }
}

void setup() {
    display.clear();
    led.period(1.0);
    led.write(0);
    cu.attach(update);
    cu.start();
}

void loop() {
    static uint8_t lap = 0;
    static unsigned overflow = 0;
    static bool clear = true;
    static bool stopped = false;
    static bool pacecar = false;

    if (device.set) {
        uint8_t culap = device.lap;
        if (culap < lap) {
            ++overflow;
        }
        if (lap != culap || clear) {
            lap = culap;
            display.show((overflow << 8) | lap);
            clear = false;
        }
    } else if (!clear) {
        display.clear();
        lap = 0;
        overflow = 0;
        clear = true;
    }

    if (device.stopped != stopped || device.pacecar != pacecar) {
        stopped = device.stopped;
        pacecar = device.pacecar;

        if (stopped) {
            led.write(1.0f);
        } else if (pacecar) {
            led.write(0.5f);
        } else {
            led.write(0.0f);
        }
    }
}

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

// printf() is fine for debugging, but we need something more
// efficient here...
class DataLogger {
    RawSerial _serial;

public:
    template<class TX, class RX>
    DataLogger(TX tx, RX rx, long baud) : _serial(tx, rx, baud) {}

    DataLogger& operator<<(const char* s) {
        _serial.puts(s);
        return *this;
    }

    DataLogger& operator<<(bool f) {
        _serial.putc(f ? '1' : '0');
        return *this;
    }

    DataLogger& operator<<(uint8_t v) {
        if (v >= 100) {
            _serial.putc('0' + v / 100);
            v %= 100;
        }
        if (v >= 10) {
            _serial.putc('0' + v / 10);
            v %= 10;
        }
        _serial.putc('0' + v);
        return *this;
    }

    DataLogger& operator<<(uint16_t v) {
        static const char hex[] = "0123456789abcdef";
        _serial.putc(hex[(v >> 12) & 0xf]);
        _serial.putc(hex[(v >> 8) & 0xf]);
        _serial.putc(hex[(v >> 4) & 0xf]);
        _serial.putc(hex[v & 0xf]);
        _serial.putc('h');  // hex
        return *this;
    }
};

DataLogger out(USBTX, USBRX, 115200);

// set digital pin 2 as input - make sure it does not deliver more
// than 5V or 3.3V, depending on platform!
CarreraDigitalControlUnit cu(D2);

void setup() {
    cu.start();
}

void loop() {
    uint8_t values[4];  // maximum number of components
    int data = cu.read();
    if (cu.split_programming_word(data, values)) {
        // values = { command, value, address }
        out << uint16_t(data) << " [PROG:"
            << " COMMAND=" << values[0]
            << " VALUE=" << values[1]
            << " ADDRESS=" << values[2]
            << "]\r\n";
    } else if (cu.split_controller_word(data, values)) {
        // values = { address, speed, button, fuel }
        out << uint16_t(data) << " [CTRL:"
            << " ADDRESS=" << values[0]
            << " SPEED=" << values[1]
            << " BUTTON=" << values[2]
            << " FUEL=" << values[3]
            << "]\r\n";
    } else if (cu.split_pacecar_word(data, values)) {
        // values = { mode, box, active, fuel }
        out << uint16_t(data) << " [PACE:"
            << " MODE=" << values[0]
            << " BOX=" << values[1]
            << " ACTIVE=" << values[2]
            << " FUEL=" << values[3]
            << "]\r\n";
    } else if (cu.split_acknowledge_word(data, values)) {
        // values = { slot }
        out << uint16_t(data) << " [ACK:"
            << " SLOTS=" << values[0]
            << "]\r\n";
    } else if (cu.split_active_word(data, values)) {
        // values = { mask, any }
        out << uint16_t(data) << " [ACT:"
            << " MASK=" << values[0]
            << " ANY=" << values[1]
            << "]\r\n";
    } else {
        out << uint16_t(data) << " [???]\r\n";
        wait(1);
        cu.reset();  // probably lost sync
    }
}

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

// use digital pin 2 as input - make sure it does not deliver more
// than 5V or 3.3V, depending on platform!

#if !defined(ARDUINO)
PinName cuPin = D2;
#elif defined(ARDUINO_ARCH_MBED)
PinName cuPin = p2;
REDIRECT_STDOUT_TO(Serial);
#elif defined(ARDUINO_ARCH_ESP8266)
int cuPin = D2;
#define fputc(c, file) Serial.write(char(c))
#define fputs(s, file) Serial.print(s)
#else
int cuPin = 2;
#define fputc(c, file) Serial.write(char(c))
#define fputs(s, file) Serial.print(s)
#endif

CarreraDigitalControlUnit cu(cuPin);

// printf() is fine for debugging, but we need something more
// efficient here...
class DataLogger {
public:
    DataLogger() {}

    DataLogger& operator<<(const char* s) {
        fputs(s, stdout);
        return *this;
    }

    DataLogger& operator<<(bool f) {
        fputc(f ? '1' : '0', stdout);
        return *this;
    }

    DataLogger& operator<<(uint8_t v) {
        if (v >= 100) {
            fputc('0' + v / 100, stdout);
            v %= 100;
        }
        if (v >= 10) {
            fputc('0' + v / 10, stdout);
            v %= 10;
        }
        fputc('0' + v, stdout);
        return *this;
    }

    DataLogger& operator<<(uint16_t v) {
        static const char hex[] = "0123456789abcdef";
        fputc(hex[(v >> 12) & 0xf], stdout);
        fputc(hex[(v >> 8) & 0xf], stdout);
        fputc(hex[(v >> 4) & 0xf], stdout);
        fputc(hex[v & 0xf], stdout);
        fputc('h', stdout);  // hex
        return *this;
    }
};

DataLogger out;

void setup() {
#ifdef ARDUINO
    Serial.begin(115200);
#endif
    cu.start();
}

void loop() {
    int data = cu.read(500000 /* 500ms timeout */);
    if (data < 0) {
        out << "Timeout reading from Control Unit\r\n";
        cu.reset();
    } else if (CarreraCommandPacket cmd = data) {
        out << uint16_t(data) << " CMD:"
            << " COMMAND=" << cmd.command()
            << " VALUE=" << cmd.value()
            << " ADDRESS=" << cmd.address()
            << "\r\n";
    } else if (CarreraControllerPacket ctr = data) {
        out << uint16_t(data) << " CTR:"
            << " ADDRESS=" << ctr.address()
            << " LANECHANGE=" << ctr.laneChange()
            << " THROTTLE=" << ctr.throttle()
            << " FUEL=" << ctr.fuelMode()
            << "\r\n";
    } else if (CarreraAutonomousPacket aut = data) {
        out << uint16_t(data) << " AUT:"
            << " STOPPED=" << aut.stopped()
            << " IN=" << aut.paceCarIn()
            << " ACTIVE=" << aut.paceCarActive()
            << " FUEL=" << aut.fuelMode()
            << "\r\n";
    } else if (CarreraActivityPacket act = data) {
        out << uint16_t(data) << " ACT:"
            << " MASK=" << act.mask()
            << " ANY=" << act.any()
            << "\r\n";
    } else if (CarreraAcknowledgePacket ack = data) {
        out << uint16_t(data) << " ACK:"
            << " MASK=" << ack.mask()
            << "\r\n";
    } else {
        out << uint16_t(data) << " [???]\r\n";
        cu.reset();  // probably lost sync
    }
}

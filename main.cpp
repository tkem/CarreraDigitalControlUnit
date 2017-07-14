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

#ifdef __MBED__
#include "mbed.h"
#endif

class ControlUnit {
#ifdef __MBED__
    InterruptIn _in;
    Timer _timer;
#else
    typedef int PinName;
    struct Timer {
        unsigned long t;
        void start() {
            t = micros();
        }
        void reset() {
            t = micros();
        }
        unsigned read_us() {
            return micros() - t;
        }
    };
    int _in;
    Timer _timer;
    static ControlUnit* _instance;
#endif
    unsigned _word;
    volatile unsigned _newword;
    volatile bool _changed;

public:
    ControlUnit(PinName in): _in(in) {
        _word = 0;
        _changed = false;
        _timer.start();
#ifdef __MBED__
        _in.rise(callback(this, &ControlUnit::rise));
        _in.fall(callback(this, &ControlUnit::fall));
#else
        _instance = this;
        pinMode(in, INPUT);
        attachInterrupt(digitalPinToInterrupt(in), &isr, CHANGE);
#endif
    }

    unsigned read() {
        while (!_changed)
            ;
        _changed = false;
        return _newword;
    }

private:
    void rise() {
        decode(true);
    }

    void fall() {
        decode(false);
    }

    void decode(bool rising) {
        int t = _timer.read_us();
        if (t > 75 && t < 125) {
            _word <<= 1;
            _word |= rising ? 0 : 1;
            _timer.reset();
        } else if (t > 6000) {
            _newword = _word;
            _word = 1;
            _changed = true;
            _timer.reset();
        }
    }

#ifdef ARDUINO
    static void isr() {
      if (digitalRead(_instance->_in) == LOW) {
        _instance->fall();
      } else {
        _instance->rise();
      }
    }
#endif
};

#ifdef ARDUINO
ControlUnit* ControlUnit::_instance = 0;
#endif

int lsb3(int v) {
    return ((v & 0x1) << 2) | (v & 0x2) | ((v & 0x4) >> 2);
}

int lsb4(int v) {
    return ((v & 0x1) << 3) | (v & 0x2) << 1 | (v & 0x4) >> 1 | ((v & 0x8) >> 3);
}

int lsb5(int v) {
    return ((v & 0x1) << 4) | (v & 0x2) << 2 | (v & 0x4) | (v & 0x8) >> 2 | ((v & 0x10) >> 4);
}

#ifdef __MBED__
Serial pc(USBTX, USBRX);
#endif

void print_prog(unsigned word) {
#ifdef __MBED__
    pc.printf("PROG: %04x B=%d W=%d R=%d\r\n",
              word, lsb5(word >> 3), lsb4(word >> 8), lsb3(word));
#else
    Serial.print("PROG: ");
    Serial.print(word, HEX);
    Serial.print(" B=");
    Serial.print(lsb5(word >> 3));
    Serial.print(" W=");
    Serial.print(lsb4(word >> 8));
    Serial.print(" R=");
    Serial.print(lsb3(word));
    Serial.println();
#endif
}

void print_ctrl(unsigned word) {
#ifdef __MBED__
    pc.printf("CTRL: %04x R=%d, SW=%d, G=%d, TA=%d\r\n",
              word, (word >> 6) & 0x7, !!(word & 0x20), (word >> 1) & 0xf, word & 1);
#else
    Serial.print("CTRL: ");
    Serial.print(word, HEX);
    Serial.print(" R=");
    Serial.print((word >> 6) & 0x7);
    Serial.print(" SW=");
    Serial.print(!!(word & 0x20));
    Serial.print(" G=");
    Serial.print((word >> 1) & 0xf);
    Serial.print(" TA=");
    Serial.print(word & 1);
    Serial.println();
#endif
}

void print_pace(unsigned word) {
#ifdef __MBED__
    pc.printf("PACE: %04x KFR=%d, TK=%d, FR=%d, NH=%d, PC=%d, TA=%d\r\n",
              word, !!(word & 0x20), !!(word & 0x10), !!(word & 0x8), !!(word & 0x4),
              !!(word & 0x2), !!(word & 0x1));
#else
    Serial.print("PACE: ");
    Serial.print(word, HEX);
    Serial.print(" KFR=");
    Serial.print(!!(word & 0x20));
    Serial.print(" TK=");
    Serial.print(!!(word & 0x10));
    Serial.print(" FR=");
    Serial.print(!!(word & 0x8));
    Serial.print(" NH=");
    Serial.print(!!(word & 0x4));
    Serial.print(" PC=");
    Serial.print(!!(word & 0x2));
    Serial.print(" TA=");
    Serial.print(!!(word & 0x1));
    Serial.println();
#endif
}

#ifdef __MBED__
ControlUnit cu(D2);
#else
ControlUnit cu(2);
#endif

//#define PROG_ONLY

void setup()
{
#ifdef __MBED__
    pc.printf("Starting MBED CU protocol reader\r\n");
#else
    Serial.begin(115200);
    Serial.println("Starting Arduino CU protocol reader");
#endif
}

void loop()
{
#ifndef PROG_ONLY
    static unsigned words[10];
    static unsigned count = 0;
#endif
    unsigned word = cu.read();
#ifdef PROG_ONLY
    if (word & 0x1000) {
        print_prog(word);
    }
#else
    if (word & 0x1000) {
        count = 0;
    }
    words[count++] = word;
    if (count == 10) {
        print_prog(words[0]);
        print_pace(words[1]);
        print_ctrl(words[3]);
        print_ctrl(words[4]);
        print_ctrl(words[5]);
        print_ctrl(words[6]);
        print_ctrl(words[7]);
        print_ctrl(words[9]);
        count = 0;
    }
#endif
}

#ifdef __MBED__
int main()
{
    setup();
    for (;;) {
        loop();
    }
    return 0;
}
#endif

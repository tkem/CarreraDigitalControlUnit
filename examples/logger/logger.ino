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

#include <cardiff.h>

void print_prog(unsigned word) {
    Serial.print("PROG: ");
    Serial.print(word, HEX);
    Serial.print(" B=");
    Serial.print(lsb5(word >> 3));
    Serial.print(" W=");
    Serial.print(lsb4(word >> 8));
    Serial.print(" R=");
    Serial.print(lsb3(word));
    Serial.println();
}

void print_ctrl(unsigned word) {
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
}

void print_pace(unsigned word) {
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
}

ControlUnit cu(2);

//#define PROG_ONLY

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting Arduino CU protocol reader");
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

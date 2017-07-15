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

static inline int lsb3(int v) {
    return ((v & 0x1) << 2) | (v & 0x2) | ((v & 0x4) >> 2);
}

static inline int lsb4(int v) {
    return ((v & 0x1) << 3) | (v & 0x2) << 1 | (v & 0x4) >> 1 | ((v & 0x8) >> 3);
}

static inline int lsb5(int v) {
    return ((v & 0x1) << 4) | (v & 0x2) << 2 | (v & 0x4) | (v & 0x8) >> 2 | ((v & 0x10) >> 4);
}

void print_prog(unsigned word)
{
    Serial.print("PROG: ");
    Serial.print(word, HEX);
    Serial.print("\t B=");
    Serial.print(lsb5(word >> 3));
    Serial.print(" W=");
    Serial.print(lsb4(word >> 8));
    Serial.print(" R=");
    Serial.print(lsb3(word));
    Serial.println();
}

void print_ctrl(unsigned word)
{
    Serial.print("CTRL: ");
    Serial.print(word, HEX);
    Serial.print("\t R=");
    Serial.print((word >> 6) & 0x7);
    Serial.print(" SW=");
    Serial.print(!!(word & 0x20));
    Serial.print(" G=");
    Serial.print((word >> 1) & 0xf);
    Serial.print(" TA=");
    Serial.print(word & 1);
    Serial.println();
}

void print_pace(unsigned word)
{
    Serial.print("PACE: ");
    Serial.print(word, HEX);
    Serial.print("\t KFR=");
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

void print_ack(unsigned word)
{
    Serial.print("ACK:  ");
    Serial.print(word, HEX);
    Serial.print("\t S=");
    Serial.print(word & 0xff, BIN);
    Serial.println();
}

void print_act(unsigned word)
{
    Serial.print("ACT:  ");
    Serial.print(word, HEX);
    Serial.print("\t R=");
    Serial.print((word >> 1) & 0x3f, BIN);
    Serial.print(" IE=");
    Serial.print(!!(word & 0x1));
    Serial.println();
}

void print_inv(unsigned word)
{
    Serial.print("???:  ");
    Serial.print(word, HEX);
    Serial.println();
}

void print_word(unsigned word)
{
    if (word & ~0x1fff) {
        print_inv(word);
    } else if (word & 0x1000) {
        print_prog(word);
    } else if (word & 0xc00) {
        print_inv(word);
    } else if ((word & 0x3c0) == 0x3c0) {
        print_pace(word);
    } else if (word & 0x200) {
        print_ctrl(word);
    } else if (word & 0x100) {
        print_ack(word);
    } else if (word & 0x80) {
        print_act(word);
    } else {
        print_inv(word);
    }
}

ControlUnit cu(2);

#define WORD_MASK ~0
//#define WORD_MASK 0x1000 // only PROG words

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting Arduino CU protocol reader");
}

void loop()
{
    unsigned word = cu.read();
    if (word & WORD_MASK) {
        print_word(word);
    }
}

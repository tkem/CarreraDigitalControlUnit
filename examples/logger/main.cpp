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

#include <cardiff.h>

Serial pc(USBTX, USBRX);

void print_prog(unsigned word) {
    pc.printf("PROG: %04x B=%d W=%d R=%d\r\n",
              word, lsb5(word >> 3), lsb4(word >> 8), lsb3(word));
}

void print_ctrl(unsigned word) {
    pc.printf("CTRL: %04x R=%d, SW=%d, G=%d, TA=%d\r\n",
              word, (word >> 6) & 0x7, !!(word & 0x20), (word >> 1) & 0xf, word & 1);
}

void print_pace(unsigned word) {
    pc.printf("PACE: %04x KFR=%d, TK=%d, FR=%d, NH=%d, PC=%d, TA=%d\r\n",
              word, !!(word & 0x20), !!(word & 0x10), !!(word & 0x8), !!(word & 0x4),
              !!(word & 0x2), !!(word & 0x1));
}

//#define PROG_ONLY

int main()
{
    ControlUnit cu(D2);
#ifndef PROG_ONLY
    unsigned words[10];
    unsigned count = 0;
#endif
    pc.printf("Starting MBED CU protocol reader\r\n");

    for (;;) {
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
    return 0;
}

#endif

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

// FIXME: exclude from Arduino, make proper mbed library example
#ifdef __MBED__

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

void print_word(unsigned word)
{
    if (word & ~0x1fff) {
        printf("???:  %04x\n", word);
    } else if (word & 0x1000) {
        printf("PROG: %04x B=%d W=%d R=%d\n",
               word, lsb5(word >> 3), lsb4(word >> 8), lsb3(word));
    } else if (word & 0xc00) {
        printf("???:  %04x\n", word);
    } else if ((word & 0x3c0) == 0x3c0) {
        printf("PACE: %04x KFR=%d, TK=%d, FR=%d, NH=%d, PC=%d, TA=%d\r\n",
               word, !!(word & 0x20), !!(word & 0x10), !!(word & 0x8), !!(word & 0x4),
               !!(word & 0x2), !!(word & 0x1));
    } else if (word & 0x200) {
        printf("CTRL: %04x R=%d, SW=%d, G=%d, TA=%d\n",
               word, (word >> 6) & 0x7, !!(word & 0x20), (word >> 1) & 0xf, word & 1);
    } else if (word & 0x100) {
        printf("ACK:  %04x S=%02x\n",
               word, word & 0xff);
    } else if (word & 0x80) {
        printf("ACT:  %04x R=%02x IE=%d\n",
               word, (word >> 1) & 0x3f, !!(word & 1));
    } else {
        printf("???:  %04x\n", word);
    }
}

#define WORD_MASK ~0
//#define WORD_MASK 0x1000 // only PROG words

int main()
{
    ControlUnit cu(D2);
    printf("Starting MBED CU protocol reader\r\n");

    for (;;) {
        unsigned word = cu.read();
        if (word & WORD_MASK) {
            print_word(word);
        }
    }

    return 0;
}

#endif

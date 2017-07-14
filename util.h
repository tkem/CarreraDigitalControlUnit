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
#ifndef CARDIFF_UTIL_H
#define CARDIFF_UTIL_H

int lsb3(int v) {
    return ((v & 0x1) << 2) | (v & 0x2) | ((v & 0x4) >> 2);
}

int lsb4(int v) {
    return ((v & 0x1) << 3) | (v & 0x2) << 1 | (v & 0x4) >> 1 | ((v & 0x8) >> 3);
}

int lsb5(int v) {
    return ((v & 0x1) << 4) | (v & 0x2) << 2 | (v & 0x4) | (v & 0x8) >> 2 | ((v & 0x10) >> 4);
}

#endif

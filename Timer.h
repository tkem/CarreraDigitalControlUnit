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
#ifndef CARDIFF_TIMER_H
#define CARDIFF_TIMER_H

#ifdef __MBED__
#include "mbed.h"
#else
#include "Arduino.h"
#endif

namespace cardiff {
#ifdef __MBED__
    typedef ::Timer Timer;
#else
    class Timer {
        unsigned long _micros;
    public:
        void start() {
            _micros = micros();
        }
        void reset() {
            _micros = micros();
        }
        unsigned read_us() {
            return micros() - _micros;
        }
    };
#endif
}

#endif

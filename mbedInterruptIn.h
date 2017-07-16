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
#ifndef CARDIFF_MBED_INTERRUPT_IN_H
#define CARDIFF_MBED_INTERRUPT_IN_H

#ifdef __MBED__

#include "mbed.h"

#else

#include "mbedCallback.h"
#include "mbedPinMode.h"
#include "mbedPinName.h"

namespace cardiff {
    class InterruptIn {
        PinName _pin;
        Callback<void()> _rise;
        Callback<void()> _fall;

    public:
        InterruptIn(PinName pin);
        ~InterruptIn();

        int read();

        operator int() {
            return read();
        }

        void rise(Callback<void()> func) {
            _rise = func;
        }

        void fall(Callback<void()> func) {
            _fall = func;
        }

        void mode(PinMode pull);

    public:
        void isr();

    private:
        InterruptIn(const InterruptIn&);
        InterruptIn& operator=(const InterruptIn&);
    };
}

#endif

#endif

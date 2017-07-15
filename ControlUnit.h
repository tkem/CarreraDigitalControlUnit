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
#ifndef CARDIFF_CONTROL_UNIT_H
#define CARDIFF_CONTROL_UNIT_H

#include "EdgeInterrupt.h"
#include "Timer.h"

namespace cardiff {
    class ControlUnit {
        volatile unsigned _data;
        volatile unsigned _counter;

        Timer _timer;
        unsigned _buffer;
        bool _sync;

        EdgeInterrupt<ControlUnit> _irq;

    public:
        ControlUnit(PinName pin);

        unsigned read();

    private:
        void emit(unsigned data);
        void rise();
        void fall();

    private:
        ControlUnit(const ControlUnit&);
        ControlUnit& operator=(const ControlUnit&);
    };
}

#endif

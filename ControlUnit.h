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
        EdgeInterrupt<ControlUnit> _irq;
        Timer _timer;
        unsigned _word;
        volatile unsigned _newword;
        volatile bool _changed;

    public:
        ControlUnit(int pin);

        unsigned read();

    private:
        void rise() {
            decode(true);
        }

        void fall() {
            decode(false);
        }

        void decode(bool rising);
    };
}

#endif

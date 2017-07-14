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

#include "ControlUnit.h"

using namespace cardiff;

ControlUnit::ControlUnit(int pin)
    : _irq(pin, this, &ControlUnit::rise, &ControlUnit::fall)
{
    _word = 0;
    _changed = false;
    _timer.start();
}

unsigned ControlUnit::read()
{
    while (!_changed)
        ;
    _changed = false;
    return _newword;
}

void ControlUnit::decode(bool rising)
{
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

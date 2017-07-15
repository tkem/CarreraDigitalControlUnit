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

ControlUnit::ControlUnit(PinName pin)
    : _counter(0), _buffer(0), _sync(0), _irq(pin, this)
{
    _timer.start();
    _irq.rise(&ControlUnit::rise);
    _irq.fall(&ControlUnit::fall);
}

unsigned ControlUnit::read()
{
    unsigned counter = _counter;
    while (counter == _counter)
        ;
    return _data;
}

void ControlUnit::emit(unsigned data)
{
    _data = data;
    ++_counter;
}

void ControlUnit::fall()
{
    // TODO: emit without waiting for 6ms timeout (e.g. keep frame index after sync)
    // TODO: handle data packets sent to CU, i.e. at t ~ 2300
    int t = _timer.read_us();
    if (t > 75 && t < 125) {
        _buffer <<= 1;
        _buffer |= 1;
        _timer.reset();
    } else if (t > 6000) {
        if ((_buffer & ~0xfff) == 0x1000) {
            _sync = true;
        }
        if (_sync) {
            emit(_buffer);
        }
        _buffer = 1;
        _timer.reset();
    }
}

void ControlUnit::rise()
{
    int t = _timer.read_us();
    if (t > 75 && t < 125) {
        _buffer <<= 1;
        _timer.reset();
    }
}

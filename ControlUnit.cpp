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

static const unsigned word_mask[10] = {
    1 << 12,
    1 << 9,
    1 << 8,  // or 7 if no command sent to CU
    1 << 9,
    1 << 9,
    1 << 9,
    1 << 9,
    1 << 9,
    1 << 8,  // or 7 if no command sent to CU
    1 << 9
};

ControlUnit::ControlUnit(PinName pin)
    : _buffer(0), _index(0), _irq(pin)
{
    _timer.start();
    _irq.rise(callback(this, &ControlUnit::rise));
    _irq.fall(callback(this, &ControlUnit::fall));
}

unsigned ControlUnit::read()
{
    bool clock = _clock;
    while (clock == _clock)
        ;
    // *not* atomic on 8-bit microcontrollers, but probably good enough for now
    return _data;
}

void ControlUnit::emit(unsigned data)
{
    _data = data;
    _clock = !_clock;
}

void ControlUnit::fall()
{
    // TODO: handle data packets sent to CU, i.e. at t ~ 2300
    int t = _timer.read_us();
    if (t > 75 && t < 125) {
        _buffer <<= 1;
        _buffer |= 1;
        if (_buffer & word_mask[_index]) {
            emit(_buffer);
            _buffer = 0;
            _index = (_index + 1) % 10;
        }
        _timer.reset();
    } else if (t > 6000) {
        if (_buffer) {
            if (_index == 2 || _index == 8) {
                emit(_buffer);
                ++_index;
            } else {
                _index = 0;  // lost sync
            }
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
        if (_buffer & word_mask[_index]) {
            emit(_buffer);
            _buffer = 0;
            _index = (_index + 1) % 10;
        }
        _timer.reset();
    }
}

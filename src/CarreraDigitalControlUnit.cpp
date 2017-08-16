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
#include "CarreraDigitalControlUnit.h"

#include "hal/us_ticker_api.h"
#include "platform/mbed_critical.h"

static const uint16_t emit_mask[10] = {
    1 << 12,
    1 << 9,
    1 << 8,  // or 7 if no command was sent to CU
    1 << 9,
    1 << 9,
    1 << 9,
    1 << 9,
    1 << 9,
    1 << 8,  // or 7 if no command was sent to CU
    1 << 9
};

static inline int atomic_read_int(const volatile int* p) {
#ifdef ARDUINO
    // 16-bit reads are *not* atomic on 8-bit platforms
    int v;
    do {
        v = *p;
    } while (v != *p);
    return v;
#else
    return *p;
#endif
}

void CarreraDigitalControlUnit::start()
{
    core_util_critical_section_enter();
    if (!_running) {
        reset();
        _irq.rise(callback(this, &CarreraDigitalControlUnit::rise));
        _irq.fall(callback(this, &CarreraDigitalControlUnit::fall));
        _running = true;
    }
    core_util_critical_section_exit();
}

void CarreraDigitalControlUnit::stop()
{
    core_util_critical_section_enter();
    if (_running) {
        _irq.rise(0);
        _irq.fall(0);
        _running = false;
    }
    core_util_critical_section_exit();
}

void CarreraDigitalControlUnit::reset()
{
    core_util_critical_section_enter();
    _time = 0;
    _buffer = 0;
    _index = 0;
    core_util_critical_section_exit();
}

int CarreraDigitalControlUnit::read()
{
    // wait for new data to arrive
    bool clk = _clk;
    while (clk == _clk)
        ;
    return atomic_read_int(&_data);
}

void CarreraDigitalControlUnit::emit(int data)
{
    _data = data;
    _clk = !_clk;
    if (++_index == 10) {
        _index = 0;
    }
}

void CarreraDigitalControlUnit::fall()
{
    // TODO: handle data packets sent to CU at d =~ 2300?

    // mbed::Timer is nice, but in an ISR we need something simple and
    // efficient, even if it may overflow...
    uint32_t t = us_ticker_read();
    uint32_t d = t - _time;
    if (_buffer && d > 75 && d < 125) {
        _buffer <<= 1;
        _buffer |= 1;
        if (_buffer & emit_mask[_index]) {
            emit(_buffer);
            _buffer = 0;
        }
        _time = t;
    } else if (d > 6000) {
        if (_buffer) {
            if (_index == 2 || _index == 8) {
                emit(_buffer);
            } else {
                _index = 0;  // lost sync
            }
        }
        _buffer = 1;
        _time = t;
    }
}

void CarreraDigitalControlUnit::rise()
{
    uint32_t t = us_ticker_read();
    uint32_t d = t - _time;
    if (d > 75 && d < 125) {
        _buffer <<= 1;
        if (_buffer & emit_mask[_index]) {
            emit(_buffer);
            _buffer = 0;
        }
        _time = t;
    }
}

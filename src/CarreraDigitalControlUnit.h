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
#ifndef CARRERA_DIGITAL_CONTROL_UNIT_H
#define CARRERA_DIGITAL_CONTROL_UNIT_H

#include <mbed.h>

class CarreraDigitalControlUnit {
    InterruptIn _irq;
    bool _running;

    uint32_t _time;
    uint16_t _buffer;
    uint8_t _index;

    volatile bool _clk;
    volatile int _data;

public:
    CarreraDigitalControlUnit(PinName pin) : _irq(pin), _running(false) {}

    void start();

    void stop();

    void reset();

    int read();

private:
    void emit(int data);
    void rise();
    void fall();
};

#endif

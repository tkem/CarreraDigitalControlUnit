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
#include "mbedInterruptIn.h"

#ifdef ARDUINO

#include "Arduino.h"

#define IS_INTERRUPT(p) (digitalPinToInterrupt(p) != NOT_AN_INTERRUPT)

using namespace cardiff;

namespace {
    struct irq_entry {
        void (*isr)();
        InterruptIn* obj;
    };

    extern irq_entry irqs[];

    template<int N>
    void isr() {
        irqs[N].obj->isr();
    }

    // FIXME: Due, Zero, and other boards where interrupt number = pin number
    irq_entry irqs[] = {
        // all Arduino boards
        { &isr<0>, 0 },
        { &isr<1>, 0 },
#if IS_INTERRUPT(18) || IS_INTERRUPT(0)
        // Mega2560, 32u4 based (e.g Leonardo, Micro)
        { &isr<2>, 0 },
        { &isr<3>, 0 },
        { &isr<4>, 0 },
#if IS_INTERRUPT(18)
        // Mega2560
        { &isr<5>, 0 },
#endif
#endif
    };
}

InterruptIn::InterruptIn(PinName pin) : _pin(pin)
{
    int irq = digitalPinToInterrupt(pin);
    irqs[irq].obj = this;
    attachInterrupt(irq, irqs[irq].isr, CHANGE);
    pinMode(pin, INPUT);
}

InterruptIn::~InterruptIn()
{
    int irq = digitalPinToInterrupt(_pin);
    detachInterrupt(irq);
    irqs[irq].obj = 0;
}

int InterruptIn::read()
{
    return digitalRead(_pin) == HIGH ? 1 : 0;
}

void InterruptIn::mode(PinMode pull)
{
    pinMode(_pin, pull);
}

void InterruptIn::isr()
{
    if (digitalRead(_pin) == HIGH) {
        _rise();
    } else {
        _fall();
    }
}

#endif

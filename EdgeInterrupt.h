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
#ifndef CARDIFF_EDGE_INTERRUPT_H
#define CARDIFF_EDGE_INTERRUPT_H

#ifdef __MBED__
#include "mbed.h"
#else
#include "Arduino.h"
#endif

namespace cardiff {
    template<class T>
    class EdgeInterrupt {
#ifdef __MBED__
        InterruptIn _pin;
#else
        typedef int PinName;
        PinName _pin;
        T* _obj;
        void (T::*_rise)();
        void (T::*_fall)();
        static EdgeInterrupt* _instance;
#endif
    public:
        EdgeInterrupt(int pin, T* obj, void(T::*rise)(), void(T::*fall)()): _pin((PinName)pin) {
#ifdef __MBED__
            _pin.rise(callback(obj, rise));
            _pin.fall(callback(obj, fall));
#else
            _instance = this;
            _obj = obj;
            _rise = rise;
            _fall = fall;
            pinMode(pin, INPUT);
            attachInterrupt(digitalPinToInterrupt(pin), &isr, CHANGE);
#endif
        }

        ~EdgeInterrupt() {
#ifdef ARDUINO
            detachInterrupt(digitalPinToInterrupt(_pin));
#endif
        }

    private:
#ifdef ARDUINO
        static void isr() {
            if (digitalRead(_instance->_pin) == LOW) {
                (_instance->_obj->*_instance->_fall)();
            } else {
                (_instance->_obj->*_instance->_rise)();
            }
        }
#endif

    };

#ifdef ARDUINO
    // FIXME: single instance per callback class!
    template<class T>
    EdgeInterrupt<T>* EdgeInterrupt<T>::_instance = 0;
#endif
}

#endif

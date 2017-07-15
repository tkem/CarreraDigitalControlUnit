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

#if defined(__MBED__)
#include "mbed.h"
#elif defined(ARDUINO)
#include "Arduino.h"
#else
#error "Not an mbed or Arduino platform"
#endif

namespace cardiff {
#ifdef ARDUINO
    typedef int PinName;
#endif

    template<class T>
    class EdgeInterrupt {
        T* _obj;
#ifdef __MBED__
        InterruptIn _pin;
#else
        PinName _pin;
        void (T::*_rise)();
        void (T::*_fall)();
        static EdgeInterrupt* _instance;
#endif
    public:
    EdgeInterrupt(PinName pin, T* obj): _obj(obj), _pin(pin) {
#ifdef ARDUINO
            _instance = this;  // FIXME: single instance per callback object!
            _rise = 0;
            _fall = 0;
            pinMode(pin, INPUT);
            attachInterrupt(digitalPinToInterrupt(pin), &isr, CHANGE);
#endif
        }

        ~EdgeInterrupt() {
#ifdef ARDUINO
            detachInterrupt(digitalPinToInterrupt(_pin));
#endif
        }

        void rise(void(T::*method)()) {
#ifdef __MBED__
            _pin.rise(callback(_obj, method));
#else
            _rise = method;
#endif
        }

        void fall(void(T::*method)()) {
#ifdef __MBED__
            _pin.fall(callback(_obj, method));
#else
            _fall = method;
#endif
        }

    private:
#ifdef ARDUINO
        static void isr() {
            if (digitalRead(_instance->_pin) == LOW) {
                if (_instance->_fall) {
                    (_instance->_obj->*_instance->_fall)();
                }
            } else {
                if (_instance->_rise) {
                    (_instance->_obj->*_instance->_rise)();
                }
            }
        }
#endif
    };

#ifdef ARDUINO
    template<class T>
    EdgeInterrupt<T>* EdgeInterrupt<T>::_instance = 0;
#endif
}

#endif

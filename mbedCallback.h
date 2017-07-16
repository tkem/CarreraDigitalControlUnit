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
#ifndef CARDIFF_MBED_CALLBACK_H
#define CARDIFF_MBED_CALLBACK_H

#if defined(__MBED__)
#include "mbed.h"
#else

namespace cardiff {
    template<typename F> class Callback;

    template<typename R>
    class Callback<R()> {
        class cls;  // placeholder
    public:
        Callback() : thunk(&Callback::nop_thunk) {}

        Callback(R (*func)()) : thunk(&Callback::func_thunk) {
            cb.func = func;
        }

        template<typename T>
        Callback(T* obj, R (T::*method)()) : thunk(&Callback::method_thunk<T>) {
            cb.method.obj = obj;
            cb.method.method = reinterpret_cast<R (cls::*)()>(method);
        }

        R operator()() {
            return (this->*thunk)();
        }

    private:
        union {
            R (*func)();
            struct {
                void* obj;
                R (cls::*method)();
            } method;
        } cb;

        R (Callback::*thunk)();

    private:
        R nop_thunk() {
        }

        R func_thunk() {
            return (*cb.func)();
        }

        template<typename T>
        R method_thunk() {
            T* obj = static_cast<T*>(cb.method.obj);
            R (T::*method)() = reinterpret_cast<R (T::*)()>(cb.method.method);
            return (obj->*method)();
        }
    };

    template<typename R>
    inline Callback<R()> callback(R (*func)()) {
        return Callback<R()>(func);
    }

    template<typename R, typename T>
    inline Callback<R()> callback(T* obj, R (T::*method)()) {
        return Callback<R()>(obj, method);
    }
}

#endif

#endif

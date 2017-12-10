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
#include "TM1637Display.h"

#include <stdlib.h>

#define TM1637_DATA_CMD 0x40
#define TM1637_ADDR_CMD 0xC0
#define TM1637_CTRL_CMD 0x80

/*
      0
     ---
  5 |   | 1
     -6-
  4 |   | 2
     ---
      3 * 7
*/
static const uint8_t digits[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f
};

TM1637Display::TM1637Display(PinName dio, PinName clk, int brightness) :
    _dio(dio), _clk(clk), _ctrl(0x08 | (brightness & 0x7))
{
    _dio.output();
    _dio.write(1);
    _clk.write(1);
}

void TM1637Display::clear()
{
    const uint8_t data[4] = { 0, 0, 0, 0 };
    write(data, sizeof data);
}

void TM1637Display::show(int n)
{
    uint8_t data[4];
    bool neg = n < 0;
    n = abs(n);
    for (size_t i = 0; i != sizeof data; ++i) {
        div_t d = div(n, 10);
        if (d.rem != 0 || d.quot != 0 || i == 0) {
            data[sizeof(data) - i - 1] = digits[d.rem];
        } else if (neg) {
            data[sizeof(data) - i - 1] = 0x40;
            neg = false;
        } else {
            data[sizeof(data) - i - 1] = 0;
        }
        n = d.quot;
    }
    write(data, sizeof data);
}

void TM1637Display::delay()
{
    wait_us(2);  // TM1637 max frequency is 500kHz, tWait=1us
}

void TM1637Display::start()
{

    _dio.write(0);
    delay();
    _clk.write(0);
    delay();
}

void TM1637Display::stop()
{
    _dio.write(0);
    delay();
    _clk.write(1);
    delay();
    _dio.write(1);
    delay();
}

int TM1637Display::write(uint8_t data)
{
    for (int i = 0; i != 8; ++i) {
        _dio.write((data & (1 << i)) != 0);
        delay();
        _clk.write(1);
        delay();
        _clk.write(0);
        delay();
    }

    _dio.write(1);
    _dio.input();
    delay();
    _clk.write(1);
    delay();
    int ack = _dio.read();
    _clk.write(0);
    delay();
    _dio.output();
    delay();
    _dio.write(1);
    return ack;
}

void TM1637Display::write(const uint8_t* data, size_t size)
{
    start();
    write(TM1637_DATA_CMD);
    stop();

    start();
    write(TM1637_ADDR_CMD);
    for (size_t i = 0; i != size; ++i) {
        write(data[i]);
    }
    stop();

    start();
    write(TM1637_CTRL_CMD | _ctrl);
    stop();
}

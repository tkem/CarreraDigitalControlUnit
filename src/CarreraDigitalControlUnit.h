/*
  Copyright 2017, 2021 Thomas Kemmer

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

#ifdef ARDUINO
#include <Arduino.h>
#endif

#ifdef __MBED__
#include <mbed.h>
#endif

/** A Carrera(R) Digital 124/132 Control Unit connection
 *
 * @note Synchronization level: Interrupt safe
 *
 * @ingroup drivers
 */
class CarreraDigitalControlUnit {
public:
#ifdef ARDUINO
    /** Create a connection to a ControlUnit using the specified pin
     *
     * @param pin A digital input connected to the Control Unit
     * @param inverted Whether the input is logically inverted
     *
     * @note pin must support interrupts
     */
    CarreraDigitalControlUnit(int pin, bool inverted = false);

    /** Create a connection to a ControlUnit using the specified pin
     *
     * @param pin A digital input connected to the Control Unit
     * @param mode The pin mode to set for the input
     * @param inverted Whether the input is logically inverted
     *
     * @note pin must support interrupts
     */
    CarreraDigitalControlUnit(int pin, int mode, bool inverted);
#endif

#ifdef __MBED__
    /** Create a connection to a ControlUnit using the specified pin
     *
     * @param pin A digital input connected to the Control Unit
     * @param inverted Whether the input is logically inverted
     *
     * @note pin must support interrupts
     */
    CarreraDigitalControlUnit(PinName pin, bool inverted = false);

    /** Create a connection to a ControlUnit using the specified pin
     *
     * @param pin A digital input connected to the Control Unit
     * @param mode The pin mode to set for the input
     * @param inverted Whether the input is logically inverted
     *
     * @note pin must support interrupts
     */
    CarreraDigitalControlUnit(PinName pin, PinMode mode, bool inverted);
#endif

    /** Start receiving data from the Control Unit
     */
    void start();

    /** Stop receiving data from the Control Unit
     */
    void stop();

    /** Reset the Control Unit connection, e.g. in case of error
     */
    void reset();

    /** Read a data word from the Control Unit
     */
    int read();

    /** Read a data word from the Control Unit with timeout
     *
     * @param timeout_us The timeout in microseconds
     *
     * @returns the next data word, or -1 on timeout
     */
    int read(uint32_t timeout_us);

private:
    void emit();
    void fall();
    void rise();
    uint32_t time_us();
#ifdef __MBED__
    mbed::InterruptIn _irq;
    mbed::Timer _timer;
#else
    static void irq();
    int _pin;
#ifdef __AVR
    volatile uint8_t* _ireg;
    uint8_t _mask;
#endif
#endif
    volatile uint16_t _data;
    bool _inverted;
    bool _running;
    unsigned _time;
    unsigned _buffer;
    unsigned _index;
};

/** A Carrera(R) Digital 124/132 controller data packet
 */
class CarreraControllerPacket {
public:

    /** Initialize with a value read from the Control Unit
     */
    CarreraControllerPacket(int data = 0) : _data(data) {}

    /** The address (number) of the controller (0..5)
     */
    uint8_t address() const { return (_data >> 6) & 0x07; }

    /** Whether the lane change button was pressed
     */
    bool laneChange() const { return (_data & 0x20) == 0; }

    /** The throttle value (0..15)
     */
    uint8_t throttle() const { return (_data >> 1) & 0x0f; }

    /** Whether fuel mode is enabled
     */
    bool fuelMode() const { return (_data & 0x01) != 0; }

    /** Whether this is a valid controller data packet
     */
    explicit operator bool() const { return is(_data); }

private:
    static bool is(int data);  // TODO: make public? name?

private:
    uint16_t _data;
};

/** A Carrera(R) Digital 124/132 autonomous and pace car data packet
 */
class CarreraAutonomousPacket {
public:

    /** Initialize with a value read from the Control Unit
     */
    CarreraAutonomousPacket(int data = 0) : _data(data) {}

    /** Whether autonomous and pace cars should stop
     */
    bool stopped() const { return (_data & 0x20) != 0; }

    /** Whether pace car should return to the pit
     */
    bool paceCarIn() const { return (_data & 0x04) == 0; }

    /** Whether the pace car is active
     */
    bool paceCarActive() const { return (_data & 0x02) != 0; }

    /** Whether fuel mode is enabled
     */
    bool fuelMode() const { return (_data & 0x01) != 0; }

    /** Whether this is a valid pace/autonomous car data packet
     */
    explicit operator bool() const { return is(_data); }

private:
    static bool is(int data);  // TODO: make public? name?

private:
    uint16_t _data;
};

/** A Carrera(R) Digital 124/132 activity data packet
 */
class CarreraActivityPacket {
public:

    /** Initialize with a value read from the Control Unit
     */
    CarreraActivityPacket(int data = 0) : _data(data) {}

    /** Bit mask of active controllers (0..63)
     */
    uint8_t mask() const;

    /** Whether any controller has been pressed
     */
    bool any() const { return (_data & 0x01) != 0; }

    /** Whether this is a valid activity data packet
     */
    explicit operator bool() const { return is(_data); }

private:
    static bool is(int data);  // TODO: make public? name?

private:
    uint16_t _data;
};

/** A Carrera(R) Digital 124/132 acknowledge data packet
 */
class CarreraAcknowledgePacket {
public:

    /** Initialize with a value read from the Control Unit
     */
    CarreraAcknowledgePacket(int data = 0) : _data(data) {}

    /** Bit mask of acknowledged time slots (0..255)
     */
    uint8_t mask() const;

    /** Whether this is a valid acknowledge data packet
     */
    explicit operator bool() const { return is(_data); }

private:
    static bool is(int data);  // TODO: make public? name?

private:
    uint16_t _data;
};

/** A Carrera(R) Digital 124/132 command data packet
 */
class CarreraCommandPacket {
public:

    /** Initialize with a value read from the Control Unit
     */
    CarreraCommandPacket(int data = 0) : _data(data) {}

    /** Command number (0..31)
     */
    uint8_t command() const {
        return ((_data & 0x80) >> 7) | ((_data & 0x40) >> 5) | ((_data & 0x20) >> 3) | ((_data & 0x10) >> 1) | ((_data & 0x08) << 1);
    }

    /** Command address (0..7)
     */
    uint8_t address() const {
        return ((_data & 1) << 2) | (_data & 2) | ((_data & 4) >> 2);
    }

    /** Value of the command (0..15)
     */
    uint8_t value() const {
        return ((_data & 0x800) >> 11) | ((_data & 0x400) >> 9) | ((_data & 0x200) >> 7) | ((_data & 0x100) >> 5);
    }

    /** Whether this is a valid command data packet
     */
    explicit operator bool() const { return is(_data); }

private:
    static bool is(int data);  // TODO: make public? name?

private:
    uint16_t _data;
};

#endif

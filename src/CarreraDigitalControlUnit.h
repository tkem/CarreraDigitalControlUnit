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

/** A Carrera(R) Digital 124/132 Control Unit connection
 *
 * @note Synchronization level: Not protected
 *
 * @ingroup drivers
 */
class CarreraDigitalControlUnit {
    InterruptIn _irq;
    bool _running;

    uint32_t _time;
    uint16_t _buffer;
    uint8_t _index;

    volatile bool _avail;
    volatile int _data;

public:
    /** Create a connection to a ControlUnit using the specified pin
     *
     *  @param pin digital input connected to the Control Unit
     */
    CarreraDigitalControlUnit(PinName pin) : _irq(pin), _running(false), _avail(false) {}

    /** Start receiving data from the Control Unit
     */
    void start();

    /** Stop receiving data from the Control Unit
     */
    void stop();

    /** Reset the Control Unit connection, e.g. in case of error
     */
    void reset();

    /** Read a data word from a Control Unit
     */
    int read();

    /** Split a programming data word into its components
     *
     * @param data The data word to split
     *
     * @param res The programming data word's components
     *
     * @returns true on success, false on error
     */
    static bool split_programming_word(int data, uint8_t res[3]);

    /** Split a controller data word into its components
     *
     * @param data The data word to split
     *
     * @param res The controller data word's components
     *
     * @returns true on success, false on error
     */
    static bool split_controller_word(int data, uint8_t res[4]);

    /** Split a pace car data word into its components
     *
     * @param data The data word to split
     *
     * @param res The pace car data word's components
     *
     * @returns true on success, false on error
     */
    static bool split_pacecar_word(int data, uint8_t res[4]);

    /** Split an acknowledge data word into its components
     *
     * @param data The data word to split
     *
     * @param res The acknowledge data word's components
     *
     * @returns true on success, false on error
     */
    static bool split_acknowledge_word(int data, uint8_t res[1]);

    /** Split an active controller data word into its components
     *
     * @param data The data word to split
     *
     * @param res The active controller data word's components
     *
     * @returns true on success, false on error
     */
    static bool split_active_word(int data, uint8_t res[2]);

private:
    void emit();
    void rise();
    void fall();
};

#endif

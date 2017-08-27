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
     * @param pin digital input connected to the Control Unit
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

    /** Read a data word from the Control Unit
     */
    int read();

    /** Split a programming data word into its components
     *
     * If successfull, res will contain the following
     *
     * - res[0]  The programming command (0..31)
     * - res[1]  The programming value (0..15)
     * - res[2]  The programming address/controller (0..7)
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
     * If successfull, res will contain the following
     *
     * - res[0]  The controller's address (0..5)
     * - res[1]  The controller's speed (0..15)
     * - res[2]  Whether the lange change button is pressed (0..1)
     * - res[3]  Whether fuel mode is enabled (0..1)
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
     * If successfull, res will contain the following
     *
     * - res[0]  Whether pace and autonomous cars are free to move (0..1)
     * - res[1]  Whether the pace car should return to the box (0..1)
     * - res[2]  Whether the pace car is active (0..1)
     * - res[3]  Whether fuel mode is enabled (0..1)
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
     * If successfull, res will contain the following
     *
     * - res[0]  A bit mask of time slots in which a message was received
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
     * If successfull, res will contain the following
     *
     * - res[0]  A bit mask representing active controllers (0..63)
     * - res[1]  Whether any controller was active (0..1)
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

0.8.4 (2023-02-11)

- Add ESP32 support.


0.8.3 (2023-02-06)
------------------

- Add timeout to examples to trigger ESP8266 watchdog.


0.8.2 (2023-01-09)
------------------

- Fix license badge.


0.8.1 (2023-01-09)
------------------

- `badges/shields`: Change to GitHub workflow badge routes.

- Move CI to GitHub Actions.

- Documentation improvements.


0.8.0 (2021-07-17)
------------------

- Add protocol packet classes.

- Add `LapCounter` example.

- Breaking change: Remove static parse-to-byte-array methods from
`CarreraDigitalControlUnit`.  Use protocl packet classes instead.


0.7.1 (2021-06-22)
------------------

- Add NodeMCU (Espressif 8266) support.

- Various minor improvements.


0.7.0 (2021-06-16)
------------------

- Add support for `ARDUINO_ARCH_MBED`, e.g. Raspberry Pi Pico.

- No longer require `mbino` library on Arduino.


0.6.2 (2017-12-16)
------------------

- Fix anonymous type linkage warning.


0.6.1 (2017-12-16)
------------------

- Use compare-and-swap for marking data as read.

- Enhance `LapCounter` example.


0.6.0 (2017-10-08)
------------------

- Add constructor with pin mode parameter.

- Update examples for mbino v1.0.


0.5.0 (2017-10-01)
------------------

- Add `inverted` constructor argument.


0.4.0 (2017-09-29)
------------------

- Improve data member types and member function names.

- Add `CarreraDigitalControlUnit::attach()`.


0.3.0 (2017-09-09)
------------------

- Add `read()` with timeout.

- Emit active data word #2 more early.


0.2.0 (2017-08-27)
------------------

- Reorder `split_programming_word()` results.

- Improve documentation of `split` methods.

- Add `LapCounter` example.


0.1.0 (2017-08-26)
------------------

- Initial release.

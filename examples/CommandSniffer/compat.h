#ifndef COMPAT_H
#define COMPAT_H

#ifdef ARDUINO
#ifdef ARDUINO_ARCH_MBED
REDIRECT_STDOUT_TO(Serial);
#else
#define printf(...) { char buf[80]; sprintf(buf, __VA_ARGS__); Serial.print(buf); }
#endif
#endif

#if !defined(ARDUINO)
#define DIGITAL_PIN(n) (D ## n)
#elif defined(ARDUINO_ARCH_MBED)
#define DIGITAL_PIN(n) (p ## n)
#else
#define DIGITAL_PIN(n) (n)
#endif

#endif

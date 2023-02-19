#ifndef _PIN_CONFIG_H
#define _PIN_CONFIG_H

#ifdef ARDUINO
#   define PIN_S0  D7
#   define PIN_S1  D6
#   define PIN_S2  D5
#   define PIN_S3  D3
#   define PIN_S4  D2
#   define PIN_BTN D1
#else
#   define PIN_S0  0
#   define PIN_S1  1
#   define PIN_S2  2
#   define PIN_S3  3
#   define PIN_S4  4
#   define PIN_BTN 5
#endif // ARDUINO

#endif // _PIN_CONFIG_H
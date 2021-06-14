/* vim: set ft=c ts=2 sw=2 : */

/**
 * Library to handle the Minirobots turtle neopixels.
 *
 * https://gitlab.com/minirobots/turtle-robot-wifi
 *
 *
 * Author: Leo Vidarte <https://minirobots.com.ar>
 *
 * This is free software:
 * you can redistribute it and/or modify it
 * under the terms of the GPL version 3
 * as published by the Free Software Foundation.
 */

#ifndef Pixel_h
#define Pixel_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


#define NEOPIXELS_LEN 2
#define NEOPIXELS_PIN 3


class Pixel
{
  public:
    static void begin();

    static void set(uint8_t pixel, uint8_t r, uint8_t g, uint8_t b);
    static void setAll(uint8_t r, uint8_t g, uint8_t b);
    static void show(uint8_t);
    static void showAll();
    static void toggle(uint8_t pixel, uint8_t r, uint8_t g, uint8_t b);
    static void toggleAll(uint8_t r, uint8_t g, uint8_t b);
    static void toggleCross(uint8_t r, uint8_t g, uint8_t b);

    static uint8_t r;
    static uint8_t g;
    static uint8_t b;
};

#endif

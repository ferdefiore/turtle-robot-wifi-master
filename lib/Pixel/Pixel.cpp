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

#include "Pixel.h"


Adafruit_NeoPixel pixels(NEOPIXELS_LEN, NEOPIXELS_PIN, NEO_GRB + NEO_KHZ800);


uint8_t Pixel::r = 0;
uint8_t Pixel::g = 0;
uint8_t Pixel::b = 0;


/* static functions */

void Pixel::begin ()
{
  pixels.begin();
  pixels.setBrightness(50);
  pixels.show();
}

void Pixel::set (uint8_t pixel, uint8_t r, uint8_t g, uint8_t b)
{
  pixels.setPixelColor(pixel, pixels.Color(r, g, b));
  pixels.show();
}

void Pixel::setAll (uint8_t r, uint8_t g, uint8_t b)
{
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.setPixelColor(1, pixels.Color(r, g, b));
  pixels.show();
}

void Pixel::show (uint8_t pixel)
{
  Pixel::set(pixel, Pixel::r, Pixel::g, Pixel::b);
}

void Pixel::showAll ()
{
  Pixel::setAll(Pixel::r, Pixel::g, Pixel::b);
}

void Pixel::toggle (uint8_t pixel, uint8_t r, uint8_t g, uint8_t b)
{
  if (pixels.getPixelColor(pixel) == 0)
    Pixel::set(pixel, r, g, b);
  else
    Pixel::set(pixel, 0, 0, 0);
}

void Pixel::toggleAll (uint8_t r, uint8_t g, uint8_t b)
{
  if (pixels.getPixelColor(0) == 0 || pixels.getPixelColor(1) == 0)
    Pixel::setAll(r, g, b);
  else
    Pixel::setAll(0, 0, 0);
}

void Pixel::toggleCross (uint8_t r, uint8_t g, uint8_t b)
{
  if (pixels.getPixelColor(0) == 0)
  {
    Pixel::set(0, r, g, b);
    Pixel::set(1, 0, 0, 0);
  }
  else
  {
    Pixel::set(0, 0, 0, 0);
    Pixel::set(1, r, g, b);
  }
}

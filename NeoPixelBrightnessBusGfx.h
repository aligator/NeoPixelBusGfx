/*!
 * @file NeoPixelBrightnessBusGfx.h
 * Based on https://github.com/adafruit/Adafruit_NeoMatrix
 *
 * @mainpage GFX-compatible layer for NeoPixel matrices by using NeoPixelBus
 * internally.
 *
 * @section intro_sec Introduction
 *
 * Arduino library to control single and tiled matrices of WS2811- and
 * WS2812-based RGB LED devices such as the Adafruit NeoPixel Shield or
 * displays assembled from NeoPixel strips, making them compatible with
 * the Adafruit_GFX graphics library.
 * It is adapted to use the NeoPixelBus https://github.com/Makuna/NeoPixelBus
 * library instead of Adafruit_NeoPixel.
 *
 * @section dependencies Dependencies
 *
 * This library depends on <a
 * href="https://github.com/Makuna/NeoPixelBus"> NeoPixelBus</a>,
 * <a href="https://github.com/adafruit/Adafruit-GFX-Library"> Adafruit_GFX</a>
 * and <a href="https://github.com/adafruit/Adafruit_BusIO"> Adafruit_BusIO</a>
 * being present on your system. Please make sure you have installed the
 * latest versions before using this library.
 *
 * @section author Author
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Phil Burgess / Paint Your Dragon for Adafruit Industries.
 * Adapted to be compatible to NeoPixelBus by Johannes Hörmann
 * (https://github.com/aligator)
 *
 * @section license License
 *
 * This file is part of the Adafruit NeoPixelBusGfx library and is based on
 * based on https://github.com/adafruit/Adafruit_NeoMatrix.
 *
 * NeoPixelBusGfx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * NeoPixelBusGfx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with NeoPixelBusGfx.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _ADAFRUIT_NEOPIXELBRIGHTNESSBUSGFX_H_
#define _ADAFRUIT_NEOPIXELBRIGHTNESSBUSGFX_H_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif
#include "NeoGfx.h"
#include <Adafruit_GFX.h>
#include <NeoPixelBrightnessBus.h>

#include "gamma.h"
#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#else
#ifndef pgm_read_byte
#define pgm_read_byte(addr)                                                    \
  (*(const unsigned char *)(addr)) ///< PROGMEM concept doesn't apply on ESP8266
#endif
#endif

/**
 * @brief Class for using NeoPixel matrices with the GFX graphics library by
 * making use of NeoPixelBus including the brightness functionality.
 */
template <typename T_COLOR_FEATURE, typename T_METHOD>
class NeoPixelBrightnessBusGfx
    : public Adafruit_GFX,
      public NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD> {

public:
  // Constructor: number of LEDs, pin number
  // NOTE:  Pin Number maybe ignored due to hardware limitations of the method.

  NeoPixelBrightnessBusGfx(int w, int h, uint8_t pin)
      : Adafruit_GFX(w, h), NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>(
                                w * h, pin),
        neoGfx(NeoGfx<T_COLOR_FEATURE, T_METHOD,
                      NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>>(w, h,
                                                                        this)) {
  }

  NeoPixelBrightnessBusGfx(int w, int h, uint8_t pin, NeoBusChannel channel)
      : Adafruit_GFX(w, h), NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>(
                                w * h, pin, channel),
        neoGfx(NeoGfx<T_COLOR_FEATURE, T_METHOD,
                      NeoPixelBus<T_COLOR_FEATURE, T_METHOD>>(w, h, this)) {}

  NeoPixelBrightnessBusGfx(int w, int h, uint8_t pinClock, uint8_t pinData)
      : Adafruit_GFX(w, h), NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>(
                                w * h, pinClock, pinData),
        neoGfx(NeoGfx<T_COLOR_FEATURE, T_METHOD,
                      NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>>(w, h,
                                                                        this)) {
  }

  NeoPixelBrightnessBusGfx(int w, int h)
      : Adafruit_GFX(w, h), NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>(w *
                                                                             h),
        neoGfx(NeoGfx<T_COLOR_FEATURE, T_METHOD,
                      NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>>(w, h,
                                                                        this)) {
  }

protected:
  NeoGfx<T_COLOR_FEATURE, T_METHOD,
         NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>>
      neoGfx;

public:
  /**
   * @brief  Pixel-drawing function for Adafruit_GFX.
   * @param  x      Pixel column (0 = left edge, unless rotation used).
   * @param  y      Pixel row (0 = top edge, unless rotation used).
   * @param  color  Pixel color in 16-bit '565' RGB format.
   */
  void drawPixel(int16_t x, int16_t y, uint16_t color) {
    neoGfx.drawPixel(x, y, color, _width, _height, rotation, WIDTH, HEIGHT);
  }

  /**
   * @brief  Fill matrix with a single color.
   * @param  color  Pixel color in 16-bit '565' RGB format.
   */
  void fillScreen(uint16_t color) { neoGfx.fillScreen(color); }

  void clear() { neoGfx.fillScreen(0); }

  /**
   * @brief  Pass-through is a kludge that lets you override the current
   *         drawing color with a 'raw' RGB (NOT RGBW!, use RgbwColor(...)
   * instead, see deprecation note) value that's issued directly to pixel(s),
   * side-stepping the 16-bit color limitation of Adafruit_GFX. This is not
   * without some limitations of its own -- for example, it won't work in
   * conjunction with the background color feature when drawing text or bitmaps
   * (you'll just get a solid rect of color), only 'transparent' text/bitmaps.
   * Also, no gamma correction. Remember to UNSET the passthrough color
   * immediately when done with it (call with no value)!
   * @param  c  Pixel color in packed 32-bit 0RGB format.
   * @deprecated Prefer usage of the NeoPixelBus colors directly (e.g.
   * RgbColor(...) and RgbwColor(...)) as the usage of a white uint32_t is not
   * supported. (e.g. 0xFF000000 results in 0x000000 but RgbwColor(0, 0, 0, 255)
   * works)
   */
  void setPassThruColor(uint32_t c) { neoGfx.setPassThruColor(c); }

  /**
   * @brief  Pass-through is a kludge that lets you override the current
   *         drawing color with a 'NeoPixelBus' color value that's issued
   *         directly to pixel(s), side-stepping the 16-bit color limitation
   *         of Adafruit_GFX. This is not without some limitations of its
   *         own -- for example, it won't work in conjunction with the
   *         background color feature when drawing text or bitmaps (you'll
   *         just get a solid rect of color), only 'transparent'
   *         text/bitmaps.  Also, no gamma correction.
   *         Remember to UNSET the passthrough color immediately when done
   *         with it (call with no value)!
   * @param  c  Pixel color in the format from NeoPixelBus. e.g. RgbColor(...)
   * or RgbwColor(...)
   */
  void setPassThruColor(typename T_COLOR_FEATURE::ColorObject c) {
    neoGfx.setPassThruColor(c);
  }

  /**
   * @brief  Stop using pass-through color, return to normal 16-bit color
   *         usage.
   */
  void setPassThruColor(void) { neoGfx.setPassThruColor(); }

  /**
   *         pixel indices (for unusual layouts if if NEO_MATRIX_* and
   *         NEO_TILE_* settings do not provide sufficient control).
   * @param  fn  Pointer to function that accepts two uint16_t arguments
   *             (column and row), returns absolute pixel index.
   */
  void setRemapFunction(uint16_t (*fn)(uint16_t, uint16_t)) {
    neoGfx.setRemapFunction(fn);
  }

  /**
   * @brief   Quantize a 24-bit RGB color value to 16-bit '565' format.
   * @param   r         Red component (0 to 255).
   * @param   g         Green component (0 to 255).
   * @param   b         Blue component (0 to 255).
   * @return  uint16_t  Quantized color for GFX drawing functions.
   */
  uint16_t Color(uint8_t r, uint8_t g, uint8_t b) {
    return neoGfx.Color(r, g, b);
  }

  /**
   * @brief   Expand 16-bit input color (Adafruit_GFX colorspace) to 24-bit
   * (NeoPixel) (w/gamma adjustment)
   * @param   color     Color to convert.
   * @return  uint32_t  Expanded color.
   */
  static uint32_t expandColor(uint16_t color) {
    return NeoGfx<
        T_COLOR_FEATURE, T_METHOD,
        NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>>::expandColor(color);
  }
};

#endif // _ADAFRUIT_NEOPIXELBRIGHTNESSBUSGFX_H_
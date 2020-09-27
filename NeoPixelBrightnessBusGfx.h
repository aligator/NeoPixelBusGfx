/*--------------------------------------------------------------------
  This file is based on the Adafruit NeoMatrix library.

  NeoPixelBusGfx is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  NeoPixelBusGfx is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoPixelBusGfx.  If not, see
  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

#ifndef _ADAFRUIT_NEOPIXELBRIGHTNESSBUSGFX_H_
#define _ADAFRUIT_NEOPIXELBRIGHTNESSBUSGFX_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif
#include <Adafruit_GFX.h>
#include <NeoPixelBrightnessBus.h>
#include "NeoGfx.h"

#include "gamma.h"
#ifdef __AVR__
 #include <avr/pgmspace.h>
#elif defined(ESP8266)
 #include <pgmspace.h>
#else
 #ifndef pgm_read_byte
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
 #endif
#endif

template<typename T_COLOR_FEATURE, typename T_METHOD>
class NeoPixelBrightnessBusGfx : public Adafruit_GFX, public NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD> {

 public:

    // Constructor: number of LEDs, pin number
    // NOTE:  Pin Number maybe ignored due to hardware limitations of the method.
    
    NeoPixelBrightnessBusGfx(int w, int h, uint8_t pin) :
      Adafruit_GFX(w, h),
      NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>(w * h, pin),
      neoGfx(NeoGfx<T_COLOR_FEATURE, T_METHOD, NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>>(w, h, this))
    {
    }

    NeoPixelBrightnessBusGfx(int w, int h, uint8_t pinClock, uint8_t pinData) :
      Adafruit_GFX(w, h),
      NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>(w * h, pinClock, pinData),
      neoGfx(NeoGfx<T_COLOR_FEATURE, T_METHOD, NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>>(w, h, this))
    {
    }

    NeoPixelBrightnessBusGfx(int w, int h) :
      Adafruit_GFX(w, h),
      NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>(w * h),
      neoGfx(NeoGfx<T_COLOR_FEATURE, T_METHOD, NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>>(w, h, this))
    {
    }

    protected:
    NeoGfx<T_COLOR_FEATURE, T_METHOD, NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>> neoGfx;

  public:

    void drawPixel(int16_t x, int16_t y, uint16_t color) override {
      neoGfx.drawPixel(x, y, color, _width, _height, rotation, WIDTH, HEIGHT);
    }

    void fillScreen(uint16_t color) override {
      neoGfx.fillScreen(color);
    }

    void clear() {
      neoGfx.fillScreen(0);
    }

    void setPassThruColor(uint32_t c) {
      neoGfx.setPassThruColor(c);
    }

    void setPassThruColor(void) {
      neoGfx.setPassThruColor();
    }
    
    void setRemapFunction(uint16_t (*fn)(uint16_t, uint16_t)) {
      neoGfx.setRemapFunction(fn);
    }

    uint16_t Color(uint8_t r, uint8_t g, uint8_t b) {
      return neoGfx.Color(r, g, b);
    }

    static uint32_t expandColor(uint16_t color) {
      return NeoGfx<T_COLOR_FEATURE, T_METHOD, NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>>::expandColor(color);
    }
};

#endif // _ADAFRUIT_NEOPIXELBRIGHTNESSBUSGFX_H_
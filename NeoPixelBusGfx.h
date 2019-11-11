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

#ifndef _ADAFRUIT_NEOPIXELBUSGFX_H_
#define _ADAFRUIT_NEOPIXELBUSGFX_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif
#include <Adafruit_GFX.h>
#include <NeoPixelBus.h>

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
class NeoPixelBusGfx : public Adafruit_GFX, public NeoPixelBus<T_COLOR_FEATURE, T_METHOD> {

 public:

    // Constructor: number of LEDs, pin number
    // NOTE:  Pin Number maybe ignored due to hardware limitations of the method.
    
    NeoPixelBusGfx(int w, int h, uint8_t pin) :
      Adafruit_GFX(w, h),
      NeoPixelBus<T_COLOR_FEATURE, T_METHOD>(w * h, pin),
      matrixWidth(w), matrixHeight(h), remapFn(NULL)
    {
    }

    NeoPixelBusGfx(int w, int h, uint8_t pinClock, uint8_t pinData) :
      Adafruit_GFX(w, h),
      NeoPixelBus<T_COLOR_FEATURE, T_METHOD>(w * h, pinClock, pinData),
      matrixWidth(w), matrixHeight(h), remapFn(NULL)
    {
    }

    NeoPixelBusGfx(int w, int h) :
      Adafruit_GFX(w, h),
      NeoPixelBus<T_COLOR_FEATURE, T_METHOD>(w * h),
      matrixWidth(w), matrixHeight(h), remapFn(NULL)
    {
    }

    void drawPixel(int16_t x, int16_t y, uint16_t color) {
      if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;

      int16_t t;
      switch(rotation) {
      case 1:
        t = x;
        x = WIDTH  - 1 - y;
        y = t;
        break;
      case 2:
        x = WIDTH  - 1 - x;
        y = HEIGHT - 1 - y;
        break;
      case 3:
        t = x;
        x = y;
        y = HEIGHT - 1 - t;
        break;
      }

      int tileOffset = 0;
      int pixelOffset = 0;

      if(remapFn) { // Custom X/Y remapping function
        pixelOffset = (*remapFn)(x, y);
      }
      
      this->SetPixelColor(tileOffset + pixelOffset, 
        passThruFlag ? RgbColor(HtmlColor(passThruColor))  : RgbColor(HtmlColor(expandColor(color))));
    }

    void fillScreen(uint16_t color) {
      uint16_t i, n;
      uint32_t c;

      c = passThruFlag ? passThruColor : expandColor(color);
      n = this->PixelCount();

      for(i=0; i<n; i++) this->SetPixelColor(i, RgbColor(HtmlColor(c)));
    }

    // Pass-through is a kludge that lets you override the current drawing
    // color with a 'raw' RGB (or RGBW) value that's issued directly to
    // pixel(s), side-stepping the 16-bit color limitation of Adafruit_GFX.
    // This is not without some limitations of its own -- for example, it
    // won't work in conjunction with the background color feature when
    // drawing text or bitmaps (you'll just get a solid rect of color),
    // only 'transparent' text/bitmaps.  Also, no gamma correction.
    // Remember to UNSET the passthrough color immediately when done with
    // it (call with no value)!

    // Pass raw color value to set/enable passthrough
    void setPassThruColor(uint32_t c) {
      passThruColor = c;
      passThruFlag  = true;
    }

    // Call without a value to reset (disable passthrough)
    void setPassThruColor(void) {
      passThruFlag = false;
    }
    
    void setRemapFunction(uint16_t (*fn)(uint16_t, uint16_t)) {
      remapFn = fn;
    }

    // Expand 16-bit input color (Adafruit_GFX colorspace) to 24-bit (NeoPixel)
    // (w/gamma adjustment)
    static uint32_t expandColor(uint16_t color) {
      return ((uint32_t)pgm_read_byte(&gamma5[ color >> 11       ]) << 16) |
              ((uint32_t)pgm_read_byte(&gamma6[(color >> 5) & 0x3F]) <<  8) |
                        pgm_read_byte(&gamma5[ color       & 0x1F]);
    }

    // Downgrade 24-bit color to 16-bit (add reverse gamma lookup here?)
    static uint16_t Color(uint8_t r, uint8_t g, uint8_t b) {
      return ((uint16_t)(r & 0xF8) << 8) |
              ((uint16_t)(g & 0xFC) << 3) |
                        (b         >> 3);
    }

 protected:

  const uint8_t matrixWidth, matrixHeight;
  uint16_t (*remapFn)(uint16_t x, uint16_t y);

  uint32_t passThruColor;
  boolean passThruFlag = false;
};

#endif // _ADAFRUIT_NEOPIXELBUSGFX_H_

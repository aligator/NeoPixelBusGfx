// NeoPixelBusGfx example for a simple 32 x 8 pixel matrix.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include <Adafruit_GFX.h>
#include <NeoPixelBusGfx.h>
#include <NeoPixelBus.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN 3
#define WIDTH 32
#define HEIGHT 8

// See NeoPixelBus documentation for choosing the correct Feature and Method
// (https://github.com/Makuna/NeoPixelBus/wiki/NeoPixelBus-object)
NeoPixelBusGfx<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod> matrix(WIDTH, HEIGHT);

// See NeoPixelBus documentation for choosing the correct NeoTopology
// you may also use NeoTile or NeoMosaik 
// (https://github.com/Makuna/NeoPixelBus/wiki/Matrix-Panels-Support)
NeoTopology<ColumnMajorAlternating180Layout> topo(WIDTH, HEIGHT);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

// use a remap function to remap based on the topology, tile or mosaik
// this function is passed as remap function to the matrix
uint16_t remap(uint16_t x, uint16_t y) {
  return topo.MapProbe(x, y);
}

void setup() {
  matrix.Begin();

  // pass the remap function
  matrix.setRemapFunction(&remap);

  matrix.setTextWrap(false);
  // brightness currently not supported
  // matrix.SetBrightness(40);
  matrix.setTextColor(colors[0]);
}

int x    = matrix.width();
int pass = 0;

void loop() {
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("Howdy"));
  if(--x < -36) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.Show();
  delay(100);
}
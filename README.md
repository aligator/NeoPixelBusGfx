# NeoPixelBusGfx [![Build Status](https://github.com/aligator/NeoPixelBusGfx/workflows/Arduino%20Library%20CI/badge.svg)](https://github.com/aligator/NeoPixelBusGfx/actions)

When I searched for a library to use with the NeoPixel(-compatible) led, I noticed that the original Adafruit_GFX causes some serious flickering on an ESP.  
NeoPixelBus instead works great but does not provide the GFX methods Adafruit_GFX provides.

That's why I just combined them and created a (nearly) drop-in replacement of Adafruit_GFX which just uses NeoPixelBus to draw the stuff.  
That means it supports all led's which NeoPixelBus supports and provides the GFX tools from Adafruit_GFX.

But in the background it still uses these libs, so it requires NeoPixelBus and Adafruit_GFX libraries to run.

# Examples in Arduino IDE

After downloading, rename folder to 'NeoPixelBusGfx' and install in Arduino Libraries folder.

To run the examples you need the following dependencies. They should be available in the arduino ide library manager):
* [adafruit/Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
* [makuna/NeoPixelBus](https://github.com/Makuna/NeoPixelBus)
* [adafruit/Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO)


# Use with PlatformIO

Add this to your platformio.ini of your project.
```
lib_deps =
  aligator/NeoPixelBusGfx
  adafruit/Adafruit GFX Library
  makuna/NeoPixelBus
  adafruit/Adafruit BusIO
  Wire
```

If you want to run the examples with PlatformIO, just create a new PlatformIO project and copy the example code into it.

# Usage

The best way to start is to try the matrixtest.
The basic usage is:
* create a matrix instance using either NeoPixelBusGfx or NeoPixelBrightnessBusGfx if you need brightness functionality
```
// See NeoPixelBus documentation for choosing the correct Feature and Method
// (https://github.com/Makuna/NeoPixelBus/wiki/NeoPixelBus-object)
NeoPixelBrightnessBusGfx<NeoGrbFeature, Neo800KbpsMethod> matrix(WIDTH, HEIGHT, DATA_PIN);
```
* create a topo instance
```
// See NeoPixelBus documentation for choosing the correct NeoTopology
// you may also use NeoTile or NeoMosaik 
// (https://github.com/Makuna/NeoPixelBus/wiki/Matrix-Panels-Support)
NeoTopology<ColumnMajorAlternating180Layout> topo(WIDTH, HEIGHT);
```
* add a small helper function to map the pixels
```
// use a remap function to remap based on the topology, tile or mosaik
// this function is passed as remap function to the matrix
uint16_t remap(uint16_t x, uint16_t y) {
  return topo.Map(x, y);
}
```
* and set up the whole thing
```
void setup() {
  matrix.Begin();

  // pass the remap function
  matrix.setRemapFunction(&remap);

  //...
}
```
The remap function is then used to map the pixels to the chosen topography.  

One side not:  
In most cases methods starting with a capital letter are from NeoPixelBus and all other methods are from Adafruit_GFX or the NeoPixelBusGfx lib.

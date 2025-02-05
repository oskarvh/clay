# FT81x Renderer for the Clay library
The FT1x is a family of Bridgetek Embedded Video Engine (EVE) IC's. This renderer is intended to act between the Clay library and the FT81x chip, and is intended to run on an embedded system. 

Testing is done on a Rapsberry Pi RP2040, with the attached example. 

## Dependencies
- CMake version 3.27.7 or newer
- [FT800-FT813 library](https://github.com/RudolphRiedel/FT800-FT813) (included as a submodule)
- [RP2040 Pico-SDK library](https://github.com/raspberrypi/pico-sdk) (included as a submodule)

## Setup
TODO: Include a pinout guide for the RP2040 and the FT81x development board.

## Build
TODO

## Limitations
I do not intend to support touch from the get-go, as this is inteded to be a part of the [ComSciCalculator](https://github.com/oskarvh/ComSciCalculator/), but it may be included in the future. 
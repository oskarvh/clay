# FT81x Renderer for the Clay library
The FT1x is a family of Bridgetek Embedded Video Engine (EVE) IC's. This renderer is intended to act between the Clay library and the FT81x chip, and is intended to run on an embedded system. 

Testing is done on a Rapsberry Pi RP2040, with the attached example. 

## Dependencies
- CMake version 3.27.7 or newer
- [FT800-FT813 library](https://github.com/RudolphRiedel/FT800-FT813) (Fetched with CMake)
- [RP2040 Pico-SDK library](https://github.com/raspberrypi/pico-sdk) (Fetched with CMake)
- arm-none-eabi-gcc: Arm GNU Toolchain 13.3.Rel1 (Build arm-13.24) 13.3.1 20240614 or compatible with rp2040
- [Raspberry Pi Pico Board](https://www.raspberrypi.com/products/raspberry-pi-pico/)
- [VM810C50A-N](https://brtchip.com/product/vm810c50a-n/) development board, or any other FT81x device

## Setup

The requires SDKs (FT81X and Pico-SDK) are fetched during CMake build. 
To active this example, enable `CLAY_INCLUDE_FT81X_EXAMPLES` in the root directory CMakeLists.txt file. 

### Physical setup
The pin mapping between the [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/) and the [VM810C50A-N](https://brtchip.com/product/vm810c50a-n/) used in this example is shown in the table below. 
| [Raspberry Pi Pico Pin Number](https://datasheets.raspberrypi.com/pico/Pico-R3-A4-Pinout.pdf)  | [VM810C50A-N J5 Pin](https://brtchip.com/wp-content/uploads/2021/09/DS_VM810C.pdf) |
| ---------------------- | --------------- |
| 4 (GP2)           | 1 (SCK)  |
| 5 (GP3)           | 2 (MOSI) |
| 1 (GP0)           | 3 (MISO) |
| 2 (GP1)           | 4 (CS#) |
| Not Connected (floating) | 5 (INT#) |
| 22 (GP17)         | 6 (PD#) |
| 40 (VBUS)         | 7 (5V) |
| Not Connected (using onboard regulator) | 8 (3.3V) |
| 3/8/13/18/23/28/33/38 (GND) | 9 (GND) |
| 3/8/13/18/23/28/33/38 (GND) | 10 (GND) |

This project has been built and tested using VScode and Raspberry Pi Pico debugging, folloing [this](https://www.digikey.com/en/maker/projects/raspberry-pi-pico-and-rp2040-cc-part-1-blink-and-vs-code/7102fb8bca95452e9df6150f39ae8422) setup. 

### Software

The `launch.json` file is included below for convenience, but you should set up your own. 
```json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Pico Debug",
            "cwd": "${workspaceRoot}",
            "executable": "${command:cmake.launchTargetPath}",
            "request": "launch",
            "type": "cortex-debug",
            "servertype": "openocd",
            "gdbPath" : "arm-none-eabi-gdb",
            "device": "RP2040",
            "configFiles": [
                "interface/cmsis-dap.cfg",
                "target/rp2040.cfg"
            ],
            "svdFile": "${workspaceRoot}/examples/ft81x/pico-sdk/hardware_regs/rp2040.svd",
            "runToEntryPoint": "main",
            // Work around for stopping at main on restart
            "postRestartCommands": [
                "break main",
                "continue"
            ],
            "showDevDebugOutput": "raw"
        }
    ]
}
```

## Build
Activate the `CLAY_INCLUDE_FT81X_EXAMPLES` example in the root directory CMakeLists.txt file, build and download/debug to the RP2040. 

## Limitations
- Only text and rectangles are included as of now. 
- Fonts needs to be created using a specific tool. See [this](https://github.com/oskarvh/ComSciCalculator/blob/main/utils/README.md) for an example and helper script on how to generate the font header files. 

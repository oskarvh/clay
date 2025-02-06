cmake_minimum_required(VERSION 3.14)

# Pull in SDK (must be before project)
#include(pico_sdk_import.cmake)

project(clay-ft81x-demo C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

add_compile_definitions(APP_NAME="clay-ft81x-demo")
add_compile_definitions(RP2040)
# Definiton for the FT810CB_ADAFRUIT_1680 display
add_compile_definitions(EVE_FT810CB_ADAFRUIT_1680)

# Initialize the SDK
pico_sdk_init()


add_executable(
    clay-ft81x-demo
    main.c
)

# pull in common dependencies
target_link_libraries(
    clay-ft81x-demo 
    PUBLIC
    pico_stdlib
)

# Enable extra build products
pico_add_extra_outputs(clay-ft81x-demo)

# Enable/disable STDIO via USB and UART
# pico_enable_stdio_usb(ft81x-demo  0)
# pico_enable_stdio_uart(ft81x-demo  0)

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

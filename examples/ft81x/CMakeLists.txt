cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 11)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Fetch the FT81x SDK
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
    ft81x-sdk
    GIT_REPOSITORY "https://github.com/oskarvh/FT800-FT813"
    GIT_TAG "5.x"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_Populate(ft81x-sdk)
set(FT81X_SDK_PATH ${CMAKE_BINARY_DIR}/_deps/ft81x-sdk-src/)

# Let the pico SDK fetch itself
set(PICO_SDK_FETCH_FROM_GIT TRUE)
set(PICO_SDK_FETCH_FROM_GIT_TAG "2.1.1")
include(${CMAKE_SOURCE_DIR}/examples/ft81x/pico_sdk_import.cmake)

project(clay-ft81x-demo C CXX ASM)

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.1.1")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.1.1 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

add_compile_definitions(APP_NAME="clay-ft81x-demo")
add_compile_definitions(RP2040)
# Display definitions
add_compile_definitions(EVE_FT810CB_ADAFRUIT_1591)

# FT81x Pin mapping, done here instead of in pin mapping file to link properly
add_compile_definitions(EVE_CS=1)
add_compile_definitions(EVE_PDN=17)
add_compile_definitions(EVE_SCK=2)
add_compile_definitions(EVE_MOSI=3)
add_compile_definitions(EVE_MISO=0)
add_compile_definitions(EVE_BACKLIGHT_PWM=127)

# Initialize the SDK
pico_sdk_init()

add_executable(
    clay-ft81x-demo
    main.c
)

# Add FT81x/EVE display driver as a library
add_library(FT81X STATIC 
    ${FT81X_SDK_PATH}/EVE_commands.c
    ${FT81X_SDK_PATH}/EVE_target.c
)
target_link_libraries(
    FT81X 
    PUBLIC 
    pico_stdlib
    hardware_spi
)
target_include_directories(FT81X PUBLIC 
    ${FT81X_SDK_PATH}/
    ${FT81X_SDK_PATH}/EVE_target/
)
# Add the font library
add_library(FT81X_FONTS STATIC 
    ${CMAKE_SOURCE_DIR}/renderers/ft81x/fonts/font_library.c
)
target_link_libraries(
    FT81X_FONTS 
    PUBLIC 
)
target_include_directories(FT81X_FONTS PUBLIC 
    ${CMAKE_SOURCE_DIR}/renderers/ft81x/fonts/
)

# pull in dependencies into the demo
target_link_libraries(
    clay-ft81x-demo 
    PUBLIC
    pico_stdlib
    hardware_spi
    FT81X
    FT81X_FONTS
)
target_include_directories(clay-ft81x-demo  PUBLIC 
    ${CMAKE_SOURCE_DIR}/renderers/ft81x/
    ${CMAKE_SOURCE_DIR}/
)

# Enable extra build products
pico_add_extra_outputs(clay-ft81x-demo)

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

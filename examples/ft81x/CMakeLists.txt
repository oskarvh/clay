cmake_minimum_required(VERSION 3.14)

project(clay-ft81x-demo C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

add_compile_definitions(APP_NAME="clay-ft81x-demo")
add_compile_definitions(RP2040)
# Definiton for the FT810CB_ADAFRUIT_1680 display
add_compile_definitions(EVE_FT810CB_ADAFRUIT_1591)
#add_compile_definitions(__SIZE_MAX__=0xff000)
#add_compile_definitions(PICO_DEBUG_MALLOC)

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
    ${CMAKE_SOURCE_DIR}/examples/ft81x/FT800-FT813/EVE_commands.c
    ${CMAKE_SOURCE_DIR}/examples/ft81x//FT800-FT813/EVE_target.c
)
target_link_libraries(
    FT81X 
    PUBLIC 
    pico_stdlib
    hardware_spi
)
target_include_directories(FT81X PUBLIC 
    ${CMAKE_SOURCE_DIR}/examples/ft81x/FT800-FT813/
    ${CMAKE_SOURCE_DIR}/examples/ft81x/FT800-FT813/EVE_target/
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

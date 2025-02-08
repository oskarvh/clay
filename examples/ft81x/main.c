// Copyright (c) 2025 Oskar von Heideken (@oskarvh)
// Extension to the amazing clay library written by Justin Andreas Lacoste (@27justin)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the
// use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//     1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software in a
//     product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//
//     2. Altered source versions must be plainly marked as such, and must not
//     be misrepresented as being the original software.
//
//     3. This notice may not be removed or altered from any source
//     distribution.
//
// SPDX-License-Identifier: Zlib

#define CLAY_IMPLEMENTATION
#include "../../clay.h"
#include "../../renderers/ft81x/clay_renderer_ft81x.c"

// Standard C libraries
#include <stdio.h>

// RP2040 pico-sdk
#include "pico/stdlib.h"

const Clay_Color COLOR_LIGHT = (Clay_Color) {224, 215, 210, 255};
const Clay_Color COLOR_RED = (Clay_Color) {168, 66, 28, 255};
const Clay_Color COLOR_ORANGE = (Clay_Color) {225, 138, 50, 255};

void HandleClayErrors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    printf("%s", errorData.errorText.chars);
    switch(errorData.errorType) {
        // etc
    }
}

// Layout config is just a struct that can be declared statically, or inline
Clay_LayoutConfig sidebarItemLayout = (Clay_LayoutConfig) {
    .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50) },
};

// Re-useable components are just normal functions
void SidebarItemComponent() {
    CLAY(CLAY_LAYOUT(sidebarItemLayout), CLAY_RECTANGLE({ .color = COLOR_ORANGE })) {}
}

int main() {
    // Init MCU hardware
    stdio_init_all();

    // Initialize the FT81x and SPI
    Clay_ft81x_Initialize();

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));

    Clay_Initialize(arena, (Clay_Dimensions) { EVE_HSIZE, EVE_VSIZE }, (Clay_ErrorHandler) { HandleClayErrors });
    Clay_SetMeasureTextFunction(ft81x_MeasureText, 0);
    while(1) { // Will be different for each renderer / environment
        // Optional: Update internal layout dimensions to support resizing
        Clay_SetLayoutDimensions((Clay_Dimensions) { EVE_HSIZE, EVE_VSIZE });
        // Optional: Update internal pointer position for handling mouseover / click / touch events - needed for scrolling & debug tools
        //Clay_SetPointerState((Clay_Vector2) { mousePositionX, mousePositionY }, isMouseDown);
        // Optional: Update internal pointer position for handling mouseover / click / touch events - needed for scrolling and debug tools
        //Clay_UpdateScrollContainers(true, (Clay_Vector2) { mouseWheelX, mouseWheelY }, deltaTime);

        // All clay layouts are declared between Clay_BeginLayout and Clay_EndLayout
        Clay_BeginLayout();

        // An example of laying out a UI with a fixed width sidebar and flexible width main content
        CLAY(CLAY_ID("OuterContainer"), CLAY_LAYOUT({ .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .padding = CLAY_PADDING_ALL(16), .childGap = 16 }), CLAY_RECTANGLE({ .color = {250,250,255,255} })) {
            CLAY(CLAY_ID("SideBar"),
                 CLAY_LAYOUT({ .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) }, .padding = CLAY_PADDING_ALL(16), .childGap = 16 }),
                 CLAY_RECTANGLE({ .color = COLOR_LIGHT })
            ) {
                CLAY(CLAY_ID("ProfilePictureOuter"), CLAY_LAYOUT({ .sizing = { .width = CLAY_SIZING_GROW(0) }, .padding = CLAY_PADDING_ALL(16), .childGap = 16, .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } }), CLAY_RECTANGLE({ .color = COLOR_RED })) {
                    //CLAY(CLAY_ID("ProfilePicture"), CLAY_LAYOUT({ .sizing = { .width = CLAY_SIZING_FIXED(60), .height = CLAY_SIZING_FIXED(60) }}), CLAY_IMAGE({ .imageData = &profilePicture, .sourceDimensions = {60, 60} })) {}
                    CLAY_TEXT(CLAY_STRING("Clay - UI Library"), CLAY_TEXT_CONFIG({ .fontSize = 24, .textColor = {255, 255, 255, 255} }));
                }

                // Standard C code like loops etc work inside components
                for (int i = 0; i < 2; i++) {
                    SidebarItemComponent();
                }

                CLAY(CLAY_ID("MainContent"), CLAY_LAYOUT({ .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) }}), CLAY_RECTANGLE({ .color = COLOR_LIGHT })) {}
            }
        }
        Clay_RenderCommandArray renderCommands = Clay_EndLayout();
        Clay_ft81x_Render(renderCommands);
    }

}
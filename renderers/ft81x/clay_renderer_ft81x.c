// Copyright (c) 2025 Oskar von Heideken (@oskarvh)
// Extension to the amazing clay library written by Justin Andreas Lacoste (@27justin)
// This header copied to align with the rest of the repo.
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

// Include clay
#include "../../clay.h"

// EVE include, requires submodule from https://github.com/RudolphRiedel/FT800-FT813
#include "EVE.h"
#include "EVE_commands.h"

/**
 * @brief Starts the display list, clear local buffers and clears color buffers
 */
void startDisplaylist(void) {
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(CMD_DLSTART);
    EVE_cmd_dl_burst(DL_CLEAR_COLOR_RGB | BLACK);
    EVE_cmd_dl_burst(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
}

/**
 * @brief End the display list by sending display and swap DL
 */
void endDisplayList(void) {
    EVE_cmd_dl_burst(DL_DISPLAY);
    EVE_cmd_dl_burst(CMD_SWAP);
    EVE_end_cmd_burst();
    // Wait until EVE is not busy anymore to ensure conclusion of command.
    while (EVE_busy());
}

/**
 * @brief Initialize the ft81x.
 */
void Clay_ft81x_Initialize(){

}

/**
 * @brief Calculate the dimensions of a string. 
 * @param text Pointer to string
 * @param config Pointer to Clay config
 * @param userData No clue, but it's there
 */
static inline Clay_Dimensions ft81x_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, uintptr_t userData) {
    
}

/**
 * @brief Render the commands using FT81x
 * @param renderCommands List of commands to render. 
 */
void Clay_ft81x_Render(Clay_RenderCommandArray renderCommands){

}
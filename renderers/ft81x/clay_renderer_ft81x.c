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
#include "../../examples/ft81x/FT800-FT813/src/EVE_commands.h"
#include "../../examples/ft81x/FT800-FT813/src/EVE_commands.h"

/**
 * @brief Starts the display list, clear local buffers and clears color buffers
 */
void startDisplaylist(void) {
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(CMD_DLSTART);
    //EVE_cmd_dl_burst(DL_CLEAR_COLOR_RGB | BLACK); // TODO: Is this needed?
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
    if (!initSpi()) {
        // Something went wrong with initializing offboard SPI.
        while (1);
    }
    EVE_init_spi();
    EVE_init();
    while (EVE_busy());
    // Program the font library
    programFontLibrary();
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
void Clay_ft81x_Render(Clay_RenderCommandArray commands){
    for(size_t i = 0; i < commands.length; i++) {
		Clay_RenderCommand *command = Clay_RenderCommandArray_Get(&commands, i);

		switch(command->commandType) {
		case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
			Clay_RectangleElementConfig *config = command->config.rectangleElementConfig;
			Clay_Color color = config->color;
			Clay_BoundingBox bb = command->boundingBox;
			break;
		}
		case CLAY_RENDER_COMMAND_TYPE_TEXT: {

			Clay_BoundingBox bb = command->boundingBox;
			Clay_Color color = command->config.textElementConfig->textColor;

			break;
		}
		case CLAY_RENDER_COMMAND_TYPE_BORDER: {
			Clay_BorderElementConfig *config = command->config.borderElementConfig;
			Clay_BoundingBox bb = command->boundingBox;

			double top_left_radius = config->cornerRadius.topLeft / 2.0;
			double top_right_radius = config->cornerRadius.topRight / 2.0;
			double bottom_right_radius = config->cornerRadius.bottomRight / 2.0;
			double bottom_left_radius = config->cornerRadius.bottomLeft / 2.0;

			// Draw the top border
			if (config->top.width > 0) {
			}

			// Draw the right border
			if (config->right.width > 0) {
			}

			// Draw the bottom border
			if (config->bottom.width > 0) {
			}

			// Draw the left border
			if (config->left.width > 0) {
			}
			break;
		}
		case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {
			// Slot your custom elements in here.
		}
		default: {
			//fprintf(stderr, "Unknown command type %d\n", (int) command->commandType);
            break;
		}
		}
	}
}
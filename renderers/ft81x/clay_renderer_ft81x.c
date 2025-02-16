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

// Standard libs
#include <math.h>
#include <string.h>

// Include clay
#include "clay.h"

// EVE include, requires submodule from https://github.com/RudolphRiedel/FT800-FT813
#include "EVE.h"
#include "EVE_commands.h"

// Include fonts
#include "font_library.h"

//! Offset used when programming custom fonts into RAM_G
static uint32_t ram_g_address_offset = 0;

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
 * @brief Program font to the FT81x
 * @param pFont Pointer to the font struct
 * @param fontIndex The index in FT81x this font will get
 * @return Nothing
 * @warning This function assumes that the first char is a space.
 * If this is not the case, the setfont2 functions last parameter
 * must be changed.
 */
void programFont(font_t *pFont, uint8_t fontIndex) {
    uint32_t thisFontsAddress = EVE_RAM_G + ram_g_address_offset;
    // Null check the pointer
    if (pFont == NULL) {
        return;
    }
    // If this is a ROM font, or of the pointer to the table is NULL, then
    // return
    if (pFont->rom_font == true || pFont->pFontTable == NULL) {
        return;
    }
    // Write the bitmap of the pFont to the graphics RAM if there is room.
    if (ram_g_address_offset + pFont->fontTableSize < EVE_RAM_G_SIZE) {
        // Program the font
        EVE_memWrite_sram_buffer(EVE_RAM_G + ram_g_address_offset,
                                 pFont->pFontTable, pFont->fontTableSize);
        // Program the offset for the font.
        // This is at offset 144 in the font table,
        // and is per default set to 148, which is the offset
        // of the font to the start of the raw data.
        // However, if programming more than 1 font, this offset
        // needs to change, as it's the offset to the start of RAM_G
        // Hence, it should be the ram_g_address_offset
        uint32_t offsetInRam = ram_g_address_offset + 148;
        EVE_memWrite_sram_buffer(EVE_RAM_G + ram_g_address_offset + 144,
                                 (uint8_t *)&offsetInRam, 4);
        ram_g_address_offset += pFont->fontTableSize;
        // Round up to the nearest four byte aligned address:
        uint32_t offset = ram_g_address_offset % 4;
        ram_g_address_offset += offset;
    } else {
        return;
    }
    // This must be in a display list to register the new font
    startDisplaylist();
    // CMD_SETBITMAP not mandatory if using setfont2
    EVE_cmd_setfont2_burst(fontIndex, thisFontsAddress, 32);

    endDisplayList();
}

/**
 * @brief Program the font library
 */
void programFontLibrary(void) {
    // Go through all entries in the font library,
    // and program the large font at the even indexes, followed by the
    // small font add the odd ones.
    for (uint8_t i = 0; i < MAX_LEN_FONT_LIBRARY_TABLE; i++) {
        if (pFontLibraryTable[i] != NULL) {
            // Program the large index:
            programFont(pFontLibraryTable[i], i);
            pFontLibraryTable[i]->ft81x_font_index = i;
        }
    }
}

/**
 * @brief Find the width of the font, in pixels
 * @param pFont Pointer to the font struct
 * @param c Character to find the width of
 * @return Width of the current char at that font in pixels
 * @warning This function assumes that the first char is a space,
 * and follows the ASCII char setup
 */
uint8_t getFontCharWidth(font_t *pFont, char c) {
    // The font table contains a LUT for the char widths,
    // so using the char as an index, the width is simply the
    // value at that index. Conveniently, it's the first data
    // in the struct
    if (!pFont->rom_font) {
        return pFont->pFontTable[(uint8_t)c];
    } else {
        // TODO: How to find the width of a ROM font?
        return pFont->font_x_width;
    }
}

/**
 * @brief Initialize the ft81x and clay lib
 */
void Clay_ft81x_Initialize(){
	// Initialize the FT81x and SPI
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
	Clay_Dimensions textSize = { 0 };
	uint16_t fontId = config->fontId;
	font_t *pFont = pFontLibraryTable[fontId];
	textSize.height = pFont->font_baseline;
	for(int i = 0; i < text.length; i++) {
		textSize.width += getFontCharWidth(pFont, text.chars[i]);
	}
	return textSize;
}


/**
 * @brief Render the commands using FT81x
 * @param renderCommands List of commands to render. 
 */
void Clay_ft81x_Render(Clay_RenderCommandArray commands){
    startDisplaylist();
    for(size_t i = 0; i < commands.length; i++) {
        
		Clay_RenderCommand *command = Clay_RenderCommandArray_Get(&commands, i);

		switch(command->commandType) {
		case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
			Clay_RectangleElementConfig *config = command->config.rectangleElementConfig;
			Clay_Color color = config->color;
			Clay_BoundingBox bb = command->boundingBox;
            EVE_color_rgb_burst(COLOR_RGB((uint8_t)roundf(color.r), (uint8_t)roundf(color.g), (uint8_t)roundf(color.b)));
            EVE_color_a_burst((uint8_t)roundf(color.a));
			EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
			EVE_cmd_dl(VERTEX2F(bb.x * 16, bb.y * 16));
			EVE_cmd_dl(VERTEX2F((bb.x+bb.width) * 16, (bb.y + bb.height) * 16));
			break;
		}
		case CLAY_RENDER_COMMAND_TYPE_TEXT: {

			Clay_BoundingBox bb = command->boundingBox;
			Clay_Color color = command->config.textElementConfig->textColor;
            uint16_t fontId = command->config.textElementConfig->fontId;
            font_t *pFont = pFontLibraryTable[fontId];
            
            Clay_StringSlice text = command->text;
            char *cloned = (char *)malloc(text.length + 1);
            memcpy(cloned, text.chars, text.length);
            cloned[text.length] = '\0';
            EVE_color_rgb_burst(COLOR_RGB((uint8_t)roundf(color.r), (uint8_t)roundf(color.g), (uint8_t)roundf(color.b)));
            EVE_color_a_burst((uint8_t)roundf(color.a));
            EVE_cmd_text_burst(
                bb.x,
                bb.y,
                pFont->ft81x_font_index, 
                0, 
                cloned
            );
            free(cloned);
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
            break;
		}
		default: {
			//fprintf(stderr, "Unknown command type %d\n", (int) command->commandType);
            break;
		}
		}
        
	}
    endDisplayList();
}
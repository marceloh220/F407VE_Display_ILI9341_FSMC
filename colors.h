/**  
* @file color.h  
* @brief Color definitions for display. 
*  
* @author Marcelo H Moraes 
* 
* @date 09/13/2021
* Copyright (c) 2021, Marcelo H Moraes
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
*
* You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
* either express or implied.
*
* See the License for the specific language governing permissions and limitations under the License.
*/ 

#ifndef _COLORS_H_
#define _COLORS_H_

#include "inttypes.h"

typedef enum {
// Basic Color definitions
    BLACK    = (uint16_t)(0x0000)
    ,BLUE    = (uint16_t)(0x001F)
    ,RED     = (uint16_t)(0xF800)
    ,GREEN   = (uint16_t)(0x07E0)
    ,CYAN    = (uint16_t)(0x07FF)
    ,MAGENTA = (uint16_t)(0xF81F)
    ,YELLOW  = (uint16_t)(0xFFE0)
    ,WHITE   = (uint16_t)(0xFFFF)

    ,NAVY        = (uint16_t)(0x000F)
    ,DARKGREEN   = (uint16_t)(0x03E0)
    ,DARKCYAN    = (uint16_t)(0x03EF)
    ,MAROON      = (uint16_t)(0x7800)
    ,PURPLE      = (uint16_t)(0x780F)
    ,OLIVE       = (uint16_t)(0x7BE0)
    ,LIGHTGREY   = (uint16_t)(0xC618)
    ,DARKGREY    = (uint16_t)(0x7BEF)
    ,ORANGE      = (uint16_t)(0xFD20)
    ,GREENYELLOW = (uint16_t)(0xAFE5)

// Six Shades of Grey
    ,GRAY_15  = (uint16_t)(0x0861)    //  15  15  15
    ,GRAY_30  = (uint16_t)(0x18E3)    //  30  30  30
    ,GRAY_50  = (uint16_t)(0x3186)    //  50  50  50
    ,GRAY_80  = (uint16_t)(0x528A)    //  80  80  80
    ,GRAY_128 = (uint16_t)(0x8410)    // 128 128 128
    ,GRAY_200 = (uint16_t)(0xCE59)    // 200 200 200

// Five Shades of Green
    ,GREEN_BASE    = (uint16_t)(0xD7F0)    // 211 255 130
    ,GREEN_DARKER  = (uint16_t)(0x8DE8)    // 137 188  69
    ,GREEN_LIGHTER = (uint16_t)(0xEFF9)    // 238 255 207
    ,GREEN_SHADOW  = (uint16_t)(0x73EC)    // 119 127 103
    ,GREEN_ACCENT  = (uint16_t)(0xAE6D)    // 169 204 104

// Mr. Blue Sky, why you had to hide away for so long? Where did we go wrong?
    ,BLUESKY_BASE    = (uint16_t)(0x95BF)    // 150 180 255
    ,BLUESKY_DARKER  = (uint16_t)(0x73B0)    // 113 118 131
    ,BLUESKY_LIGHTER = (uint16_t)(0xE75F)    // 227 235 255
    ,BLUESKY_SHADOW  = (uint16_t)(0x4ACF)    //  75  90 127
    ,BLUESKY_ACCENT  = (uint16_t)(0xB5F9)    // 182 188 204

// Five Shades of Violet
    ,VIOLET_BASE    = (uint16_t)(0x8AEF)    // 143  94 124
    ,VIOLET_DARKER  = (uint16_t)(0x4187)    //  66  49  59
    ,VIOLET_LIGHTER = (uint16_t)(0xC475)    // 194 142 174
    ,VIOLET_SHADOW  = (uint16_t)(0x40E6)    //  66  29  52
    ,VIOLET_ACCENT  = (uint16_t)(0xC992)    // 204  50 144

// Five Shades of Earthy or whatever this color are
    ,EARTHY_BASE    = (uint16_t)(0x6269)    //  97  79  73
    ,EARTHY_DARKER  = (uint16_t)(0x3103)    //  48  35  31
    ,EARTHY_LIGHTER = (uint16_t)(0x8C30)    // 140 135 129
    ,EARTHY_SHADOW  = (uint16_t)(0xAB29)    // 173 102  79
    ,EARTHY_ACCENT  = (uint16_t)(0xFE77)    // 250 204 188

} color_t;

#endif /* _COLORS_H_ */

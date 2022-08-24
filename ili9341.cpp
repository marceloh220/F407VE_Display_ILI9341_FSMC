/**  
* @file ili9341.cpp  
* @brief Library with class to control display  
* with ILI9341 controller. 
*  
* @author Marcelo H Moraes 
* 
* @date 09/13/2021
*
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

#include <cstdint>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "ili9341.h"
#include "registers.h"

typedef enum {
	MemoryAccessControlNormalOrder, MemoryAccessControlReverseOrder
} MemoryAccessControlRefreshOrder_t;

typedef enum {
	MemoryAccessControlColorOrderRGB, MemoryAccessControlColorOrderBGR
} MemoryAccessControlColorOrder_t;

/*!
 * @brief  Memory mappiing access for display orienation
 */
static unsigned char lcdBuildMemoryAccessControlConfig(
		MemoryAccessControlRefreshOrder_t rowAddressOrder,
		MemoryAccessControlRefreshOrder_t columnAddressOrder,
		MemoryAccessControlRefreshOrder_t rowColumnExchange,
		MemoryAccessControlRefreshOrder_t verticalRefreshOrder,
		MemoryAccessControlColorOrder_t colorOrder,
		MemoryAccessControlRefreshOrder_t horizontalRefreshOrder);

/*!
 * @brief  Starter the FSMC peripheral
 */
static void FSMC_init();

/* --- Public methods --- */

ILI9341::ILI9341(PinName rst, PinName bl,
		Ax_t Ax, NEx_t NEx,
		uint16_t width, uint16_t height, lcdOrientation_t orientation) : _rst(rst), _bl(bl) {
	Properties.width = width;
	Properties.height = height;
	Properties.orientation = orientation;
	fsmcCMD = (volatile uint16_t*)NEx;
	fsmcDATA = (volatile uint16_t*)(NEx+Ax);
}

void ILI9341::begin(void) {
	PortraitConfig = lcdBuildMemoryAccessControlConfig(
			MemoryAccessControlNormalOrder,		// rowAddressOrder
			MemoryAccessControlReverseOrder,	// columnAddressOrder
			MemoryAccessControlNormalOrder,		// rowColumnExchange
			MemoryAccessControlNormalOrder,		// verticalRefreshOrder
			MemoryAccessControlColorOrderBGR,	// colorOrder
			MemoryAccessControlNormalOrder);	// horizontalRefreshOrder

	LandscapeConfig = lcdBuildMemoryAccessControlConfig(
			MemoryAccessControlNormalOrder,		// rowAddressOrder
			MemoryAccessControlNormalOrder,		// columnAddressOrder
			MemoryAccessControlReverseOrder,	// rowColumnExchange
			MemoryAccessControlNormalOrder,		// verticalRefreshOrder
			MemoryAccessControlColorOrderBGR,	// colorOrder
			MemoryAccessControlNormalOrder);	// horizontalRefreshOrder

	PortraitMirrorConfig = lcdBuildMemoryAccessControlConfig(
			MemoryAccessControlReverseOrder,	// rowAddressOrder
			MemoryAccessControlNormalOrder,		// columnAddressOrder
			MemoryAccessControlNormalOrder,		// rowColumnExchange
			MemoryAccessControlNormalOrder,		// verticalRefreshOrder
			MemoryAccessControlColorOrderBGR,	// colorOrder
			MemoryAccessControlNormalOrder);	// horizontalRefreshOrder

	LandscapeMirrorConfig = lcdBuildMemoryAccessControlConfig(
			MemoryAccessControlReverseOrder,	// rowAddressOrder
			MemoryAccessControlReverseOrder,	// columnAddressOrder
			MemoryAccessControlReverseOrder,	// rowColumnExchange
			MemoryAccessControlNormalOrder,		// verticalRefreshOrder
			MemoryAccessControlColorOrderBGR,	// colorOrder
			MemoryAccessControlNormalOrder);	// horizontalRefreshOrder
	FSMC_init();
	reset();

	writeCommand(ILI9341_DISPLAYOFF);
/*
	writeCommand(0xCF);
	writeData(0x00);writeData(0x83);writeData(0x30);
    
    writeCommand(0xED);
	writeData(0x64);writeData(0x03);writeData(0x12);writeData(0x81);

	writeCommand(0xE8);
	writeData(0x85);writeData(0x01);writeData(0x79);

	writeCommand(0xCB);
	writeData(0x39);writeData(0x2C);writeData(0x00);writeData(0x34);
	writeData(0x02);

	writeCommand(0xF7);
	writeData(0x20);

	writeCommand(0xEA);
	writeData(0x00);writeData(0x00);
*/
	writeCommand(ILI9341_POWERCONTROL1);
	writeData(0x26);

	writeCommand(ILI9341_POWERCONTROL2);
	writeData(0x11);

	writeCommand(ILI9341_VCOMCONTROL1);
	writeData(0x35);writeData(0x3E);

	writeCommand(ILI9341_VCOMCONTROL2);
	writeData(0xBE);

	writeCommand(ILI9341_MEMCONTROL);
	writeData(PortraitConfig);

	writeCommand(ILI9341_PIXELFORMAT);
	writeData(0x55);

	writeCommand(ILI9341_FRAMECONTROLNORMAL);
	writeData(0x00);writeData(0x1B);

	writeCommand(0xF2);
	writeData(0x08);

	writeCommand(ILI9341_GAMMASET);
	writeData(0x01);

	writeCommand(ILI9341_POSITIVEGAMMCORR);
	writeData(0x1F);writeData(0x1A);writeData(0x18);writeData(0x0A);
	writeData(0x0F);writeData(0x06);writeData(0x45);writeData(0x87);
	writeData(0x32);writeData(0x0A);writeData(0x07);writeData(0x02);
	writeData(0x07);writeData(0x05);writeData(0x00);

	writeCommand(ILI9341_NEGATIVEGAMMCORR);
	writeData(0x00);writeData(0x25);writeData(0x27);writeData(0x05);
	writeData(0x10);writeData(0x09);writeData(0x3A);writeData(0x78);
	writeData(0x4D);writeData(0x05);writeData(0x18);writeData(0x0D);
	writeData(0x38);writeData(0x3A);writeData(0x1F);

	writeCommand(ILI9341_COLADDRSET);
	writeData(0x00);writeData(0x00);writeData(0x00);writeData(0xEF);

	writeCommand(ILI9341_PAGEADDRSET);
	writeData(0x00);writeData(0x00);writeData(0x01);writeData(0x3F);

	writeCommand(ILI9341_ENTRYMODE);
	writeData(0x07);

	writeCommand(ILI9341_DISPLAYFUNC);
	writeData(0x0A);writeData(0x82);writeData(0x27);writeData(0x00);

	writeCommand(ILI9341_SLEEPOUT);
	ThisThread::sleep_for(100ms);
	writeCommand(ILI9341_DISPLAYON);
	ThisThread::sleep_for(100ms);
//	writeCommand(ILI9341_MEMORYWRITE);
    fillScreen(BLACK);
    _bl.period(.005);
    _bright = 50;
    _bl = .5;

}

void ILI9341::test(void) {
	setWindow(0, 0, Properties.width - 1, Properties.height - 1);
	
	uint8_t stripSize = Properties.height / 8;

	for (int y = 0; y < Properties.height; y++) {
		for (int x = 0; x < Properties.width; x++) {
			if (y > Properties.height - 1 - (stripSize * 1))
				writeData(WHITE);
			else if (y > Properties.height - 1 - (stripSize * 2))
				writeData(BLUE);
			else if (y > Properties.height - 1 - (stripSize * 3))
				writeData(GREEN);
			else if (y > Properties.height - 1 - (stripSize * 4))
				writeData(CYAN);
			else if (y > Properties.height - 1 - (stripSize * 5))
				writeData(RED);
			else if (y > Properties.height - 1 - (stripSize * 6))
				writeData(MAGENTA);
			else if (y > Properties.height - 1 - (stripSize * 7))
				writeData(YELLOW);
			else
				writeData(BLACK);
		}
	}
}

uint16_t ILI9341::color565(uint8_t r, uint8_t g, uint8_t b) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void ILI9341::fillScreen(uint16_t color) {
	setWindow(0, 0, Properties.width - 1, Properties.height - 1);
	int dimensions = Properties.width * Properties.height;
	while (dimensions--) {
		writeData(color);
	}
}

void ILI9341::drawPixel(uint16_t x, uint16_t y, uint16_t color) {
	// Clip
	if ((x < 0) || (y < 0) || (x >= Properties.width)
			|| (y >= Properties.height))
		return;

	setWindow(x, y, x, y);
	writeData(color);
}

void ILI9341::drawPixels(uint16_t x, uint16_t y, uint16_t *data, uint32_t dataLength) {

	setWindow(x, y, Properties.width - 1, Properties.height - 1);

	do {
		writeData(*data++);
	} while (--dataLength);
}

void ILI9341::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
	// Bresenham's algorithm - thx wikpedia

	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			drawPixel(y0, x0, color);
		} else {
			drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void ILI9341::drawFastHLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color) {
	// Allows for slightly better performance than setting individual pixels

	if (width < x) {
		// Switch direction
		swap(x, width);
	}

	// Check limits
	if (width >= Properties.width) {
		width = Properties.width - 1;
	}

	if (x >= Properties.width) {
		x = Properties.width - 1;
	}

	setWindow(x, y, width, y);
	for (int line = x; line <= width; line++) {
		writeData(color);
	}
}

void ILI9341::drawFastVLine(uint16_t x, uint16_t y, uint16_t height, uint16_t color) {
	if (height < y) {
        // Switch direction
		swap(y, height);
	}

	if (x >= Properties.width) {
		x = Properties.width - 1;
	}

	if (y >= Properties.height) {
		y = Properties.height - 1;
	}

	if (height >= Properties.height) {
		height = Properties.height - 1;
	}

	setWindow(x, y, x, height);
	for (int line = y; line <= height; line++) {
		writeData(color);
	}
}

void ILI9341::drawRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color) {
	drawFastHLine(x, y, x + width - 1, color);
	drawFastHLine(x, y + height - 1, x + width - 1, color);
	drawFastVLine(x, y, y + height - 1, color);
	drawFastVLine(x + width - 1, y, y + height - 1, color);
}

void ILI9341::fillRect(int16_t x, int16_t y, int16_t width, int16_t height,	uint16_t fillcolor) {
	// clipping
	if ((x >= Properties.width) || (y >= Properties.height))
		return;
	if ((x + width - 1) >= Properties.width)
		width = Properties.width - x;
	if ((y + height - 1) >= Properties.height)
		height = Properties.height - y;

	for (int16_t y1 = y; y1 <= y + height; y1++) {
		drawFastHLine(x, y1, x + width, fillcolor);
	}
}

void ILI9341::drawRoundRect(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius, uint16_t color) {
	// smarter version
	drawFastHLine(x + radius, y, x + width - radius, color);
	drawFastHLine(x + radius, y + height - 1, x + width - radius, color);
	drawFastVLine(x, y + radius, y + height - radius, color);
	drawFastVLine(x + width - 1, y + radius, y + height - radius, color);

	// draw four corners
	drawCircleHelper(x + radius, y + radius, radius, 1, color);
	drawCircleHelper(x + width - radius - 1, y + radius, radius, 2, color);
	drawCircleHelper(x + width - radius - 1, y + height - radius - 1, radius, 4, color);
	drawCircleHelper(x + radius, y + height - radius - 1, radius, 8, color);
}

void ILI9341::fillRoundRect(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius, uint16_t color) {
	// smarter version
	fillRect(x + radius, y, width - 2 * radius, height, color);

	// draw four corners
	fillCircleHelper(x + width - radius - 1, y + radius, radius, 1, height - 2 * radius - 1, color);
	fillCircleHelper(x + radius, y + radius, radius, 2, height - 2 * radius - 1, color);
}

void ILI9341::drawCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color) {
	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x = 0;
	int16_t y = radius;

	drawPixel(x0, y0 + radius, color);
	drawPixel(x0, y0 - radius, color);
	drawPixel(x0 + radius, y0, color);
	drawPixel(x0 - radius, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);
		drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 - y, y0 - x, color);
	}
}

void ILI9341::fillCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color) {
	drawFastVLine(x0, y0 - radius, y0 + radius + 1, color);
	fillCircleHelper(x0, y0, radius, 3, 0, color);
}

void ILI9341::drawCircleHelper(int16_t x0, int16_t y0, int16_t radius, uint8_t cornername, uint16_t color) {
	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x = 0;
	int16_t y = radius;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			drawPixel(x0 + x, y0 + y, color);
			drawPixel(x0 + y, y0 + x, color);
		}
		if (cornername & 0x2) {
			drawPixel(x0 + x, y0 - y, color);
			drawPixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			drawPixel(x0 - y, y0 + x, color);
			drawPixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			drawPixel(x0 - y, y0 - x, color);
			drawPixel(x0 - x, y0 - y, color);
		}
	}
}

void ILI9341::fillCircleHelper(int16_t x0, int16_t y0, int16_t radius, uint8_t cornername, int16_t delta, uint16_t color) {
	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x = 0;
	int16_t y = radius;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine(x0 + x, y0 - y, y0 + y + 1 + delta, color);
			drawFastVLine(x0 + y, y0 - x, y0 + x + 1 + delta, color);
		}
		if (cornername & 0x2) {
			drawFastVLine(x0 - x, y0 - y, y0 + y + 1 + delta, color);
			drawFastVLine(x0 - y, y0 - x, y0 + x + 1 + delta, color);
		}
	}
}

void ILI9341::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}

void ILI9341::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {

	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		swap(y0, y1);
		swap(x0, x1);
	}
	if (y1 > y2) {
		swap(y2, y1);
		swap(x2, x1);
	}
	if (y0 > y1) {
		swap(y0, y1);
		swap(x0, x1);
	}

	if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;
		drawFastHLine(a, y0, b + 1, color);
		return;
	}

	int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
			dx12 = x2 - x1, dy12 = y2 - y1;
	int32_t sa = 0, sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
		last = y1;   // Include y1 scanline
	else
		last = y1 - 1; // Skip it

	for (y = y0; y <= last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		 a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		 b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			swap(a, b);
		drawFastHLine(a, y, b + 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = (int32_t) dx12 * (y - y1);
	sb = (int32_t) dx02 * (y - y0);
	for (; y <= y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		 a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		 b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			swap(a, b);
		drawFastHLine(a, y, b + 1, color);
	}
}

void ILI9341::drawImage(uint16_t x, uint16_t y, const sImage_t *pBitmap) {
	if ((x >= Properties.width) || (y >= Properties.height))
		return;
	if ((x + pBitmap->width - 1) >= Properties.width)
		return;
	if ((y + pBitmap->height - 1) >= Properties.height)
		return;

	for (int i = 0; i < pBitmap->height; ++i) {
		drawPixels(x, y + i, (uint16_t*)(pBitmap->pData + i * pBitmap->bytesPerLine),
				pBitmap->bytesPerLine / (pBitmap->bitsPerPixel / 8));
	}
}

void ILI9341::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg) {
	if ((x >= Properties.width) || 			// Clip right
		(y >= Properties.height) || 		// Clip bottom
		((x + Font.pFont->Width) < 0) || // Clip left
		((y + Font.pFont->Height) < 0))  // Clip top
		return;

	uint8_t fontCoeff = Font.pFont->Height / 8;
	uint8_t xP = 0;

	for (uint8_t i = 0; i < Font.pFont->Height; i++) {
		uint8_t line;

		for (uint8_t j = 0; j < fontCoeff; j++) {
			line = Font.pFont->table[((c - 0x20) * Font.pFont->Height
					* fontCoeff) + (i * fontCoeff) + j];

			for (uint8_t k = 0; k < 8; k++) {
				if ((line & 0x80) == 0x80) {
					drawPixel(x + k + xP, y + i, color);
				} else if (bg != color) {
					drawPixel(x + k + xP, y + i, bg);
				}
				line <<= 1;
			}

			xP += 8;
		}

		xP = 0;
	}
}

void ILI9341::printf(const char *fmt, ...) {
	static char buf[256];
	char *p;
	va_list lst;
	va_start(lst, fmt);
	vsprintf(buf, fmt, lst);
	va_end(lst);
	p = buf;
	while (*p) {
		if (*p == '\n') {
			cursorXY.y += Font.pFont->Height;
			cursorXY.x = 0;
		} else if (*p == '\r') {
			// skip em
		} else if (*p == '\t') {
			cursorXY.x += Font.pFont->Width * 4;
		} else {
			drawChar(cursorXY.x, cursorXY.y, *p, Font.TextColor,
					Font.BackColor);
			cursorXY.x += Font.pFont->Width;
			if (Font.TextWrap
					&& (cursorXY.x
							> (Properties.width - Font.pFont->Width))) {
				cursorXY.y += Font.pFont->Height;
				cursorXY.x = 0;
			}
		}
		p++;
		if (cursorXY.y >= Properties.height) {
			cursorXY.y = 0;
		}
	}
}

void ILI9341::clrLine(uint16_t bg) {
    uint16_t width = Properties.width;
    uint16_t height = Font.pFont->Height;
    for(uint16_t y = cursorXY.y; y < (cursorXY.y+height); y++) {
        drawFastHLine(0, y, width, bg);
    }
}

void ILI9341::clrLine() {
    clrLine(Font.BackColor);
}

void ILI9341::setTextFont(font_t *font) {
	Font.pFont = font;
}

void ILI9341::setTextColor(uint16_t textColor, uint16_t bgColor) {
	Font.TextColor = textColor;
	Font.BackColor = bgColor;
}

void ILI9341::setTextWrap(uint8_t width) {
	Font.TextWrap = width;
}

void ILI9341::setOrientation(lcdOrientation_t value) {
	Properties.orientation = value;
	writeCommand(ILI9341_MEMCONTROL);

	switch (Properties.orientation) {
	case LCD_ORIENTATION_PORTRAIT:
		writeData(PortraitConfig);
		Properties.width = ILI9341_PIXEL_WIDTH;
		Properties.height = ILI9341_PIXEL_HEIGHT;
		break;
	case LCD_ORIENTATION_PORTRAIT_MIRROR:
		writeData(PortraitMirrorConfig);
		Properties.width = ILI9341_PIXEL_WIDTH;
		Properties.height = ILI9341_PIXEL_HEIGHT;
		break;
	case LCD_ORIENTATION_LANDSCAPE:
		writeData(LandscapeConfig);
		Properties.width = ILI9341_PIXEL_HEIGHT;
		Properties.height = ILI9341_PIXEL_WIDTH;
		break;
	case LCD_ORIENTATION_LANDSCAPE_MIRROR:
		writeData(LandscapeMirrorConfig);
		Properties.width = ILI9341_PIXEL_HEIGHT;
		Properties.height = ILI9341_PIXEL_WIDTH;
		break;
	default:
		break;
	}

	//writeCommand(ILI9341_MEMORYWRITE);
	setWindow(0, 0, Properties.width - 1, Properties.height - 1);
}

void ILI9341::home(void) {
	cursorXY.x = 0;
	cursorXY.y = 0;
	setWindow(0, 0, Properties.width - 1, Properties.height - 1);
}

void ILI9341::setCursor(uint16_t x, uint16_t y) {
	cursorXY.x = x;
	cursorXY.y = y;
	setWindow(x, y, x, y);
}

uint16_t ILI9341::getColumn(uint16_t coll) {
    return (coll * (Font.pFont->Width));
}

uint16_t ILI9341::getLine(uint16_t line) {
    return (line * (Font.pFont->Height));
}

void ILI9341::backlightBright(uint8_t bright) {
    if(bright > 100) bright = 100;
    if(bright < 0) bright = 0;
    _bright = bright;
	_bl = bright/100.0;
}

uint8_t ILI9341::backlightBright() {
    return _bright;
}

void ILI9341::inversionOff(void) {
	writeCommand(ILI9341_INVERTOFF);
}

void ILI9341::inversionOn(void) {
	writeCommand(ILI9341_INVERTON);
}

void ILI9341::displayOff(void) {
	writeCommand(ILI9341_DISPLAYOFF);
	backlightBright(0);
}

void ILI9341::displayOn(void) {
	writeCommand(ILI9341_DISPLAYON);
	backlightBright(_bright);
}

void ILI9341::tearingOff(void) {
	writeCommand(ILI9341_TEARINGEFFECTOFF);
}

void ILI9341::tearingOn(bool m) {
	writeCommand(ILI9341_TEARINGEFFECTON);
	writeData(m);
}

uint16_t ILI9341::getWidth(void) {
	return Properties.width;
}

uint16_t ILI9341::getHeight(void) {
	return Properties.height;
}

uint16_t ILI9341::getControllerID(void) {
	uint16_t id;
	writeCommand(ILI9341_READID4);
	id = readData();
	id = readData();
	id = ((uint16_t) readData() << 8);
	id |= readData();
	return id;
}

lcdOrientation_t ILI9341::GetOrientation(void) {
	return Properties.orientation;
}

font_t* ILI9341::getTextFont(void) {
	return Font.pFont;
}

lcdProperties_t ILI9341::getProperties(void) {
	return Properties;
}

uint16_t ILI9341::readPixel(uint16_t x, uint16_t y) {
	uint16_t temp[3];
	// Clip
	if ((x < 0) || (y < 0) || (x >= Properties.width) || (y >= Properties.height))
		return 0;

	writeCommand(ILI9341_COLADDRSET);
	writeData((x >> 8) & 0xFF);
	writeData(x & 0xFF);

	writeCommand(ILI9341_PAGEADDRSET);
	writeData((y >> 8) & 0xFF);
	writeData(y & 0xFF);

	writeCommand(ILI9341_MEMORYREAD);

	temp[0] = readData(); // dummy read
	temp[1] = readData();
	temp[2] = readData();

	return color565((temp[1] >> 8) & 0xFF, temp[1] & 0xFF,
			(temp[2] >> 8) & 0xFF);
}

void ILI9341::reset(void) {
    if(_rst.is_connected()) {
        _rst = 1;
        ThisThread::sleep_for(10ms);
        _rst = 0;
        ThisThread::sleep_for(50ms);
        _rst = 1;
        ThisThread::sleep_for(10ms);
    }
    else {
	    writeCommand(ILI9341_SOFTRESET);
	    ThisThread::sleep_for(50ms);
    }
}

/* --- Protected methods --- */

void ILI9341::setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	writeCommand(ILI9341_COLADDRSET);
	writeData((x0 >> 8) & 0xFF);
	writeData(x0 & 0xFF);
	writeData((x1 >> 8) & 0xFF);
	writeData(x1 & 0xFF);
	writeCommand(ILI9341_PAGEADDRSET);
	writeData((y0 >> 8) & 0xFF);
	writeData(y0 & 0xFF);
	writeData((y1 >> 8) & 0xFF);
	writeData(y1 & 0xFF);
	writeCommand(ILI9341_MEMORYWRITE);
}

int  ILI9341::_putc(int c) {
    if (c == '\n') {
        cursorXY.y += Font.pFont->Height;
        cursorXY.x = 0;
    } else if (c == '\r') {
        // skip em
    } else if (c == '\t') {
		cursorXY.x += Font.pFont->Width * 4;
    } else {
        drawChar(cursorXY.x, cursorXY.y, c, Font.TextColor, Font.BackColor);
        cursorXY.x += Font.pFont->Width;
        if (Font.TextWrap && (cursorXY.x > (Properties.width - Font.pFont->Width))) {
            cursorXY.y += Font.pFont->Height;
            cursorXY.x = 0;
        }
    }
    return c;
}

int ILI9341::_getc() {
    return -1;
}

/* --- Static functions --- */
static unsigned char lcdBuildMemoryAccessControlConfig(
		MemoryAccessControlRefreshOrder_t rowAddressOrder,
		MemoryAccessControlRefreshOrder_t columnAddressOrder,
		MemoryAccessControlRefreshOrder_t rowColumnExchange,
		MemoryAccessControlRefreshOrder_t verticalRefreshOrder,
		MemoryAccessControlColorOrder_t colorOrder,
		MemoryAccessControlRefreshOrder_t horizontalRefreshOrder) {

	unsigned char value = 0;
	if (horizontalRefreshOrder)
		value |= ILI9341_MADCTL_MH;
	if (colorOrder)
		value |= ILI9341_MADCTL_BGR;
	if (verticalRefreshOrder)
		value |= ILI9341_MADCTL_ML;
	if (rowColumnExchange)
		value |= ILI9341_MADCTL_MV;
	if (columnAddressOrder)
		value |= ILI9341_MADCTL_MX;
	if (rowAddressOrder)
		value |= ILI9341_MADCTL_MY;
	return value;

}

static void FSMC_init() {

	static int FSMC_Initialized = 0;
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	SRAM_HandleTypeDef SRAM_LCD = {0};

	if (FSMC_Initialized) {
		return;
	}
	FSMC_Initialized = 1;

	/* Peripheral clock enable */
	__HAL_RCC_FSMC_CLK_ENABLE();

	/** FSMC GPIO Configuration
	 PE7   ------> FSMC_D4
	 PE8   ------> FSMC_D5
	 PE9   ------> FSMC_D6
	 PE10   ------> FSMC_D7
	 PE11   ------> FSMC_D8
	 PE12   ------> FSMC_D9
	 PE13   ------> FSMC_D10
	 PE14   ------> FSMC_D11
	 PE15   ------> FSMC_D12
	 PD8   ------> FSMC_D13
	 PD9   ------> FSMC_D14
	 PD10   ------> FSMC_D15
	 PD13   ------> FSMC_A18
	 PD14   ------> FSMC_D0
	 PD15   ------> FSMC_D1
	 PD0   ------> FSMC_D2
	 PD1   ------> FSMC_D3
	 PD4   ------> FSMC_NOE
	 PD5   ------> FSMC_NWE
	 PD7   ------> FSMC_NE1
	 */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
			| GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
			| GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_13
			| GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4
			| GPIO_PIN_5 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    FSMC_NORSRAM_TimingTypeDef Timing = {0};
    FSMC_NORSRAM_TimingTypeDef ExtTiming = {0};

	SRAM_LCD.Instance = FSMC_NORSRAM_DEVICE;
	SRAM_LCD.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	/* SRAM_LCD.Init */
	SRAM_LCD.Init.NSBank = FSMC_NORSRAM_BANK1;
	SRAM_LCD.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	SRAM_LCD.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	SRAM_LCD.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	SRAM_LCD.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	SRAM_LCD.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	SRAM_LCD.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	SRAM_LCD.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	SRAM_LCD.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	SRAM_LCD.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	SRAM_LCD.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	SRAM_LCD.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	SRAM_LCD.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	SRAM_LCD.Init.PageSize = FSMC_PAGE_SIZE_NONE;
	/* Timing */
	Timing.AddressSetupTime = 2;
    Timing.AddressHoldTime = 15;
    Timing.DataSetupTime = 4;
    Timing.BusTurnAroundDuration = 15;
    Timing.CLKDivision = 16;
    Timing.DataLatency = 17;
    Timing.AccessMode = FSMC_ACCESS_MODE_A;
    /* ExtTiming */
    ExtTiming.AddressSetupTime = 0;
    ExtTiming.AddressHoldTime = 15;
    ExtTiming.DataSetupTime = 3;
    ExtTiming.BusTurnAroundDuration = 15;
    ExtTiming.CLKDivision = 16;
    ExtTiming.DataLatency = 17;
    ExtTiming.AccessMode = FSMC_ACCESS_MODE_A;

    HAL_SRAM_Init(&SRAM_LCD , &Timing, &ExtTiming);

}

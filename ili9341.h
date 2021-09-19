/**  
* @file ili9341.h  
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

#ifndef _ILI9341_H_
#define _ILI9341_H_

#include "mbed.h"
#include "colors.h"
#include "fonts.h"
#include "image.h"
#include <cstdint>
#include <stdbool.h>

/**
 * @brief  NEx pin connected to CS
 *         The FSMC treats the Display as ERAM,
 *         so only bank 1 can be used to drive hin
 */
typedef enum {
	NE1 = (uint32_t)0x60000000, /* In STN32F407VE only NE1 can be used for display */
	NE2 = (uint32_t)0x64000000,
	NE3 = (uint32_t)0x68000000,
	NE4 = (uint32_t)0x6c000000
} NEx_t;

/**
 * @brief  Ax pin connected to RS
 *  Ax in HADDR register goes from 1 to 25,
 *  Ax must be dislocate one extra to left
 * */
typedef enum {
	A16 = (uint32_t)(2<<16), A17 = (uint32_t)(2<<17), A18 = (uint32_t)(2<<18), A19 = (uint32_t)(2<<19),
	A20 = (uint32_t)(2<<20), A21 = (uint32_t)(2<<21), A22 = (uint32_t)(2<<22), A23 = (uint32_t)(2<<23)
} Ax_t;

#ifdef swap
#undef swap
#endif
#define swap(a, b) { int16_t t = a; a = b; b = t; }

#define ILI9341_PIXEL_WIDTH		240
#define ILI9341_PIXEL_HEIGHT 	320

/**
 * @brief  Orientation for dispay
 */
typedef enum {
	LCD_ORIENTATION_PORTRAIT = 0,
	LCD_ORIENTATION_LANDSCAPE = 1,
	LCD_ORIENTATION_PORTRAIT_MIRROR = 2,
	LCD_ORIENTATION_LANDSCAPE_MIRROR = 3
} lcdOrientation_t;

/**
 * @brief  Draw Properties structures definition
 */
typedef struct {
	uint32_t TextColor;
	uint32_t BackColor;
	font_t *pFont;
	uint8_t TextWrap;
} lcdFontProperties_t;

/**
 * @brief  Cursor position structures definition
 */
typedef struct {
	uint16_t x;
	uint16_t y;
} lcdCursorPos_t;


/**
 * @brief  Properties structures to define resources to different displays
 */
typedef struct {
	uint16_t width;         //  width in pixels
	uint16_t height;        //  height in pixels
	lcdOrientation_t orientation;  // Whether the  orientation can be modified
	bool touchscreen;   // Whether the  has a touch screen
	bool hwscrolling;   // Whether the  support HW scrolling
} lcdProperties_t;

/**
 * @brief Create a object to display characteristic and address
 *
 * @param rst       Output where is connected RST pin of display
 * @param bl        Output where is connected Backlight pin of display
 * @param Ax       	Output where is connected RS pin of display
 * @param NEx      	Output where is connected CS pin of display
 * @param width    	Maximum width of display
 * @param height   	Maximum height of display
 * @param orientation  Initial orientation of display
 *
 * @return void
*/
class ILI9341 {
protected:

	volatile uint16_t* fsmcCMD;
	volatile uint16_t* fsmcDATA;

    uint8_t _bright = 0;

    mbed::DigitalOut _rst;
    mbed::PwmOut _bl;

	lcdProperties_t Properties = {
			ILI9341_PIXEL_WIDTH, ILI9341_PIXEL_HEIGHT,
			LCD_ORIENTATION_PORTRAIT, true, true
	};

	lcdFontProperties_t Font = {
			GREEN, BLACK, &Font12, 1
	};

	lcdCursorPos_t cursorXY = { 0, 0 };

	uint8_t PortraitConfig = 0;
	uint8_t LandscapeConfig = 0;
	uint8_t PortraitMirrorConfig = 0;
	uint8_t LandscapeMirrorConfig = 0;

	inline void writeCommand(uint8_t command) {*fsmcCMD = command;}
    inline void writeData(uint16_t data) {*fsmcDATA = data;}
    void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    inline uint16_t readData(void) {return *fsmcDATA;}
	
    virtual int _putc(int value);
    virtual int _getc();

public:

	/**
	 * @brief Create a object to display characteristic and address
	 *
     * @param rst       Output where is connected RST pin of display
     * @param bl        Output where is connected Backlight pin of display
	 * @param Ax       	Output where is connected RS pin of display
	 * @param NEx      	Output where is connected CS pin of display
	 * @param width    	Maximum width of display
	 * @param height   	Maximum height of display
	 * @param orientation  Initial orientation of display
	 *
	 * @return void
	 */
	ILI9341(PinName rst = NC, PinName bl = PB_1,
            Ax_t Ax = A18, NEx_t NEx = NE1,
			uint16_t width = ILI9341_PIXEL_WIDTH,
			uint16_t heigth = ILI9341_PIXEL_HEIGHT,
			lcdOrientation_t orientation = LCD_ORIENTATION_PORTRAIT);

	/**
	* @brief Start the display
	*
	* @return void
	*/
	void begin(void);

	/**
	* @brief Shows colored stripes in display
	*
	* @return void
	*/
	void test(void);

	/**
	* @brief Convert RGB code to 565 code supported by display
	*
	* @return void
	*/
	uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

	/**
	* @brief Fill the screen with a selected color
	*
	* @param color	Color to fill screen
	*
	* @return void
	*/
	void fillScreen(uint16_t color);

	/**
	* @brief Draw a pixel
	*
	* @param x		x-coordinate axis
	* @param y		y-coordinate axis
	* @param color	Color of pixel
	*
	* @return void
	*/
	void drawPixel(uint16_t x, uint16_t y, uint16_t color);
    void drawPixels(uint16_t x, uint16_t y, uint16_t *data,	uint32_t dataLength);

	/**
	* @brief Draw a line
	*
	* @param x0	    x-coordinate start
	* @param x1	    x-coordinate end
	* @param y0	    y-coordinate start
	* @param y1	    y-coordinate end
	* @param color	Color of line
	*
	* @return void
	*/
	void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

	/**
	* @brief Draw a horizontal line
	*
	* @param x	    x-coordinate
	* @param y		y-coordinate of line
    * @param width  Width of line
	* @param color	Color of line
	*
	* @return void
	*/
	void drawFastHLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color);

	/**
	* @brief Draw a vertical line
	*
	* @param x		 x-coordinate
	* @param y	     y-coordinate
	* @param height	 Height of line
	* @param color	 Color of line
	*
	* @return void
	*/
	void drawFastVLine(uint16_t x, uint16_t y, uint16_t height, uint16_t color);

	/**
	* @brief Draw a rectangle
	* 		 It could be a square. You know, if the sides be equals.
	*
	* @param x		x-coordinate axis
	* @param y		y-coordinate axis
	* @param w		Width of rectangle, or a square if the width and height are equals
	* @param h		Height of rectangle, or square
	* @param color	Color of rectangle, maybe a square
	*
	* @return void
	*/
	void drawRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);

	/**
	* @brief Draw filled a rectangle
	* 		 It could be a square, but you know this
	*
	* @param x		x-coordinate axis
	* @param y		y-coordinate axis
	* @param width	Width of rectangle
	* @param height	Height of rectangle
	* @param color	Color of rectangle... or a square
	*
	* @return void
	*/
	void fillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t fillcolor);

	/**
	* @brief Draw a rectangle with no sharps
	* 		 Safe for kids
	*
	* @param x		x-coordinate axis
	* @param y		y-coordinate axis
	* @param width	Width of rectangle
	* @param height	Height of rectangle
	* @param radius	Radius of the round, maybe a high radius can make this became a circle
	* @param color	Color of  rounded rectangle or a square i don't know
	*
	* @return void
	*/
	void drawRoundRect(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius, uint16_t color);

	/**
	* @brief Draw a filed rectangle with no sharps
	*
	* @param x		x-coordinate axis
	* @param y		y-coordinate axis
	* @param w		Width of rectangle
    * @param height	Height of rectangle
	* @param r		Radius of the round
	* @param color	Color of rectangle or a square or a circle, i don't care
	*
	* @return void
	*/
	void fillRoundRect(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius, uint16_t color);

	/**
	 * brief Draw a circle. Wooow, how could I imagine this?!
	 *
	 * @param x0		x-coordinate
	 * @param y0		y-coordinate
	 * @param radius    Radius
	 * @param color	    Color
	 *
	 * @return void
	 */
	void drawCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color);

	/**
	 * @brief I'll leave this one for you
	 *
	 */
	void fillCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color);

	/**
	 * @brief Helper function to draw a circle, don't ask me what this means
	 *
	 * @param x0			x-coordinate
	 * @param y0			y-coordinate
	 * @param radius	    Radius
	 * @param cornername	Corner (1, 2, 3, 4)
	 * @param delta		    Delta
	 * @param color		    Color
	 *
	 * @return void
	 */
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t radius, uint8_t cornername, uint16_t color);

	/**
	 * @brief Helper function to draw a filled circle
	 *
	 * @param x0			x-coordinate
	 * @param y0			y-coordinate
	 * @param radius	    Radius
	 * @param cornername	Corner (1, 2, 3, 4)
	 * @param delta		    Delta
	 * @param color		    Color
	 *
	 * @return void
	*/
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t radius, uint8_t cornername, int16_t delta, uint16_t color);

	/**
	 * @brief Draw a triangle
	 * @param x0  	Vertex #0 x coordinate
	 * @param y0  	Vertex #0 y coordinate
	 * @param x1  	Vertex #1 x coordinate
	 * @param y1  	Vertex #1 y coordinate
	 * @param x2  	Vertex #2 x coordinate
	 * @param y2    Vertex #2 y coordinate
	 * @param color 16-bit 5-6-5 Color to draw with
	 *
	 * @return void
	*/
	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

	/**
	 * @brief Draw a filled triangle
	 * @param x0  	 Vertex #0 x coordinate
	 * @param y0  	 Vertex #0 y coordinate
	 * @param x1  	 Vertex #1 x coordinate
	 * @param y1  	 Vertex #1 y coordinate
	 * @param x2  	 Vertex #2 x coordinate
	 * @param y2     Vertex #2 y coordinate
	 * @param color  Color
	 *
	 * @return void
	*/
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

	/**
	 * @brief Draw bitmap image
	 * @param x  	 	Vertex #0 x coordinate
	 * @param y  	 	Vertex #0 y coordinate
	 * @param pBitmap   A pointer to sImage_t type of image
	 *
	 * @return void
	*/
	void drawImage(uint16_t x, uint16_t y, const sImage_t *pBitmap);

	/**
	 * @brief Draw a character at the specified coordinates
	 *
	 * @param x		    x-coordinate
	 * @param y		    y-coordinate
	 * @param c		    Character
	 * @param color	    Character color
	 * @param bg		Background color
	 * @param size		Character size
	 *
	 * @return void
	 */
	void drawChar(int16_t x, int16_t y, uint8_t c, uint16_t color, uint16_t bg);
    void printf(const char *fmt, ...);
    void clrLine(uint16_t bg);
    void clrLine();

	/**
	 * @brief Sets the font
	 *
	 * @param font 	pointer font_t type font
	 *
	 * @return void
	 */
	void setTextFont(font_t *font);

	/**
	 * @brief Sets the text color
	 *
	 * @param textColor		Text color
	 * @param bgColor		Background color
	 *
	 * @return void
	 */
	void setTextColor(uint16_t textColor, uint16_t bgColor);

	/**
	 * @brief Set Text wrap. Sorry kit, i don't know what rec this does
	 *
	 * @param warp
	 *
	 * @return void
	 */
	void setTextWrap(uint8_t wrap);

	/**
	 * @brief Set the orientation of display
	 *
	 * @param orientation	Orientation of display type lcdOrientation_t
	 *
	 * @return void
	*/
	void setOrientation(lcdOrientation_t orientation);

	/**
	 * @brief Set cursor to initial position (x = 0, y = 0)
	 *
	 * @return void
	*/
	void home(void);

	/**
	 * @brief Set cursor position
	 *
	 * @param x		x-coordinate
	 * @param y		y-coordinate
	 *
	 * @return void
	*/
	void setCursor(uint16_t x, uint16_t y);
    uint16_t getColumn(uint16_t coll);
    uint16_t getLine(uint16_t line);

    /**
	 * @brief Set the brigth of display
     *        in black_f4ve this is a pwm pin out
	 *
     * @param  bright   The bright or screen, go from 0 to 100
     *
	 * @return void
	*/
	void backlightBright(uint8_t bright);
    uint8_t backlightBright();

	void inversionOff(void);
	void inversionOn(void);

    /**
	 * @brief Turn off the display
     *
	 * @return void
	*/
	void displayOff(void);

    /**
	 * @brief Turn on the display
     *
	 * @return void
	*/
	void displayOn(void);

	void tearingOff(void);
	void tearingOn(bool state);

    /**
	 * @brief Get max width of screen
     *
	 * @return uint16_t
	*/
	uint16_t getWidth(void);

    /**
	 * @brief Get max height of screen
     *
	 * @return uint16_t
	*/
	uint16_t getHeight(void);

    /**
	 * @brief Get the current orientation of screen
     *
	 * @return lcdOrientation_t
	*/
	lcdOrientation_t GetOrientation(void);

    /**
	 * @brief Get the controller ID
     *        Guess what, if this don't return 0x9341 the things will go crazy
     *
	 * @return uint16_t
	*/
	uint16_t getControllerID(void);

    /**
	 * @brief Get the current font
     *
	 * @return font_t*
	*/
	font_t* getTextFont(void);

    /**
	 * @brief Get all proprieties of display
     *
	 * @return font_t*
	*/
	lcdProperties_t getProperties(void);

    /**
	 * @brief Read the pixel color
     *
     * @param x     x-coordinate
     * @param y     y-coordinate
     *
	 * @return uint16_t
	*/
	uint16_t readPixel(uint16_t x, uint16_t y);

    /**
	 * @brief Reset the display
     *
	 * @return void
	*/
	void reset(void);

};

#endif  /* _ILI9341_H_ */

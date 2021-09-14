#include "mbed.h"
#include "ili9341.h"

#define min(a,b) (((a)<(b))?(a):(b))

ILI9341 display;
Timer timer;

void demoLCD();
uint32_t testFillScreen();
uint32_t testText();
uint32_t testLines(uint16_t color);
uint32_t testFastLines(uint16_t color1, uint16_t color2);
uint32_t testRects(uint16_t color);
uint32_t testFilledRects(uint16_t color1, uint16_t color2);
uint32_t testFilledCircles(uint8_t radius, uint16_t color);
uint32_t testCircles(uint8_t radius, uint16_t color);
uint32_t testTriangles();
uint32_t testFilledTriangles();
uint32_t testRoundRects();
uint32_t testFilledRoundRects();
uint32_t testDrawImage();

int main() {
    display.begin();
    while (1)
    {
        demoLCD();
    }
    return 0;
}

typedef struct {
    uint32_t fillScreen;
    uint32_t colorStripes;
    uint32_t text;
    uint32_t lines;
    uint32_t fastLines;
    uint32_t drawRects;
    uint32_t fillRects;
    uint32_t drawCircles;
    uint32_t fillCircles;
    uint32_t drawTriangles;
    uint32_t fillTriangles;
    uint32_t drawRoundRects;
    uint32_t fillRoundRects;
    uint32_t drawImage;
} test_t;

void demoLCD()
{
    test_t t; uint32_t total;
    display.setOrientation(LCD_ORIENTATION_PORTRAIT);
    timer.start();
    total = t.fillScreen = testFillScreen();

    timer.reset();
    display.test();
    total += t.colorStripes = chrono::duration_cast<chrono::milliseconds>(timer.elapsed_time()).count();
    total += t.text = testText();
    total += t.lines = testLines(CYAN);
    total += t.fastLines = testFastLines(RED, BLUE);
    total += t.drawRects = testRects(GREEN);
    total += t.fillRect = testFilledRects(YELLOW, MAGENTA);
    total += t.fillCircles = testFilledCircles(10, MAGENTA);
    total += t.drawCircles = testCircles(10, WHITE);
    total += t.drawTriangles = testTriangles();
    total += t.fillTriangles = testFilledTriangles();
    total += t.drawRoundRects = testRoundRects();
    total += t.fillRoundRects = testFilledRoundRects();
    total += t.drawImage = testDrawImage();
    timer.stop();

    uint32_t id = display.getControllerID();
    display.fillScreen(BLACK);
    display.setTextColor(GREEN, BLACK);
    display.home();
    display.setTextFont(&Font12);
    display.printf("Display controller: ILI%#04x\r\n\n", id);
    display.printf("fillScreen       %03d ms\r\n", t.fillScreen);
    display.printf("colorStripes     %03d ms\r\n", t.colorStripes);
    display.printf("text             %03d ms\r\n", t.text);
    display.printf("lines            %03d ms\r\n", t.lines);
    display.printf("fastLines        %03d ms\r\n", t.fastLines);
    display.printf("rects            %03d ms\r\n", t.drawRects);
    display.printf("fillRects        %03d ms\r\n", t.fillRects);
    display.printf("circles          %03d ms\r\n", t.drawCircles);
    display.printf("fillCircles      %03d ms\r\n", t.fillCircles);
    display.printf("triangles        %03d ms\r\n", t.drawTriangles);
    display.printf("fillTriangles    %03d ms\r\n", t.fillTriangles);
    display.printf("roundRects       %03d ms\r\n", t.drawRoundRects);
    display.printf("fillRoundRects   %03d ms\r\n", t.fillRoundRects);
    display.printf("Image            %03d ms\r\n", t.drawImage);
    display.printf("TOTAL            %03d ms\r\n", total);
    display.setTextColor(RED, BLACK);
    display.printf("\nEnd test, reset to other run.\r\n");
    while(true);
}

uint32_t testFillScreen()
{
    uint32_t t = 0;
    timer.reset();
    display.fillScreen(BLACK);
    t += timer.elapsed_time().count();
    ThisThread::sleep_for(1s);
    timer.reset();
    display.fillScreen(RED);
    t += timer.elapsed_time().count();
    ThisThread::sleep_for(1s);
    timer.reset();
    display.fillScreen(GREEN);
    t += timer.elapsed_time().count();
    ThisThread::sleep_for(1s);
    timer.reset();
    display.fillScreen(BLUE);
    t += timer.elapsed_time().count();
    ThisThread::sleep_for(1s);
    timer.reset();
    display.fillScreen(YELLOW);
    t += timer.elapsed_time().count();
    ThisThread::sleep_for(1s);
    return t/1000;
}

uint32_t testText()
{
    display.fillScreen(BLACK);
    timer.reset();
    display.setCursor(0, 0);
    display.setTextColor(WHITE, BLACK);
    display.setTextFont(&Font8);
    display.printf("Hello World!\r\n");
    display.setTextColor(YELLOW, BLACK);
    display.setTextFont(&Font12);
    display.printf("%i\r\n", 1234567890);
    display.setTextColor(RED, BLACK);
    display.setTextFont(&Font16);
    display.printf("%#X\r\n", 0xDEADBEEF);
    display.printf("\r\n");
    display.setTextColor(GREEN, BLACK);
    display.setTextFont(&Font20);
    display.printf("Groop\r\n");
    display.setTextFont(&Font12);
    display.printf("I implore thee,\r\n");
    display.setTextFont(&Font12);
    display.printf("my foonting turlingdromes.\r\n");
    display.printf("And hooptiously drangle me\r\n");
    display.printf("with crinkly bindlewurdles,\r\n");
    display.printf("Or I will rend thee\r\n");
    display.printf("in the gobberwarts\r\n");
    display.printf("with my blurglecruncheon,\r\n");
    display.printf("see if I don't!\r\n");
    uint32_t t = chrono::duration_cast<chrono::milliseconds>(timer.elapsed_time()).count();
    ThisThread::sleep_for(3s);
    return t;
}

uint32_t testLines(uint16_t color)
{
    uint32_t t = 0;
    int     x1, y1, x2, y2,
            w = display.getWidth(),
            h = display.getHeight();
    display.fillScreen(BLACK);
    x1 = y1 = 0;
    y2    = h - 1;
    timer.reset();
    for(x2 = 0; x2 < w; x2 += 6) display.drawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for(y2 = 0; y2 < h; y2 += 6) display.drawLine(x1, y1, x2, y2, color);
    t += timer.elapsed_time().count();
    ThisThread::sleep_for(1s);
    display.fillScreen(BLACK);
    x1 = w - 1;
    y1 = 0;
    y2 = h - 1;
    timer.reset();
    for(x2 = 0; x2 < w; x2 += 6) display.drawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for(y2 = 0; y2 < h; y2 += 6) display.drawLine(x1, y1, x2, y2, color);
    t += timer.elapsed_time().count();
    ThisThread::sleep_for(1s);
    display.fillScreen(BLACK);
    x1 = 0;
    y1 = h - 1;
    y2 = 0;
    timer.reset();
    for(x2 = 0; x2 < w; x2 += 6) display.drawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for(y2 = 0; y2 < h; y2 += 6) display.drawLine(x1, y1, x2, y2, color);
    t += timer.elapsed_time().count();
    ThisThread::sleep_for(1s);
    display.fillScreen(BLACK);
    x1 = w - 1;
    y1 = h - 1;
    y2 = 0;
    timer.reset();
    for(x2 = 0; x2 < w; x2 += 6) display.drawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for(y2 = 0; y2 < h; y2 += 6) display.drawLine(x1, y1, x2, y2, color);
    t += timer.elapsed_time().count();
    ThisThread::sleep_for(1s);
    return t/1000;
}

uint32_t testFastLines(uint16_t color1, uint16_t color2)
{
    int x, y, w = display.getWidth(), h = display.getHeight();
    display.fillScreen(BLACK);
    timer.reset();
    for(y = 0; y < h; y += 5) display.drawFastHLine(0, y, w, color1);
    for(x = 0; x < w; x += 5) display.drawFastVLine(x, 0, h, color2);
    uint32_t t = chrono::duration_cast<chrono::milliseconds>(timer.elapsed_time()).count();
    ThisThread::sleep_for(1s);
    return t;
}

uint32_t testRects(uint16_t color)
{
    int n, i, i2,
        cx = display.getWidth()  / 2,
        cy = display.getHeight() / 2;
    display.fillScreen(BLACK);
    n = min(display.getWidth(), display.getHeight());
    timer.reset();
    for(i = 2; i < n; i += 6)
    {
        i2 = i / 2;
        display.drawRect(cx - i2, cy - i2, i, i, color);
    }
    uint32_t t = chrono::duration_cast<chrono::milliseconds>(timer.elapsed_time()).count();
    ThisThread::sleep_for(1s);
    return t;
}

uint32_t testFilledRects(uint16_t color1, uint16_t color2)
{
    uint32_t t = 0;
    int n, i, i2,
        cx = display.getWidth() / 2 - 1,
        cy = display.getHeight() / 2 - 1;
    display.fillScreen(BLACK);
    n = min(display.getWidth(), display.getHeight());
    for(i = n; i > 0; i -= 6)
    {
        i2 = i / 2;
        timer.reset();
        display.fillRect(cx-i2, cy-i2, i, i, color1);
        t += timer.elapsed_time().count();
        // Outlines are not included in timing results
        display.drawRect(cx-i2, cy-i2, i, i, color1);
    }
    ThisThread::sleep_for(1s);
    return t/1000;
}

uint32_t testFilledCircles(uint8_t radius, uint16_t color)
{
    int x, y, w = display.getWidth(), h = display.getHeight(), r2 = radius * 2;
    display.fillScreen(BLACK);
    timer.reset();
    for(x = radius; x < w; x += r2)
    {
        for(y = radius; y < h; y += r2)
        {
        display.fillCircle(x, y, radius, color);
        }
    }
    uint32_t t = chrono::duration_cast<chrono::milliseconds>(timer.elapsed_time()).count();
    ThisThread::sleep_for(1s);
    return t;
}

uint32_t testCircles(uint8_t radius, uint16_t color)
{
    int x, y, r2 = radius * 2,
        w = display.getWidth()  + radius,
        h = display.getHeight() + radius;
    // Screen is not cleared for this one -- this is
    // intentional and does not affect the reported time.
    timer.reset();
    for(x = 0; x < w; x += r2)
    {
        for(y = 0; y < h; y += r2)
        {
        display.drawCircle(x, y, radius, color);
        }
    }
    uint32_t t = chrono::duration_cast<chrono::milliseconds>(timer.elapsed_time()).count();
    ThisThread::sleep_for(1s);
    return t;
}

uint32_t testTriangles()
{
    int n, i, cx = display.getWidth() / 2 - 1,
                cy = display.getHeight() / 2 - 1;
    display.fillScreen(BLACK);
    n = min(cx, cy);
    timer.reset();
    for(i = 0; i < n; i += 5)
    {
        display.drawTriangle(
        cx    , cy - i, // peak
        cx - i, cy + i, // bottom left
        cx + i, cy + i, // bottom right
        display.color565(i, i, i));
    }
    uint32_t t = chrono::duration_cast<chrono::milliseconds>(timer.elapsed_time()).count();
    ThisThread::sleep_for(1s);
    return t;
}

uint32_t testFilledTriangles()
{
    uint32_t t = 0;
    int i, cx = display.getWidth() / 2 - 1,
            cy = display.getHeight() / 2 - 1;
    display.fillScreen(BLACK);
    for(i = min(cx, cy); i > 10; i -= 5)
    {
        timer.reset();
        display.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, display.color565(0, i*10, i*10));
        t += timer.elapsed_time().count();
        display.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, display.color565(i*10, i*10, 0));
    }
    ThisThread::sleep_for(1s);
    return t/1000;
}

uint32_t testRoundRects()
{
    int w, i, i2,
        cx = display.getWidth() / 2 - 1,
        cy = display.getHeight() / 2 - 1;
    display.fillScreen(BLACK);
    w = display.getWidth(), display.getHeight();
    timer.reset();
    for(i = 0; i < w; i += 6)
    {
        i2 = i / 2;
        display.drawRoundRect(cx-i2, cy-i2, i, i, i/8, display.color565(i, 0, 0));
    }
    uint32_t t = chrono::duration_cast<chrono::milliseconds>(timer.elapsed_time()).count();
    ThisThread::sleep_for(1s);
    return t;
}

uint32_t testFilledRoundRects()
{
    int i, i2,
        cx = display.getWidth()  / 2 - 1,
        cy = display.getHeight() / 2 - 1;
    display.fillScreen(BLACK);
    timer.reset();
    for(i = min(display.getWidth(), display.getHeight()); i > 20; i -=6 )
    {
        i2 = i / 2;
        display.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, display.color565(0, i, 0));
    }
    uint32_t t = chrono::duration_cast<chrono::milliseconds>(timer.elapsed_time()).count();
    ThisThread::sleep_for(1s);
    return t;
}

uint32_t testDrawImage()
{
    display.fillScreen(BLACK);
    timer.reset();
    if (display.GetOrientation() == LCD_ORIENTATION_LANDSCAPE || display.GetOrientation() == LCD_ORIENTATION_LANDSCAPE_MIRROR)
    {
        display.drawImage((display.getWidth() - bmSTLogo.xSize) / 2, 0, &bmSTLogo);
    }
    else
    {
        display.drawImage(0, (display.getHeight() - bmSTLogo.ySize) / 2, &bmSTLogo);
    }
    uint32_t t = chrono::duration_cast<chrono::milliseconds>(timer.elapsed_time()).count();
    ThisThread::sleep_for(3s);
    return t;
}

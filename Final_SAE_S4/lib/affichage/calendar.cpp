#include "widgets.h"

void Calendar(GFXcanvas16 *pcanvas, uint16_t screenWidth, uint16_t screenHeight, String day, uint8_t num, String month)
{
    pcanvas->fillScreen(BLACK);
    pcanvas->drawBitmap(0.25 * screenWidth, 0.05 * screenHeight, bitmap_calendrier_64, 64, 64, WHITE);
    pcanvas->setTextSize(2);
    pcanvas->setCursor(0.1 * screenWidth, 0.65 * screenHeight);
    pcanvas->print(day);
    pcanvas->print(":");
    pcanvas->print(num);
    pcanvas->print(":");
    pcanvas->print(month);
}
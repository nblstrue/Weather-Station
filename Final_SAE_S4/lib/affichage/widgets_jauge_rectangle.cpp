#include <widgets.h>

JaugeRectangle::JaugeRectangle(GFXcanvas16 *pcanvas, uint16_t screenWidth, uint16_t screenHeight, uint16_t min, uint16_t max, String unite)
{
    _canvas = pcanvas;
    _width = screenWidth;
    _height = screenHeight;
    _x0 = 0.05 * _width;
    _y0 = 0.1 * _height;
    _min = min;
    _max = max;
    _unite = unite;
}

void JaugeRectangle::update(uint16_t valeur)
{
    _valeur = valeur;
}

void JaugeRectangle::draw()
{
    _drawBackground();
    _drawJauge();
}

void JaugeRectangle::_drawBackground()
{
    _canvas->fillScreen(BLACK);
    _canvas->drawRect(_x0, _y0, 0.9 * _width, 2 * _y0, WHITE);

    for (int i = 0; i < 9; i++)
        _canvas->fillRect(_x0 + i * 0.1 * _width, _y0 - 3, 1, 6, WHITE);
    _canvas->fillRect(_x0 + 0.9 * _width - 1, _y0 - 3, 1, 6, WHITE);

    _canvas->setTextSize(1);
    _canvas->setCursor(0.9 * _x0, 0.15 * _y0);
    _canvas->print(_min);
    _canvas->setCursor(0.8 * _width, 0.15 * _y0);
    _canvas->print(_max); // Mieux faire

    _canvas->drawBitmap(_x0, 0.45 * _height, bitmap_idee_ampoule_32, 32, 32, WHITE); // modifier pour pouvoir choisi quel image on veut
}

void JaugeRectangle::_drawJauge()
{
    _canvas->fillRect(_x0, _y0, 0.9 * _width * _valeur / _max, 2 * _y0, WHITE);

    _canvas->setTextSize(2);
    _canvas->setCursor(0.35 * _width, 0.55 * _height);
    _canvas->print(_valeur);
    _canvas->setCursor(0.7 * _width, 0.55 * _height);
    _canvas->print(_unite);
}

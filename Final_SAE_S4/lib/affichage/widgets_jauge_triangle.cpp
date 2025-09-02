#include <widgets.h>

JaugeTriangle::JaugeTriangle(GFXcanvas16 *pcanvas, uint16_t screenWidth, uint16_t screenHeight, uint16_t min, uint16_t max, String unite)
{
    _canvas = pcanvas;
    _width = screenWidth;
    _height = screenHeight;
    _x0 = 0.05 * _width;
    _y0 = 0.3 * _height;
    _min = min;
    _max = max;
    _unite = unite;
}

void JaugeTriangle::update(uint32_t valeur)
{
    _valeur = valeur;
}

void JaugeTriangle::draw()
{
    if (_valeur > _max)
        _drawSaturation();
    else
    {
        _drawBackground();
        _drawJauge();
    }
}

void JaugeTriangle::_drawBackground() // il faudrait décaler le 0 d'un cran
{
    _canvas->fillScreen(BLACK);
    _canvas->drawTriangle(_x0, _y0, _x0 + 0.85 * _width, _y0, _x0 + 0.85 * _width, _y0 - 0.2 * _height, WHITE);

    for (int i = 1; i < 10; i++)
        _canvas->drawLine(_x0 + 0.85 * _width * i / 10, _y0 - 2, _x0 + 0.85 * _width * i / 10, _y0 + 2, WHITE);
    _canvas->drawLine(_x0 + 0.85 * _width, _y0 - 3, _x0 + 0.85 * _width, _y0 + 3, WHITE);

    _canvas->setTextSize(1);
    _canvas->setCursor(0.03 * _width, 1.2 * _y0);
    _canvas->print(_min);
    _canvas->setCursor(0.8 * _width, 1.2 * _y0);
    _canvas->print(_max);
}

void JaugeTriangle::_drawJauge()
{
    _canvas->fillTriangle(_x0, _y0, _x0 + 0.85 * _width * _valeur / _max, _y0, _x0 + 0.85 * _width * _valeur / _max, _y0 - 0.2 * _height * _valeur / _max, WHITE);

    _canvas->setTextSize(2);
    _canvas->setCursor(0.28 * _width, 0.55 * _height);
    _canvas->print(_valeur);
    _canvas->setCursor(0.7 * _width, 0.55 * _height);
    _canvas->print(_unite);
}

void JaugeTriangle::_drawSaturation()
{
    _canvas->fillScreen(BLACK);
    _canvas->setTextSize(2);
    _canvas->setCursor(0.25 * _width, 0.25 * _height);
    _canvas->print("Capteur sature");
}
#include <widgets.h>

JaugeAiguille::JaugeAiguille(GFXcanvas16 *pcanvas, uint16_t screenWidth, uint16_t screenHeight, String unite)
{
    _canvas = pcanvas;
    _width = screenWidth;
    _height = screenHeight;
    _centerRadius = _width / 2;
    _base = 0.6 * _height;
    _unite = unite;
}

void JaugeAiguille::update(float valeur)
{
    _valeur = valeur;
}

void JaugeAiguille::draw()
{
    _drawBackground();
    _drawJauge();
}

void JaugeAiguille::_drawBackground()
{
    _canvas->fillScreen(BLACK);
    _canvas->fillCircle(_centerRadius, _base, _centerRadius * 0.08, WHITE);

    _canvas->setTextSize(1);
    _canvas->setCursor(0, _base * 0.95);
    _canvas->print("0");
    _canvas->setCursor(_centerRadius + 0.8 * _centerRadius - 5, _base * 0.95);
    _canvas->print("100");

    for (int i = 1; i < 10; i++)
        _canvas->drawPixel(_centerRadius - _centerRadius * cos(i * PI / 10), _base - _centerRadius * sin(i * PI / 10), WHITE);
}

void JaugeAiguille::_drawJauge()
{
    float ptTemp = 2 * PI * float(_valeur) / 200 - PI;
    float p0 = ptTemp + PI / 2;
    float p1 = ptTemp - PI / 2;

    _canvas->drawLine(_centerRadius + 2 * cos(p0), _base + 2 * sin(p0), _centerRadius + _centerRadius * 0.9 * cos(ptTemp), _base + _centerRadius * 0.9 * sin(ptTemp), WHITE);
    _canvas->drawLine(_centerRadius + 2 * cos(p1), _base + 2 * sin(p1), _centerRadius + _centerRadius * 0.9 * cos(ptTemp), _base + _centerRadius * 0.9 * sin(ptTemp), WHITE);

    _canvas->setTextSize(2);
    _canvas->setCursor(0.45 * _centerRadius, _base * 1.25);
    _canvas->print(_valeur);
    _canvas->setCursor(1.45 * _centerRadius, _base * 1.25);
    _canvas->print(_unite);
}
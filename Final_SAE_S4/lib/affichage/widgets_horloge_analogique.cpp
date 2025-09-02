#include <widgets.h>

#define TAILLE_HEURE 0.5
#define TAILLE_MINUTE 0.75
#define TAILLE_SECONDE 0.95

AnaClock::AnaClock(GFXcanvas16 *pcanvas, uint16_t screenWidth, uint16_t screenHeight)
{
    _canvas = pcanvas;
    _width = screenWidth;
    _height = screenHeight;
    _centerX = _width / 2;
    _centerY = _height / 2;
    _radius = 0.35 * _width;
}

void AnaClock::update(uint8_t hour, uint8_t minute, uint8_t second)
{
    _hour = hour;
    _minute = minute;
    _second = second;
}

void AnaClock::draw()
{
    _drawBackground();
    _drawHour();
    _drawMinute();
    _drawSecond();
}

void AnaClock::_drawBackground()
{
    _canvas->fillScreen(BLACK);
    _canvas->fillCircle(_centerX, _centerY, 0.01 * _width, WHITE);
    _canvas->setTextSize(1);

    for (int i = 0; i < 12; i++)
    {
        if (i == 0 || i == 6)
        {
            _canvas->setCursor(_centerX + cos((i / 3) * (PI / 2)) * _radius, _centerY + sin((i / 3) * (PI / 2)) * _radius - 3);
            _canvas->print(i + 3);
        }
        else if (i == 3 || i == 9)
        {
            _canvas->setCursor(_centerX + cos((i / 3) * (PI / 2)) * _radius - 6, _centerY + sin((i / 3) * (PI / 2)) * _radius - 1);
            _canvas->print(i + 3);
        }
        else
            _canvas->drawPixel(_centerX + _radius * cos(i * PI / 6), _centerY + _radius * sin(i * PI / 6), WHITE);
    }
}

void AnaClock::_drawHour()
{
    float ptHour = 2 * PI * int(_hour) / 12 - PI / 2;
    float p0 = ptHour + PI / 2;
    float p1 = ptHour - PI / 2;

    _canvas->drawLine(_centerX + 1 * cos(p0), _centerY + 1 * sin(p0), _centerX + _radius * TAILLE_HEURE * cos(ptHour), _centerY + _radius * TAILLE_HEURE * sin(ptHour), WHITE); // Aiguille des heures
    _canvas->drawLine(_centerX + 1 * cos(p1), _centerY + 1 * sin(p1), _centerX + _radius * TAILLE_HEURE * cos(ptHour), _centerY + _radius * TAILLE_HEURE * sin(ptHour), WHITE);
}

void AnaClock::_drawMinute()
{
    float ptMinute = 2 * PI * int(_minute) / 60 - PI / 2;
    float p0 = ptMinute + PI / 2;
    float p1 = ptMinute - PI / 2;

    _canvas->drawLine(_centerX + 2 * cos(p0), _centerY + 2 * sin(p0), _centerX + float(_radius * TAILLE_MINUTE) * cos(ptMinute), _centerY + float(_radius * TAILLE_MINUTE) * sin(ptMinute), WHITE); // Aiguille des heures
    _canvas->drawLine(_centerX + 2 * cos(p1), _centerY + 2 * sin(p1), _centerX + float(_radius * TAILLE_MINUTE) * cos(ptMinute), _centerY + float(_radius * TAILLE_MINUTE) * sin(ptMinute), WHITE);
}

void AnaClock::_drawSecond()
{
    float angleSeconde = 2 * PI * int(_second) / 60 - PI / 2;

    _canvas->drawLine(_centerX, _centerY, _centerX + float(_radius * TAILLE_SECONDE) * cos(angleSeconde), _centerY + float(_radius * TAILLE_SECONDE) * sin(angleSeconde), WHITE); // Aiguille
}
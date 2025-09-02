#ifndef widgets
#define widgets

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <Adafruit_BusIO_Register.h>

#include "images.h"

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

void Calendar(GFXcanvas16 *pcanvas, uint16_t screenWidth, uint16_t screenHeight, String day, uint8_t num, String month);

class AnaClock
{
public:
    AnaClock(GFXcanvas16 *pcanvas, uint16_t screenWidth, uint16_t screenHeight);
    void update(uint8_t hour, uint8_t minute, uint8_t second);
    void draw();

private:
    void _drawBackground();
    void _drawHour();
    void _drawMinute();
    void _drawSecond();
    uint16_t _width;
    uint16_t _height;
    uint16_t _centerX;
    uint16_t _centerY;
    uint16_t _radius;
    float _hour;
    float _minute;
    float _second;
    GFXcanvas16 *_canvas;
};

class JaugeAiguille
{
public:
    JaugeAiguille(GFXcanvas16 *pcanvas, uint16_t screenWidth, uint16_t screenHeight, String unite);
    void update(float valeur);
    void draw();

private:
    void _drawBackground();
    void _drawJauge();
    float _valeur;
    uint16_t _width;
    uint16_t _height;
    uint16_t _centerRadius;
    uint16_t _base;
    String _unite;
    GFXcanvas16 *_canvas;
};

class JaugeRectangle // pouvoir changer le bitmap
{
public:
    JaugeRectangle(GFXcanvas16 *pcanvas, uint16_t screenWidth, uint16_t screenHeight, uint16_t min, uint16_t max, String unite);
    void update(uint16_t valeur);
    void draw();

private:
    void _drawBackground();
    void _drawJauge();
    void _drawSaturation();
    uint16_t _valeur;
    uint16_t _width;
    uint16_t _height;
    uint16_t _x0;
    uint16_t _y0;
    uint16_t _min;
    uint16_t _max;
    String _unite;
    GFXcanvas16 *_canvas;
};

class JaugeTriangle
{
public:
    JaugeTriangle(GFXcanvas16 *pcanvas, uint16_t screenWidth, uint16_t screenHeight, uint16_t min, uint16_t max, String unite);
    void update(uint32_t valeur);
    void draw();

private:
    void _drawBackground();
    void _drawJauge();
    void _drawSaturation();
    uint32_t _valeur;
    uint16_t _width;
    uint16_t _height;
    uint16_t _x0;
    uint16_t _y0;
    uint16_t _min;
    uint16_t _max;
    String _unite;
    GFXcanvas16 *_canvas;
};

// &Canvas, WIDTH / 2, HEIGHT * 0.7, HEIGHT * 0.7, "C"                      // Données pour afficher un widget aiguille
// &Canvas, 0.05 * WIDTH, 0.25 * HEIGHT, 0.9 * WIDTH, 0.225 * HEIGHT, "C"   // Données pour afficher un widget rectangle
// &Canvas, 0.05 * WIDTH, 0.4 * HEIGHT, 0.9 * WIDTH, 0.3 * HEIGHT, "Bar"    // Données pour afficher un widget triangle

#endif
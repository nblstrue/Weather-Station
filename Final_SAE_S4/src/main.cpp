#include <Arduino.h>
#include <SPI.h>
#include <Ticker.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "ds1621.h"
#include "rtc3231.h"
#include "widgets.h"
#include "tsl2591.h"
#include "Adafruit_BME680.h"

#define MONITOR_SPEED 115200
#define LED_WIFI_PIN A0
#define LED_WEB_PIN A1
#define SERVO_PIN 21
#define DELAY 1000
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

#define ADRESSE_TEMPERATURE 0x49
#define ADRESSE_TEMPS 0x68
#define ADRESSE_LUMIERE 0x29
#define ADRESSE_ECRAN 0x3C
#define ADRESS_BME680_I2C 0x76
#define ADRESS_BME680_SPI 0x81

#define SCLK_PIN 5
#define MOSI_PIN 18
#define DC_PIN 14
#define CS_PIN 33
#define RST_PIN 32

/*const char ssid[] = "Bbox-4F4DBAC4";
const char pwd[] = "hRTvZUKfa45wNU1qw7";*/
/*const char ssid[] = "Bbox-451FD75D";
const char pwd[] = "rCL45nb39Zn3ZxF6V4";*/

const char ssid[] = "Linksys01370";
const char pwd[] = "3fanq5w4pb";

const char *PARAM_BUTTON = "state";
const char *PARAM_VALUE = "slider_value";
const char *PARAM_CITY = "cityName";
const String API_KEY = "&appid=4ed4771a297b8f8173a1f45d7da47789";
const String URL = "http://api.openweathermap.org/data/2.5/weather?q=";
const String UNITS = "&units=metric";
const String LANG = "&lang=fr";

String City = "Paris,FR";
float gtemperature, glumiere, gpression;
uint32_t ghumidite;
const char *weather_0_description;
const char *weather_0_icon;
float main_temp;

void action();
void affichage();
void setRoutes();
void connect(WiFiEvent_t event, WiFiEventInfo_t info);
void disconnect(WiFiEvent_t event, WiFiEventInfo_t info);
void got_Ip(WiFiEvent_t event, WiFiEventInfo_t info);
String getDataFromAPI();
bool initSPIFFS();
String createJson();
String createCityJson();
String httpGetRequest(const char *url);

Rtc3231 Temps;
TSL2591 Lumiere;
Ds1621 Temperature;
Adafruit_BME680 HumiPress;

Servo servoMoteur;
AsyncWebServer server(80);
Ticker timer(action, DELAY, 0, MILLIS);
Adafruit_SSD1351 Oled(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);
GFXcanvas16 Caneva(SCREEN_WIDTH, SCREEN_HEIGHT);
AnaClock Horl_Ana(&Caneva, SCREEN_WIDTH, SCREEN_HEIGHT);
JaugeAiguille JaugeTemp(&Caneva, SCREEN_WIDTH, SCREEN_HEIGHT, "C");
JaugeAiguille JaugeHum(&Caneva, SCREEN_WIDTH, SCREEN_HEIGHT, "%");
JaugeTriangle JaugePress(&Caneva, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 2000, "hPa");
JaugeRectangle JaugeLumiere(&Caneva, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 3000, "Lux");

void setup()
{
  Serial.begin(MONITOR_SPEED);
  pinMode(LED_WIFI_PIN, OUTPUT);
  pinMode(LED_WEB_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);

  if (!Temperature.begin(ADRESSE_TEMPERATURE) || !Temps.begin(ADRESSE_TEMPS) || !Lumiere.begin(ADRESSE_LUMIERE) || !HumiPress.begin(ADRESS_BME680_I2C, true) || !initSPIFFS())
  {
    Serial.println("Erreur initialisation des composants");
    return;
  }

  if (HumiPress.setPressureOversampling(0x5) && HumiPress.setHumidityOversampling(0x1))
    Serial.println("Oversampling Successful");
  else
    Serial.println("Oversampling Failed");

  Oled.begin();
  timer.start();
  Lumiere.config(Lumiere.GAIN_MED, Lumiere.ATIME_300);
  servoMoteur.attach(SERVO_PIN);

  WiFi.mode(WIFI_STA);
  WiFi.onEvent(connect, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(got_Ip, ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(disconnect, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.begin(ssid, pwd);
}

void loop()
{
  timer.update();
}

bool initSPIFFS()
{
  bool res = false;
  res = SPIFFS.begin();

  if (!res)
    Serial.println("Error Init SPIFFS");
  else
  {
    Serial.println("Init SPIFFS successful");

    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    while (file)
    {
      Serial.print("File: ");
      Serial.println(file.name());
      file.close();
      file = root.openNextFile();
    }
  }

  return res;
}

void action()
{
  HumiPress.beginReading();
  gtemperature = Temperature.getTemperature();
  glumiere = Lumiere.calculateLux();
  gpression = HumiPress.readPressure() / 100;
  ghumidite = HumiPress.readHumidity();
  HumiPress.endReading();

  affichage();
}

void affichage()
{
  static int cpt = 0;
  switch (cpt)
  {
  case 0:
    cpt++;
    JaugeTemp.update(gtemperature);
    JaugeTemp.draw();
    delay(DELAY);
    break;
  case 1:
    cpt++;
    JaugeLumiere.update(glumiere);
    JaugeLumiere.draw();
    delay(DELAY);
    break;
  case 2:
    cpt++;
    JaugeHum.update(ghumidite);
    JaugeHum.draw();
    delay(DELAY);
    break;
  default:
    cpt = 0;
    HumiPress.beginReading();
    JaugePress.update(gpression);
    JaugePress.draw();
    delay(DELAY);
    break;
  }
  Oled.drawRGBBitmap(0, 0, Caneva.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT);
}

void setRoutes()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  server.on("/justgage.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/justgage.js", "text/javascript"); });

  server.on("/raphael.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/raphael.min.js", "text/javascript"); });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/script.js", "text/javascript"); });

  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/w3.css", "text/css"); });

  server.on("/button", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if(request->hasParam(PARAM_BUTTON)) 
    {
      String state = request->getParam(PARAM_BUTTON)->value();
      if(state == "1")
        digitalWrite(LED_WEB_PIN, HIGH);
      else
        digitalWrite(LED_WEB_PIN, LOW);
    } request->send(200); });

  server.on("/slider", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if(request->hasParam(PARAM_VALUE))
    {
      String Svalue = request->getParam(PARAM_VALUE)->value();
      int servo_value = Svalue.toInt();
      servoMoteur.write(servo_value);
    }
    request->send(200); });

  server.on("/datas", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", createJson()); });

  server.on("/cityinfos", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if(request->hasParam(PARAM_CITY))
              {
                City = request->getParam(PARAM_CITY)->value();
                getDataFromAPI();
              }
              request->send(200, "text/plain", getDataFromAPI()); });

  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              String Jour = "", Date = "", _Temps = "";
              int params = request->params();
              for (int i = 0; i < params; i++)
              {
                AsyncWebParameter *p = request->getParam(i);
                if (p->isPost())
                {
                  if (p->name() == "theJour")
                  {
                    Jour = p->value();
                  }
                  if (p->name() == "theDate")
                  {
                    Date = p->value();
                  }
                  if (p->name() == "theTemps")
                  {
                    _Temps = p->value();
                  }
                }
              }
              
              request->send(SPIFFS, "/index.html", "text/html");
              Serial.println(Jour);
              Serial.println(Date);
              Serial.println(_Temps);
              Temps.setDate(Jour.toInt(), (Date.substring(8, 10)).toInt(), (Date.substring(5, 7)).toInt() - 1);
              Temps.setTime((_Temps.substring(0, 2)).toInt(), (_Temps.substring(3, 5)).toInt(), 0); });
}

String createJson()
{
  String res = "{\"temperature\":" + (String)gtemperature;
  res += ",\"lux\":" + (String)glumiere;
  res += ",\"humidity\":" + (String)ghumidite;
  res += ",\"pressure\":" + (String)gpression;
  res += "}";
  Serial.println(res);
  return res;
}

String getDataFromAPI()
{
  String jsonBuffer = "";
  String API = "" + URL + City + API_KEY + UNITS + LANG;
  jsonBuffer = httpGetRequest(API.c_str());

  return jsonBuffer;
}

String httpGetRequest(const char *url)
{
  WiFiClient client;
  HTTPClient http;
  String payload = "";

  http.begin(client, url);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0)
  {
    Serial.print("HTTP code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }

  http.end();
  return payload;
}

void connect(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.printf("\nConnected to: %s\n", ssid);
  digitalWrite(LED_WIFI_PIN, HIGH);
}

void disconnect(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Disconnected");
  WiFi.begin(ssid, pwd);
}

void got_Ip(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
  setRoutes();
}
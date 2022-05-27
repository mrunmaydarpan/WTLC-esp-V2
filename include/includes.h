#ifdef DryRun
#define DryRunTime 120000
#endif

#if debugData
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <AsyncElegantOTA.h>
#include <ArduinoJson.h>
#if HA_INIT
#include <ArduinoHA.h>
#else
// #include <ESPDash.h>
#endif
#include <ESP8266mDNS.h>
#include <JC_Button.h>
#include <EEPROM.h>
#if OLED
#include <Adafruit_SSD1306.h>
#else
#include <LiquidCrystal_I2C.h>
#endif
#include <SoftwareSerial.h>
#include <Timer.h>
#if SENSOR_1
#include <Ultrasonic.h>
#endif
#include <Smoothed.h>
#include <pins.h>
#if WM_SET
#include <AsyncWiFiManager.h>
#elif ESP_CON
#include <ESPConnect.h>
#endif

#if SENSOR_1
Ultrasonic Sonar(TriggerPin, EchoPin);
#elif SENSOR_2
SoftwareSerial sensorSerial(sensorRX, sensorTX);
#elif SENSOR_3
#endif
#if OLED
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#else
LiquidCrystal_I2C lcd(0x27, 16, 2);
#endif
Button button(PB);
Button modeButton(_Mode);
Timer t;
Smoothed<uint8_t> mySensor;
AsyncWebServer server(80);
#if HA_INIT
byte mac[WL_MAC_ADDR_LENGTH];
// #else
// ESPDash dashboard(&server);
#endif
// DNSServer dns;
WiFiClient client;
#if WM_SET
AsyncWiFiManager wm;
#endif
#include <includes.h>
// setup EEPROM locations
#define motorState_mem 1
#define maxDistance_mem 2
#define minDistance_mem 3
#define MotorStartThreshold_mem 4
#define manualOff_mem 5
#define LastMotorState_mem 6
#define dryRun_LastDistance_mem 7
#define StatorType_mem 8
#define AutoMode_mem 9
#define DryRun_mem 10

int MinDistance;
int MotorStartThreshold;
int MaxDistance, value, LastValue;
uint8_t STATOR_TYPE;
uint8_t errorCount;
uint8_t ledBlink;
int Distance, DistanceX;
int dryRun_timer;
uint8_t dryRun_Distance(0),
    dryRun_LastDistance(0);
#ifdef web_setting
extern const char index_html[];
#endif
const byte DNS_PORT = 53;

const unsigned long LONG_PRESS(2000);

bool MotorState, LastMotorState;
bool AutoMode = false;
bool ManualOff = false;
bool blink_state = false;
bool errorCountState, LasterrorCountState;
bool WiFiState;
bool DryRunState;
bool MQTT = false;
bool webConfig = false;
bool shouldSaveConfig = false;
bool DryRun = false;

long duration;
long DryRunTime = 120000;
String Command;

unsigned char data[4] = {};
#if OLED
const uint8_t mdtronix_icon[] PROGMEM = {0x00, 0x00, 0x0c, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf8, 0x1f, 0xff, 0xc0, 0x00, 0x00, 0x1f, 0xff, 0xfe, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x83, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xfe, 0x00, 0x07, 0xff, 0xf3, 0xff, 0xf0, 0x3f, 0xff, 0x80, 0x0f, 0xff, 0xc0, 0xff, 0xfc, 0x1f, 0xff, 0xe0, 0x3f, 0xff, 0x00, 0x3f, 0xff, 0x07, 0xff, 0xf0, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xc1, 0xff, 0xfc, 0xff, 0xf8, 0x00, 0x07, 0xff, 0xc0, 0xff, 0xfe, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xc0, 0x3f, 0xfe, 0xff, 0x80, 0x1c, 0x00, 0xff, 0xc0, 0x0f, 0xfe, 0xff, 0x00, 0x3f, 0x00, 0x3f, 0xc0, 0x07, 0xfe, 0xfe, 0x00, 0xff, 0xc0, 0x1f, 0xc0, 0x03, 0xfe, 0xfc, 0x03, 0xff, 0xf0, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x07, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x01, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x03, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x0f, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0x3f, 0xfe, 0xfc, 0x0f, 0xff, 0xf8, 0x1f, 0xc0, 0xff, 0xfe, 0x3c, 0x0f, 0xff, 0xf8, 0x1f, 0x03, 0xff, 0xf8, 0x0c, 0x0f, 0xff, 0xf8, 0x1c, 0x07, 0xff, 0xe0, 0x04, 0x0f, 0xff, 0xf8, 0x10, 0x1f, 0xff, 0xc0, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x7f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0xf8, 0x01, 0xff, 0xfc, 0x00, 0x00, 0x0f, 0xff, 0xf8, 0x07, 0xff, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xf8, 0x1f, 0xff, 0xc0, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x60, 0x00, 0x00};
const uint8_t network_icon[] PROGMEM = {0x00, 0x80, 0x01, 0xc0, 0x33, 0xe0, 0x37, 0xf0, 0x37, 0xf0, 0x30, 0xc0, 0x30, 0xc0, 0xfe, 0xc0, 0xfe, 0xc0, 0x7c, 0xc0, 0x38, 0x00, 0x10, 0x00};
const uint8_t alert_icon[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x07, 0x80, 0x00, 0x00, 0x03, 0xc0, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x03, 0xc0, 0x00, 0x00, 0x07, 0x80, 0x01, 0xe0, 0x00, 0x00, 0x0f, 0x01, 0x80, 0xf0, 0x00, 0x00, 0x0f, 0x03, 0xc0, 0xf0, 0x00, 0x00, 0x1e, 0x03, 0xc0, 0x78, 0x00, 0x00, 0x3c, 0x03, 0xc0, 0x3c, 0x00, 0x00, 0x3c, 0x03, 0xc0, 0x3c, 0x00, 0x00, 0x7c, 0x03, 0xc0, 0x3e, 0x00, 0x00, 0x78, 0x03, 0xc0, 0x1e, 0x00, 0x00, 0xf0, 0x03, 0xc0, 0x0f, 0x00, 0x00, 0xf0, 0x03, 0xc0, 0x0f, 0x00, 0x01, 0xe0, 0x03, 0xc0, 0x07, 0x80, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x07, 0x80, 0x03, 0xc0, 0x01, 0xe0, 0x07, 0x80, 0x03, 0xc0, 0x01, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x78, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x3c, 0x00, 0x03, 0xc0, 0x00, 0x3c, 0x78, 0x00, 0x03, 0xc0, 0x00, 0x1e, 0xf8, 0x00, 0x01, 0x80, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t done_icon[] = {0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x03, 0xff, 0x80, 0x07, 0xc3, 0x86, 0x0f, 0x00, 0x0e, 0x1c, 0x00, 0x1c, 0x38, 0x00, 0x38, 0x38, 0x00, 0x70, 0x70, 0x00, 0xe0, 0x70, 0x01, 0xc0, 0x60, 0xc3, 0x86, 0x60, 0xe7, 0x06, 0x60, 0x7e, 0x06, 0x60, 0x3c, 0x06, 0x70, 0x18, 0x0e, 0x70, 0x00, 0x0e, 0x38, 0x00, 0x1c, 0x38, 0x00, 0x1c, 0x1c, 0x00, 0x38, 0x0f, 0x00, 0xf0, 0x07, 0xc3, 0xe0, 0x03, 0xff, 0xc0, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00};
#else
byte BlinkUp[] = {B11111, B11111, B11111, B11111,
                  B00000, B00000, B00000, B00000};

byte BlinkDown[] = {B00000, B00000, B00000, B00000,
                    B11111, B11111, B11111, B11111};

byte ManualOff_char[] = {
    0x02,
    0x02,
    0x06,
    0x1F,
    0x1F,
    0x1E,
    0x0E,
    0x0E};
byte network_icon[] = {
    0x00,
    0x0A,
    0x0B,
    0x0A,
    0x0A,
    0x1A,
    0x0A,
    0x00};
#endif

#if HA_INIT
#define DEVICE_VERSION _VERSION
#define DEVICE_MANUFACTURER "MDtronix Lab"
#define DEVICE_MODEL "WaterTank Controller: v9.0"

#define BROKER_USER "hassio"
#define BROKER_PASS "darkmaster"

#define DEVICE_NAME "MDtronix WTLC"
#define pump_name "WaterTank Pump"
#define mode_name "WaterTank Auto"
#define distance_name "WaterTank Distance"
#define level_name "WaterTank Level"
#define SensorError_name "WaterTank Sensor"

HADevice device;
HAMqtt mqtt_HA(client, device);
HASwitch pump_HA("Pump", false);
HASwitch mode_HA("Mode", false);
HASensor value_HA("Level");
HASensor distance_HA("Distance");
HASensor sensor_error_HA("System");
#endif

#include <headers.h>

void setup()
{
  EEPROM.begin(32);
  Serial.begin(115200);
  Serial.println();
  Serial.println();
#if HA_INIT
  if (!LittleFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }
  else
  {
    Serial.println("mounted");
  }
  if (!loadConfig())
  {
    Serial.println("failed to load config");
  }
  else
  {
    Serial.println("config loaded");
  }
#endif
#if OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
#else
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, BlinkUp);
  lcd.createChar(1, BlinkDown);
  lcd.createChar(2, ManualOff_char);
  lcd.createChar(3, network_icon);
#endif
#if SENSOR_1
#elif SENSOR_2
  sensorSerial.begin(9600);
#elif SENSOR_3
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(PWM_OUTPUT_PIN, INPUT);
#endif
  ReadMem();
  button.begin();
  modeButton.begin();
  pinMode(led, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(Relay_ON, OUTPUT);
  pinMode(Relay_ON_2, OUTPUT);
  pinMode(Relay_OFF, OUTPUT);
  if (ManualOff == false && AutoMode == true)
  {
    AutoMode = true;
  }
  else
  {
    AutoMode = false;
  }
  StartUp();
  if (MotorState == true && ManualOff == false && AutoMode == true)
  {
    debugln("turning motor on");
    LastMotorState = false;
    PumpON_command();
  }
  else
  {
    PumpOFF_command();
  }
  t.every(100, readSensor);
  t.every(500, update_lcd);
#if debugData
  t.every(1000, Debug);
#endif

  mySensor.begin(SMOOTHED_AVERAGE, 50);
  // mySensor.clear();
  WIFI_CONNECT();
#if HA_INIT
  set_device();
#endif
  setting_code();
}

void loop()
{
  t.update();
  button.read();
  modeButton.read();
  lcdDefault();
  buttonEvent();
  OneTimeRun();
  if (ManualOff == true)
  {
#if OLED
#else
    lcd.setCursor(14, 1);
    lcd.write(2);
#endif
  }
  else
  {
#if OLED
#else
    lcd.setCursor(14, 1);
    lcd.print(" ");
#endif
  }
#if HA_INIT
  mqtt_HA.loop();
  if (mqtt_HA.isConnected())
  {
#if !OLED
    lcd.setCursor(13, 1);
    lcd.write(3);
#endif
  }
  else
  {
#if !OLED
    lcd.setCursor(13, 1);
    lcd.print(" ");
#endif
  }
#endif
#ifdef WM_SET
  wm.process();
#endif
  MDNS.update();
  EEPROM.write(manualOff_mem, ManualOff);
  EEPROM.write(AutoMode_mem, AutoMode);
  EEPROM.write(dryRun_LastDistance_mem, dryRun_LastDistance);
  EEPROM.write(LastMotorState_mem, LastMotorState);
  EEPROM.write(motorState_mem, MotorState);
  EEPROM.write(minDistance_mem, MinDistance);
  EEPROM.write(maxDistance_mem, MaxDistance);
  EEPROM.write(MotorStartThreshold_mem, MotorStartThreshold);
  EEPROM.write(StatorType_mem, STATOR_TYPE);
  EEPROM.write(DryRun_mem, DryRun);
  EEPROM.commit();
}
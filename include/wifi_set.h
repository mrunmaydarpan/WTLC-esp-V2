const char *min_ = "min";
const char *max_ = "max";
const char *thres_ = "threshold";
const char *stator_ = "stator";

#ifdef WM_SET
void configModeCallback(AsyncWiFiManager *myAsyncWiFiManager)
{
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setFont(NULL);
    display.setCursor(5, 22);
    display.println("SETUP WIFI");
    display.display();
    delay(2000);
    debugln("Entered config mode");
}

void save_callback()
{
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setFont(NULL);
    display.setCursor(5, 23);
    display.println("WIFI SAVED");
    display.display();
    delay(1000);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setFont(NULL);
    display.setCursor(5, 23);
    display.println("REBOOTING");
    display.display();
    delay(1000);
    ESP.restart();
}

#if HA_INIT
void saveParamsCallback()
{
    StaticJsonDocument<200> doc;
    doc["mqtt_server"] = custom_mqtt_server.getValue();
    File configFile = LittleFS.open("/config.json", "w");
    if (!configFile)
    {
        Serial.println("Failed to open config file for writing");
    }
    serializeJson(doc, configFile);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setFont(NULL);
    display.setCursor(5, 17);
    display.println("SERVER:");
    display.setCursor(5, 37);
    display.print(custom_mqtt_server.getValue());
    display.display();
    delay(1000);
}
#endif
#endif

void WIFI_CONNECT()
{
    // wm.resetSettings();
#ifdef WM_SET
#if OLED
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setFont(NULL);
    display.setCursor(0, 16);
    display.println("CONNECTING");
    display.setCursor(0, 35);
    display.setTextSize(2);
    display.print("WiFi");
    display.display();
    delay(1000);
#else
    lcd.clear();
    lcd.print("connecting WiFi");
#endif
#endif
#ifdef WM_SET
    WiFi.mode(WIFI_AP_STA);
    wm.setConfigPortalBlocking(false);
#if HA_INIT
    wm.setSaveConfigCallback(save_callback);
    wm.setSaveParamsCallback(saveParamsCallback);
    wm.addParameter(&custom_mqtt_server);
#endif
    // wm.setAPCallback(configModeCallback);
    if (wm.autoConnect("MDtronix-WTLC")) // if Connected successfully
    {
#if OLED
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setFont(NULL);
        display.setCursor(5, 17);
        display.println("CONNECTED TO");
        display.setCursor(5, 30);
        display.print(WiFi.SSID());
        display.setCursor(5, 43);
        display.print(WiFi.localIP());
        display.drawBitmap(90, 20, done_icon, 24, 24, 1);
        display.display();
        delay(2000);
#if HA_INIT
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setFont(NULL);
        display.setCursor(5, 17);
        display.println("SERVER:");
        display.setCursor(5, 37);
        display.print(BROKER_ADDR);
        display.display();
#endif
#else
        lcd.clear();
        lcd.print("Connected");
        lcd.setCursor(0, 1);
        lcd.print(WiFi.localIP());
#endif
    }
    else
    { // if not connected
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(2);
        display.setFont(NULL);
        display.setCursor(0, 17);
        display.println("WIFI NOT  CONNECTED");
        display.display();
        delay(1000);
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(2);
        display.setFont(NULL);
        display.setCursor(5, 22);
        display.println("SETUP WIFI");
        display.display();
    }
    delay(1000);

#if OLED
#else
    lcd.clear();
#endif
#else
    WiFi.softAP("MDtronix-WTLC");
#endif
}

void setting_code()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                          StaticJsonDocument<200> doc;
                          doc["level"] = value;
                          doc["pump"] = MotorState;
                          doc["mode"] = AutoMode;
                          String reply;
                          serializeJsonPretty(doc, reply);
                          request->send(200, "text/json", reply); });

#ifdef web_setting
    server.on("/setting", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  debugln("setting pages");
                  request->send_P(200, "text/html", index_html, processor); });
#endif
    server.on("/get_setting", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  StaticJsonDocument<200> doc;
                  doc["min"] = MinDistance;
                  doc["max"] = MaxDistance;
                  doc["threshold"] = MotorStartThreshold;
                  doc["starter"] = STATOR_TYPE;
                  String reply;
                  serializeJsonPretty(doc,reply);
                  request->send(200,"text/json",reply); });

    server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  int min;
                  int max;
                  int threshold;
                  int stator;
                  String reply;
                if (request->hasParam(min_) && request->hasParam(max_) && request->hasParam(thres_) && request->hasParam(stator_))
                  {
                      min = request->getParam(min_)->value().toInt();
                      max = request->getParam(max_)->value().toInt();
                      threshold = request->getParam(thres_)->value().toInt();
                      stator = request->getParam(stator_)->value().toInt();
                      if (min == 0 || max == 0 || threshold == 0)
                      {
                          reply = "error";
                          request->send(201, "text/plain", reply);
                      }
                      else if (min < max && threshold <= 70 && threshold >= 20 && stator < 4 && stator != 0)
                      {
                          StaticJsonDocument<200> doc;
                          doc["min"] = min;
                          doc["max"] = max;
                          doc["threshold"] = threshold;
                          doc["starter"] = stator;
                          serializeJsonPretty(doc,reply);
                          MinDistance = min;
                          MaxDistance = max;
                          MotorStartThreshold = threshold;
                          STATOR_TYPE = stator;
                          debugln("min: " + min ? min : MinDistance);
                          request->send(200, "text/plain", reply);
                       }
                  }
                  else if(request->hasParam("pump")) //control pump from app
                  {
                      MotorState = request->getParam("pump")->value().toInt();
                      request->send(203,"text/plain","ok");
                  }
                  else if(request->hasParam("mode")) //change mode from app
                  {
                      AutoMode = request->getParam("mode")->value().toInt();
                      request->send(204,"text/plain","ok");
                  }
                  else
                  {
                      reply = "No message sent";
                      request->send(202, "text/plain", reply);
                  } });

    // dns.start(DNS_PORT, "*", IPAddress(WiFi.localIP()));
    if (!MDNS.begin("mdtronix-wtlc"))
    {
        debugln("Error setting up MDNS responder!");
        while (1)
        {
            delay(1000);
        }
    }
    debugln("mDNS responder started");
    AsyncElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();
    MDNS.addService("http", "tcp", 80);
}
#if HA_INIT
void set_device()
{
    WiFi.macAddress(mac);
    device.setUniqueId(mac, sizeof(mac)); // Unique ID must be set!
    device.setName(DEVICE_NAME);
    device.setSoftwareVersion(DEVICE_VERSION);
    device.setManufacturer(DEVICE_MANUFACTURER);
    device.setModel(DEVICE_MODEL);
    device.enableSharedAvailability();
    device.enableLastWill();
    pump_HA.onStateChanged(pump_action); // handle switch state
    pump_HA.setName(pump_name);
    pump_HA.setIcon("mdi:water");
    value_HA.setName(level_name);
    value_HA.setIcon("mdi:waves");
    value_HA.setUnitOfMeasurement("%");
    distance_HA.setName(distance_name);
    distance_HA.setIcon("mdi:ruler");
    distance_HA.setUnitOfMeasurement("cm");
    mode_HA.onStateChanged(mode_action);
    mode_HA.setName(mode_name);
    mode_HA.setIcon("mdi:nintendo-switch");
    sensor_error_HA.setName(SensorError_name);

    IPAddress HA_IP;
    HA_IP.fromString(BROKER_ADDR);
    mqtt_HA.begin(HA_IP, BROKER_USER, BROKER_PASS);
}
#endif
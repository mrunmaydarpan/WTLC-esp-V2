void StartUp()
{
    const char *compile_date = __DATE__;
    debugln(F("Ready......."));
    debugln("SW: " + String(_VERSION));
    debugln("DT: " + String(compile_date));
    debugln("MaxDistance: " + String(MaxDistance));
    debugln("MinDistance: " + String(MinDistance));
    debugln("start at: " + String(MotorStartThreshold));
    debugln("ManualOff: " + String(EEPROM.read(manualOff_mem)));
    debugln("MotorState: " + String(EEPROM.read(motorState_mem)));
    debugln("Mode: " + String(EEPROM.read(AutoMode_mem)));
    debug(F("Starting."));
    pinMode(buzz, OUTPUT);
#if OLED
    display.clearDisplay();
    delay(500);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(4, 4);
    display.println("SW:          HW:");
    display.setCursor(22, 4);
    display.print(_VERSION);
    display.setCursor(101, 4);
    display.print(_HARDWARE);
    display.setCursor(80, 20);
    display.print("SENSOR:");
    display.setCursor(122, 20);
    display.print(SENSOR_DISP);
    display.setCursor(92, 32);
    display.print("MODE:");
    display.setCursor(122, 32);
    display.print(STATOR_TYPE);
    // display.drawRect(0, 0, 128, 16, 1);
    display.drawBitmap(4, 16, mdtronix_icon, 63, 48, 1);
    display.setCursor(79, 55);
    display.print("MDtronix");
    display.display();
    delay(3000);
    display.clearDisplay();
    display.setTextSize(1);
    display.setFont(NULL);
    display.setCursor(4, 16);
    display.println("MIN:");
    display.setCursor(29, 16);
    display.print(MinDistance);
    display.setCursor(4, 30);
    display.print("MAX:");
    display.setCursor(29, 30);
    display.print(MaxDistance);
    display.setCursor(4, 44);
    display.print("START AT: ");
    display.setCursor(59, 44);
    display.print(MotorStartThreshold);
    display.display();
    delay(1000);
    display.clearDisplay();
#else
    lcd.clear();
    lcd.setCursor(11, 0);
    lcd.print("v" + String(_VERSION));
    lcd.setCursor(0, 1);
    lcd.print(compile_date);
    for (int l = 0; l < int(sizeof(brand) - 1); l++)
    {
        lcd.setCursor(l, 0);
        lcd.print(brand[l]);
#if Buzzer
        tone(buzz, 4500, 150);
#endif
        digitalWrite(led, !digitalRead(led));
        debug(F("."));
        delay(300);
    }
#endif
    debugln();
#if !OLED
    delay(500);
    char showVersion[14];
    char showData[16];
    sprintf(showVersion, "HW:%s", _HARDWARE);
    sprintf(showData, "L:%d H:%d P:%d", MaxDistance, MinDistance, MotorStartThreshold);
    lcd.clear();
    lcd.print(showVersion);
    lcd.setCursor(0, 1);
    lcd.print(showData);
    lcd.setCursor(14, 0);
#if SENSOR_1
    lcd.print("1");
#elif SENSOR_2
    lcd.print("2");
#elif SENSOR_3
    lcd.print("3");
#endif
    lcd.setCursor(15, 0);
    lcd.print(STATOR_TYPE);
    delay(2000);
    lcd.clear();
#endif
    digitalWrite(led, LOW);
}
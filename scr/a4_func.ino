void initHardware() {
#if (WDT_ENABLE == 1)
  // Для ESP32 watchdog настраивается иначе
  esp_task_wdt_init(8, true);
  esp_task_wdt_add(NULL);
#endif
  
  // ----- дисплей -----
  lcd.init();
  lcd.backlight();
  lcd.clear();
#if (USE_PLOTS == 1 || USE_PID == 1 || USE_DAWN == 1)
  lcd.createChar(0, row8);
  lcd.createChar(1, row1);
  lcd.createChar(2, row2);
  lcd.createChar(3, row3);
  lcd.createChar(4, row4);
  lcd.createChar(5, row5);
  lcd.createChar(6, row6);
  lcd.createChar(7, row7);
#endif

  // ----- RTC -----
  if (!rtc.begin()) {  // при ошибке инициализации
    // rtc.setTime(COMPILE_TIME);
  }

  // ---------- BME ----------
#if (USE_BME == 1)
  if (!bme.begin(BME_ADDR)) {
    // ошибка инициализации BME
  }
#endif

  // разгон шины
#if (WIRE_OVERCLOCK == 1)
  Wire.setClock(400000);
#endif

  // ---------- DHT ----------
#if (DHT_SENS2 == 1)
  dht.begin();
#endif

#if (USE_HTU21D == 1)
  myHTU21D.begin();
#endif

#if (USE_DRIVE == 1)
  // привод - для ESP32 используем ledc для PWM
  pinMode(DRV_PWM, OUTPUT);
  pinMode(DRV_SIGNAL1, OUTPUT);
  pinMode(DRV_SIGNAL2, OUTPUT);
  digitalWrite(DRV_SIGNAL1, DRIVER_LEVEL);
  digitalWrite(DRV_SIGNAL2, DRIVER_LEVEL);
  // Настройка PWM через ledc для ESP32
  ledcSetup(0, 31400, 8);  // канал 0, частота 31.4 кГц, 8 бит
  ledcAttachPin(DRV_PWM, 0);
  ledcWrite(0, settings.drvSpeed);
#endif

  // настройка прерываний для энкодера на ESP32
  attachInterrupt(digitalPinToInterrupt(CLK), encISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DT), encISR2, CHANGE);

  pinMode(SENS_VCC, OUTPUT);

#if (SERVO1_RELAY == 1)
  pinMode(SERVO_0, OUTPUT);
#endif

#if (SERVO2_RELAY == 1)
  pinMode(SERVO_1, OUTPUT);
#endif

  for (byte i = 0; i < 7; i++) pinMode(relayPins[i], OUTPUT);

#if (SERVO1_RELAY == 0)
  servo1.attach(SERVO_0, SERVO_MIN_PULSE, SERVO_MAX_PULSE);
#endif
#if (SERVO2_RELAY == 0)
  servo2.attach(SERVO_1, SERVO_MIN_PULSE, SERVO_MAX_PULSE);
#endif
}

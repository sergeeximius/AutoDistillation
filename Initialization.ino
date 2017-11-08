void InitSensors(void) {
  DeviceAddress addr[3];

  // Start up the library
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
  Serial.print(F ("Found "));
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(F (" DS18B20 devices."));
  Serial.println();

  oneWire.reset_search();
  for (byte i = 0; i < sensors.getDeviceCount(); i++) {
    sensors.getAddress(addr[i], i);
    printAddress(addr[i]);
    if (!memcmp(CubeThermometer, addr[i], 8)) {
      Serial.println (F ("Found CubeThermometer."));
      sensors.setResolution(CubeThermometer, TEMPERATURE_PRECISION);
      IsFoundCubeThermometr = 1;
      Serial.println();
    }
    if (!memcmp(DeflThermometer, addr[i], 8)) {
      Serial.println (F ("Found DeflThermometer."));
      sensors.setResolution(DeflThermometer, TEMPERATURE_PRECISION);
      IsFoundDeflThermometr = 1;
      Serial.println();
    }
    if (!memcmp(AlarThermometer, addr[i], 8)) {
      Serial.println (F ("Found AlarThermometer."));
      sensors.setResolution(AlarThermometer, TEMPERATURE_PRECISION);
      IsFoundAlarThermometr = 1;
      Serial.println();
    }
  }

  if (IsFoundCubeThermometr == 0) Serial.println(F ("Critical Error. CubeThermometer not found!"));
  if (IsFoundDeflThermometr == 0) Serial.println(F ("Critical Error. DeflThermometer not found!"));
  if (IsFoundAlarThermometr == 0) Serial.println(F ("Warning. AlarThermometer not found!"));
}

void SearchSensors() {
  byte addr[8];
  if (oneWire.search(addr) != 1)
  {
    oneWire.reset_search();
    delay(1000);
    for (byte i = 0; i < 8; i++) addr[i] = 0;
    return;
  }

  oneWire.reset();
  oneWire.select(addr);
  Serial.println("----------");
  Serial.print(F ("Addr: "));
  for (byte i = 0; i < 8; i++)
  {
    Serial.print("0x");
    if (addr[i] < 0x10) Serial.print("0");
    Serial.print(addr[i], HEX);
    if (i == 8) Serial.println();
    else Serial.print(", ");
  }
}

void printAddress(DeviceAddress deviceAddress) {
  Serial.print(F ("Addr: "));
  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.print("0x");
    if (deviceAddress[i] < 0x10) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    if (i == 7) Serial.println();
    else Serial.print(", ");
  }
}

void InitBMP(void) {
  if (pressure.begin()) {
    Serial.println(F ("BMP180 init success"));
    IsFoundBMP180 = 1;
    CurrAtmPress = GetAtmPressure();
    CurrTempHkip = tempHkip();
    Serial.print(F ("CurrTempHkip - "));
    Serial.println(CurrTempHkip);
  }
  else {
    Serial.println(F ("BMP180 init fail\n\n"));
    //while (1); // Pause forever.
  }
}

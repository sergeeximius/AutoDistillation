float GetAtmPressure (void) {
  //return Pressure mBar
  char status;
  double T, P;

  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      //Serial.print("temperature: ");
      //Serial.print(T, 2);
      //Serial.print(" deg C, ");

      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);

        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          //Serial.print(F ("Pressure: "));
          //Serial.print(P * 0.75, 2);
          //Serial.println(F (" mmHg"));

          //return P * 0.75;
            return float(P);
        }
        else return 0;
      }
      else return 0;
    }
    else return 0;
  }
  else return 0;
}

// Температура кипения воды в зависимости от текущего атмосферного давления
double tempHkip() {
  double TempC = 25;
  if (IsFoundBMP180 == 1) {
    return (( 373 * (TempC + 273) * 2300000 * 0.018 ) / ( 2300000 * 0.018 * (TempC + 273) + 0.029 * 9.81 * pressure.altitude(CurrAtmPress, 1013.25) * 373 ) - 273);
  }
  else {
    return TEMP_CALIBR;
  }
}

// Корректировка дельты дефлегматора в зависимости от изменения атмосферного давления
float tempSDiff(float oldPress) {
  Serial.print(F ("tempSDiff - "));
  Serial.println(0.034 * (CurrAtmPress * 0.75 - oldPress * 0.75));
  return 0.034 * (CurrAtmPress * 0.75 - oldPress * 0.75);
}

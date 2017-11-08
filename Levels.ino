void SetLevel(int LevelValue)
{
  if (LastLevel == 3 and LevelValue == 4)
  {
    if (CurrDistillTask == 4)
    {
      LevelValue = 8;
    }
    else
    {
      LevelValue = 11;
    }
  }
  if (LastLevel == 11 and LevelValue == 12)
  {
    LevelValue = 9;
  }
  if (LastLevel == 10 and LevelValue == 11)
  {
    LevelValue = 4;
  }
  if (LastLevel == 6 and LevelValue == 7)
  {
    LevelValue = 6;
  }
  if (LastLevel == 8 and LevelValue == 9)
  {
    LevelValue = 6;
  }

  SensorIsActive = true;

  switch (LevelValue) {
    case 0:
      //Serial.println("Current Level is STOP");
      timeStopDiff = timeNow - timeLastLevel;
      CurrLevel = 0;
      // Задаем отбор в 0% из 100% возможных. Клапан отбора закрыт, РМЦ-3 на паузе
      Brezenhem(GetBrezVal(0), BrezenhemVal);
      TriggerCubeTemp = false;
      TriggerTime = false;
      TriggerDeltaTemp = false;
      CurrPowerValue = 0;
      if (AUTO_POWER == 1) {
        digitalWrite(rmc3StopPin, HIGH);
      }
      break;
    case 1:
      Serial.println(F ("Current Level is RAZGON"));
      CurrLevel = 1;
      LastLevel = 1;
      timeLastLevel = timeNow - timeStopDiff;
      Brezenhem(GetBrezVal(0), BrezenhemVal);
      TriggerCubeTemp = true;
      TriggerValue = 80;
      TriggerTime = false;
      TriggerDeltaTemp = false;
      TriggerDeflTemp = false;
      CurrPowerValue = 2000;
      break;
    case 2:
      Serial.println(F ("Current Level is NA_SEBYA"));
      CurrLevel = 2;
      LastLevel = 2;
      timeLastLevel = timeNow  - timeStopDiff;
      Brezenhem(GetBrezVal(0), BrezenhemVal);
      CurrPowerValue = GetPowerTeloValue();
      TriggerCubeTemp = false;
      TriggerTime = true;
      TriggerValue = GetTimeValue(0);
      Serial.print(F ("Trigger Value - "));
      Serial.println(TriggerValue);
      TriggerDeflTemp = false;
      break;
    case 3:
      Serial.println(F ("Current Level is GOLOVY"));
      CurrLevel = 3;
      LastLevel = 3;
      timeLastLevel = timeNow  - timeStopDiff;
      //Brezenhem(GetBrezVal(START_GOLV_SHIM), BrezenhemVal);
      CurrPowerValue = GetPowerGolvValue();
      TriggerCubeTemp = false;
      TriggerTime = true;
      TriggerValue = GetTimeValue(GetFlowValue(CurrPowerValue, GetFNValue() * FN_GOLV_K));
      Serial.print(F ("Trigger Value - "));
      Serial.println(TriggerValue);
      TriggerDeltaTemp = false;
      TriggerDeflTemp = false;
      StartCubeTemp = GetStaticCubeTemp();
      Brezenhem(GetBrezVal(GetFlowShim(CurrPowerValue, GetFNValue() * FN_GOLV_K)), BrezenhemVal);
      Serial.print("AC volume, ml - ");
      Serial.println(GetFlowValue(CurrPowerValue, GetFNValue() * FN_GOLV_K) * TriggerValue / 3600);
      break;
    case 4:
      Serial.println(F ("Current Level is TELO"));
      CurrLevel = 4;
      LastLevel = 4;
      timeLastLevel = timeNow  - timeStopDiff;
      //Brezenhem(GetBrezVal(START_TELO_SHIM), BrezenhemVal);
      CurrPowerValue = GetPowerTeloValue();
      TriggerCubeTemp = true;
      TriggerValue = GetCubeTempValue();
      TriggerTime = false;
      TriggerDeltaTemp = true;
      TriggerDeltaValue = GetDeltaTempValue();
      Serial.print(F ("Trigger Value - "));
      Serial.println(TriggerValue);
      TriggerDeflTemp = false;
      //StartDeflTemp = GetStaticDeflTemp();
      //StartAtmPress = CurrAtmPress;
      Brezenhem(GetBrezVal(GetFlowShim(CurrPowerValue, GetFNValue())), BrezenhemVal);
      break;
    case 5:
      Serial.println(F ("Current Level is HVOSTY"));
      CurrLevel = 5;
      LastLevel = 5;
      timeLastLevel = timeNow  - timeStopDiff;
      // Клапан отбора как и был после тела, РМЦ-3 не трогаем
      Brezenhem(CurrBrezenhemSize, BrezenhemVal);
      StopValue = 0;
      TriggerCubeTemp = true;
      TriggerValue = GetCubeTempValue();
      Serial.print(F ("Trigger Value - "));
      Serial.println(TriggerValue);
      TriggerTime = false;
      TriggerDeltaTemp = false;
      TriggerDeflTemp = false;
      StartDeflTemp = GetStaticDeflTemp();
      break;
    case 6:
      //Serial.println("Current Level is DISPLAY");
      CurrLevel = 6;
      timeLastLevel = timeNow;
      // Клапан отбора на 0%, РМЦ-3 не трогаем
      Brezenhem(0, BrezenhemVal);
      TriggerCubeTemp = false;
      TriggerTime = false;
      TriggerDeltaTemp = false;
      TriggerDeflTemp = false;
      StartCubeTemp = GetStaticCubeTemp();
      StartDeflTemp = GetStaticDeflTemp();
      break;
    case 7:
      //Serial.println("Current Level is BRAGA NBK");
      CurrLevel = 7;
      LastLevel = 7;
      timeLastLevel = timeNow;
      Brezenhem(GetBrezVal(0), BrezenhemVal);
      CurrPowerValue = 1475;
      TriggerCubeTemp = false;
      TriggerTime = false;
      TriggerDeltaTemp = false;
      TriggerDeflTemp = false;
      StartCubeTemp = GetStaticCubeTemp();
      StartDeflTemp = GetStaticDeflTemp();
      break;
    case 8:
      Serial.println(F ("Current Level is CUBE"));
      CurrLevel = 8;
      LastLevel = 8;
      timeLastLevel = timeNow;
      Brezenhem(GetBrezVal(0), BrezenhemVal);
      CurrPowerValue = 1750;
      TriggerCubeTemp = false;
      TriggerDeflTemp =  true;
      TriggerValue = GetDeflTempValue();
      Serial.print(F ("Trigger Value - "));
      Serial.println(TriggerValue);
      TriggerTime = false;
      TriggerDeltaTemp = false;
      StartCubeTemp = GetStaticCubeTemp();
      StartDeflTemp = GetStaticDeflTemp();
      break;
    case 9:
      Serial.println(F ("Current Level is PREDTELO_1"));
      CurrLevel = 9;
      LastLevel = 9;
      timeLastLevel = timeNow;
      CurrPowerValue = GetPowerTeloValue();
      TriggerCubeTemp = false;
      TriggerTime = true;
      TriggerValue = GetTimeValue(GetFlowValue(CurrPowerValue, GetFNValue() * FN_PRED_K * 2));
      Serial.print(F ("Trigger Value - "));
      Serial.println(TriggerValue);
      TriggerDeltaTemp = false;
      TriggerDeflTemp = false;
      StartCubeTemp = GetStaticCubeTemp();
      StartDeflTemp = GetStaticDeflTemp();
      Brezenhem(GetBrezVal(GetFlowShim(CurrPowerValue, GetFNValue() * FN_PRED_K * 2)), BrezenhemVal);
      Serial.print("AC volume, ml - ");
      Serial.println(GetFlowValue(CurrPowerValue, GetFNValue() * FN_PRED_K * 2) * TriggerValue / 3600);
      break;
    case 10:
      Serial.println(F ("Current Level is PREDTELO_2"));
      CurrLevel = 10;
      LastLevel = 10;
      timeLastLevel = timeNow;
      CurrPowerValue = GetPowerTeloValue();
      TriggerCubeTemp = false;
      TriggerTime = true;
      TriggerValue = GetTimeValue(GetFlowValue(CurrPowerValue, GetFNValue() * FN_PRED_K));
      Serial.print(F ("Trigger Value - "));
      Serial.println(TriggerValue);
      TriggerDeltaTemp = false;
      TriggerDeflTemp = false;
      StartCubeTemp = GetStaticCubeTemp();
      StartDeflTemp = GetStaticDeflTemp();
      StartAtmPress = CurrAtmPress;
      Brezenhem(GetBrezVal(GetFlowShim(CurrPowerValue, GetFNValue() * FN_PRED_K)), BrezenhemVal);
      Serial.print("AC volume, ml - ");
      Serial.println(GetFlowValue(CurrPowerValue, GetFNValue() * FN_PRED_K) * TriggerValue / 3600);
      break;
    case 11:
      Serial.println(F ("Current Level is DELAY"));
      CurrLevel = 11;
      LastLevel = 11;
      timeLastLevel = timeNow;
      CurrPowerValue = GetPowerTeloValue();
      TriggerCubeTemp = false;
      TriggerTime = true;
      // Время задержки 30 мин
      TriggerValue = 1800;
      Serial.print(F ("Trigger Value - "));
      Serial.println(TriggerValue);
      TriggerDeltaTemp = false;
      TriggerDeflTemp = false;
      StartCubeTemp = GetStaticCubeTemp();
      StartDeflTemp = GetStaticDeflTemp();
      Brezenhem(GetBrezVal(0), BrezenhemVal);
      //Serial.print("AC volume, ml - ");
      //Serial.println(GetFlowValue(CurrPowerValue, GetFNValue() * FN_PRED_K) * TriggerValue / 3600);
      break;
  }
  PrintScreen(2);
}

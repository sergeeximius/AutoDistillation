void PrintScreen(int ScreenValue)
{
  lcd.clear();
  lcd.setCursor(0, 0);

  // В названии режима его Имя/Ткуба_для_стопа/ФЧ

  switch (ScreenValue) {
    case 0:      // Главное меню
      CurrScreen = 0;
      switch (CurrMenuIthem) {
        case 10:
          lcd.print(F (">Distillation"));
          lcd.setCursor(0, 1);
          lcd.print(F ("-->next down"));
          break;
        case 11:
          lcd.print(F (">>Distill/92/4"));
          lcd.setCursor(0, 1);
          lcd.print(F ("-->next down"));
          break;
        case 12:
          lcd.print(F (">>Distill/92/5"));
          lcd.setCursor(0, 1);
          lcd.print(F ("-->next down"));
          break;
        case 13:
          lcd.print(F (">>Braga NBK"));
          lcd.setCursor(0, 1);
          lcd.print(F ("-->next down"));
          break;
        case 14:
          lcd.print(F (">>Cube"));
          lcd.setCursor(0, 1);
          lcd.print(F ("--end"));
          break;
        case 20:
          lcd.print(F (">Rectification"));
          lcd.setCursor(0, 1);
          lcd.print(F ("-->next down"));
          break;
        case 21:
          lcd.print(F (">>Rect_1/93/4"));
          lcd.setCursor(0, 1);
          lcd.print(F ("-->next down"));
          break;
        case 22:
          lcd.print(F (">>Rect_2/93/4"));
          lcd.setCursor(0, 1);
          lcd.print(F ("--end"));
          break;
        case 30:
          lcd.print(F (">Service"));
          lcd.setCursor(0, 1);
          lcd.print(F ("--end"));
          break;
        case 31:
          lcd.print(F (">>Flow Calibr..."));
          lcd.setCursor(0, 1);
          lcd.print(F ("-->next down"));
          break;
        case 32:
          lcd.print(F (">>Temp Calibr..."));
          lcd.setCursor(0, 1);
          lcd.print(F ("--end"));
          break;
        default:
          CurrMenuIthem--;
          PrintScreen(0);
          break;
      }
      break;
    case 1:      // Меню уровней главного/Подменю
      CurrScreen = 1;
      switch (CurrMenuIthem) {
        case 11:
          lcd.print(F ("AC val - "));
          lcd.print(CurrACValue);
          lcd.print(F (" l"));
          lcd.setCursor(0, 1);
          lcd.print(F ("SEL to start"));
          break;
        case 12:
          lcd.print(F ("AC val - "));
          lcd.print(CurrACValue);
          lcd.print(F (" l"));
          lcd.setCursor(0, 1);
          lcd.print(F ("SEL to start"));
          break;
        case 13:
          lcd.print(F ("None options"));
          lcd.setCursor(0, 1);
          lcd.print(F ("SEL to start"));
          break;
        case 14:
          lcd.print(F ("Stop - "));
          lcd.print(CurrACVolume);
          lcd.print(F (" %"));
          lcd.setCursor(0, 1);
          lcd.print(F ("SEL to start"));
          break;
        case 21:
          lcd.print(F ("AC val - "));
          lcd.print(CurrACValue);
          lcd.print(F (" l"));
          lcd.setCursor(0, 1);
          lcd.print(F ("SEL to start"));
          break;
        case 22:
          lcd.print(F ("AC val - "));
          lcd.print(CurrACValue);
          lcd.print(F (" l"));
          lcd.setCursor(0, 1);
          lcd.print(F ("SEL to start"));
          break;
        case 31:
          lcd.print(F ("Valve open 60s."));
          lcd.setCursor(0, 1);
          lcd.print(F ("SEL to start"));
          break;
        case 32:
          lcd.print(F ("Temp set "));
          lcd.print(CurrTempHkip);
          lcd.setCursor(0, 1);
          lcd.print(F ("SEL to start"));
          break;
        case 43:
          lcd.print(F ("Flow - "));
          lcd.print(CurrFlowSpeed);
          lcd.print(F (" ml/h"));
          lcd.setCursor(0, 1);
          lcd.print(F ("SEL to start"));
          break;
      }
      break;
    case 2:      // Главный экран уровня
      CurrScreen = 2;
      lcd.print(Levels[CurrLevel]);

      if (ShowAlarTemp == 5) {
        // Показываем текущую мощность
        lcd.setCursor(11, 0);
        lcd.print(F ("P-"));
        lcd.print(GetShimVal(CurrPowerValue));
        ShowAlarTemp = 0;
      }
      else {
        if (IsFoundAlarThermometr == 1) {
          // Показывает температуру перегрева дефл.
          lcd.setCursor(9, 0);
          lcd.print(F ("A-"));
          lcd.print(GetStaticAlarTemp());
        }
        else {
          lcd.setCursor(11, 0);
          lcd.print(F ("P-"));
          lcd.print(GetShimVal(CurrPowerValue));
        }
        ShowAlarTemp++;
      }

      lcd.setCursor(0, 1);
      lcd.print(F ("C-"));
      lcd.print(GetStaticCubeTemp());
      lcd.setCursor(8, 1);
      lcd.print(F ("D-"));
      lcd.print(GetStaticDeflTemp());
      break;
    case 3:      // Расширенный экран уровня
      CurrScreen = 3;
      if (CurrLevel == 3 or CurrLevel == 8)
      {
        lcd.print(F ("Cs-"));
        lcd.print(StartCubeTemp);
      }
      else if (CurrLevel == 4 or CurrLevel == 5 or CurrLevel == 9 or CurrLevel == 10 or CurrLevel == 11) {
        if (CurrLevel == 4 or CurrLevel == 10) {
          if (ShowSDiff == 5) {
            // Показываем поправку Т кипения спирта на амт. давление
            lcd.print(F ("Sd-"));
            lcd.print(tempSDiff(StartAtmPress));
            ShowSDiff = 0;
          }
          else {
            lcd.print(F ("Ds-"));
            lcd.print(StartDeflTemp);
            ShowSDiff++;
          }
        }
        else {
          lcd.print(F ("Ds-"));
          lcd.print(StartDeflTemp);
        }
      }
      else if (CurrLevel == 7) {
        if (NBK_Auto == true)
        {
          lcd.print(F ("Cd-"));
          lcd.print(CurrTempHkip + 0.4 - GetStaticCubeTemp());
        }
        else
        {
          lcd.print(F ("Tk-"));
          lcd.print(CurrTempHkip);
        }
      }
      else {
        lcd.print(Levels[CurrLevel]);
      }
      if (CurrLevel == 8) {
        lcd.setCursor(8, 0);
        lcd.print(F ("Ds-"));
        lcd.print(StartDeflTemp);
      }
      else {

        if (ShowAtmPress == 5) {
          // Показываем текущее атм. давление, мм.рт.ст.
          lcd.setCursor(9, 0);
          lcd.print(F ("P-"));
          lcd.print(CurrAtmPress * 0.75, 1);
          ShowAtmPress = 0;
        }
        else {
          // Показывает величину ШИМ отбора
          lcd.setCursor(11, 0);
          if (StopValue == 0)
          {
            if (CurrLevel == 7)
            {
              if (NBK_Auto == true)
              {
                lcd.print(F ("Auto"));
              }
              else
              {
                lcd.print(F ("Manua"));
              }
            }
            else
            {
              lcd.print(F ("F-"));
              lcd.print(map(CurrBrezenhemSize, 0, BrezenhemSize, 0, 100));
            }
          }
          else
          {
            lcd.print(F ("S-"));
            lcd.print(StopCount);
          }
          ShowAtmPress++;
        }

      }
      lcd.setCursor(0, 1);
      if (CurrLevel == 2 or CurrLevel == 3 or CurrLevel == 9 or CurrLevel == 10 or CurrLevel == 11)
      {
        lcd.print(F ("LTime-"));
        lcd.print((timeLastLevel / 1000 + TriggerValue - timeNow / 1000) / 60);
      }
      else
      {
        lcd.print(F ("Time - "));
        lcd.print(timeNow / 1000 / 60);
      }
      lcd.print(F (" min"));
      break;
    case 4:      // Экран меню уровня
      CurrScreen = 4;
      switch (CurrMenuIthem) {
        case 41:
          lcd.print(F (">Stop/Resume"));
          lcd.setCursor(0, 1);
          lcd.print(F ("-->next down"));
          break;
        case 42:
          lcd.print(F (">Next Level"));
          lcd.setCursor(0, 1);
          lcd.print(F ("-->next down"));
          break;
        case 43:
          lcd.print(F (">>Flow Calibr..."));
          lcd.setCursor(0, 1);
          lcd.print(F ("--end"));
          break;
        default:
          CurrMenuIthem = 41;
          PrintScreen(4);
          break;
      }
      break;
    case 5:
      CurrScreen = 5;    // Экран уровня калибровки DS18B20
      lcd.print(F ("Dev0 C - "));
      lcd.print(CurrTempHkip - GetStaticCubeTemp());
      lcd.setCursor(0, 1);
      lcd.print(F ("Dev1 D - "));
      lcd.print(CurrTempHkip - GetStaticDeflTemp());
      break;
  }
}

void PrintStartLogo()
{
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Automatic");
  lcd.setCursor(2, 1);
  lcd.print(F ("Distillation"));
  delay(3000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print(F ("Version"));
  lcd.setCursor(4, 1);
  lcd.print(F (VERSION));
  delay(4000);
}

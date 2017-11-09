
void GetPressKey(byte ScreenValue)
{
  byte tmpFlow = map(CurrBrezenhemSize, 0, BrezenhemSize, 0, 100);
  // Обработка кнопок
  unsigned int x;
  x = analogRead (0);

  if (x < 1000) {

    KeyIsActive = true;
    timeKeyLastActive = timeNow;
    SensorIsActive = false;

    //Serial.println("Key is active");

    switch (ScreenValue) {
      case 0:      // Экран главного меню
        if (x < 60) {
          //lcd.print ("Right ");
          if (CurrMenuIthem == 10 or CurrMenuIthem == 20 or CurrMenuIthem == 30)
          {
            CurrMenuIthem++;
          }
          else
          {
            PrintScreen(1);
            delay(300);
            break;
          }
          delay(300);
        }
        else if (x < 200) {
          //lcd.print ("Up    ");
          if (CurrMenuIthem == 20 or CurrMenuIthem == 30)
          {
            CurrMenuIthem = CurrMenuIthem - 10;
          }
          else if (CurrMenuIthem == 10)
          {
            CurrMenuIthem = 10;
          }
          else
          {
            if (CurrMenuIthem != 11 and CurrMenuIthem != 21 and CurrMenuIthem != 31)
            {
              CurrMenuIthem--;
            }
          }
          delay(300);
        }
        else if (x < 400) {
          //lcd.print ("Down  ");
          if (CurrMenuIthem == 10 or CurrMenuIthem == 20)
          {
            CurrMenuIthem = CurrMenuIthem + 10;
          }
          else if (CurrMenuIthem == 30)
          {
            CurrMenuIthem = 30;
          }
          else
          {
            CurrMenuIthem++;
          }
          delay(300);
        }
        else if (x < 600) {
          //lcd.print ("Left  ");
          if (CurrMenuIthem > 30)
          {
            CurrMenuIthem = 30;
          }
          else if (CurrMenuIthem > 20 and CurrMenuIthem < 30)
          {
            CurrMenuIthem = 20;
          }
          else if (CurrMenuIthem > 10 and CurrMenuIthem < 20)
          {
            CurrMenuIthem = 10;
          }
          delay(300);
        }
        else if (x < 800) {
          //lcd.print ("Select");
          if (CurrMenuIthem == 10 or CurrMenuIthem == 20 or CurrMenuIthem == 30)
          {
            CurrMenuIthem++;
          }
          else
          {
            PrintScreen(1);
            delay(300);
            break;
          }
          delay(300);
        }
        if (x < 900)
        {
          PrintScreen(0);
        }
        break;

      case 1:    // Экран уровней подменю
        if (x < 60) {
          //lcd.print ("Right ");
          delay(300);
        }
        else if (x < 200) {
          //lcd.print ("Up    ");
          if (CurrMenuIthem == 11 or CurrMenuIthem == 12 or CurrMenuIthem == 21 or CurrMenuIthem == 22)
          {
            CurrACValue += 0.1;
            PrintScreen(1);
            delay(300);
            break;
          }
          if (CurrMenuIthem == 14)
          {
            CurrACVolume += 5;
            if (CurrACVolume < 40 and CurrACVolume > 10)
            {
              CurrACVolume = 40;
            }
            if (CurrACVolume > 63)
            {
              CurrACVolume = 63;
            }
            PrintScreen(1);
            delay(300);
            break;
          }
          if (CurrMenuIthem == 43)
          {
            CurrFlowSpeed += 10;
            PrintScreen(1);
            delay(300);
            break;
          }
          if (CurrMenuIthem == 32)
          {
            CurrTempHkip += 0.01;
            PrintScreen(1);
            delay(300);
            break;
          }

          delay(300);
        }
        else if (x < 400) {
          //lcd.print ("Down  ");
          if (CurrMenuIthem == 11 or CurrMenuIthem == 12 or CurrMenuIthem == 21 or CurrMenuIthem == 22)
          {
            CurrACValue -= 0.1;
            PrintScreen(1);
            delay(300);
            break;
          }
          if (CurrMenuIthem == 14)
          {
            CurrACVolume -= 5;

            if (CurrACVolume < 40 and CurrACVolume > 10)
            {
              CurrACVolume = 10;
            }
            if (CurrACVolume < STOP_AC_VOLUME)
            {
              CurrACVolume = STOP_AC_VOLUME;
            }
            PrintScreen(1);
            delay(300);
            break;
          }
          if (CurrMenuIthem == 43)
          {
            CurrFlowSpeed -= 10;
            PrintScreen(1);
            delay(300);
            break;
          }
          if (CurrMenuIthem == 32)
          {
            CurrTempHkip -= 0.01;
            PrintScreen(1);
            delay(300);
            break;
          }
          delay(300);
        }
        else if (x < 600) {
          //lcd.print ("Left  ");
          PrintScreen(0);
          delay(300);
        }
        else if (x < 800) {
          //lcd.print ("Select");
          switch (CurrMenuIthem) {
            case 11:
              CurrDistillTask = 0;
              SetLevel(1);
              PrintScreen(2);
              break;
            case 12:
              CurrDistillTask = 3;
              SetLevel(1);
              PrintScreen(2);
              break;
            case 13:
              SetLevel(7);
              PrintScreen(2);
              break;
            case 14:
              CurrDistillTask = 4;
              SetLevel(1);
              PrintScreen(2);
              break;
            case 21:
              CurrDistillTask = 1;
              SetLevel(1);
              PrintScreen(2);
              break;
            case 22:
              CurrDistillTask = 2;
              SetLevel(1);
              PrintScreen(2);
              break;
            case 31:
              lcd.clear();
              lcd.print("Open valve ...");
              lcd.setCursor(0, 1);
              lcd.print("Wait!");
              digitalWrite(relayPin, LOW);
              delay(60000);
              digitalWrite(relayPin, HIGH);
              lcd.setCursor(0, 1);
              lcd.print("Done!");
              delay(1000);
              PrintScreen(0);
              break;
            case 32:
              CurrPowerValue = 1200;
              PrintScreen(5);
              break;
            case 43:
              PrintScreen(2);
              break;
          }
          delay(300);
        }
        break;

      case 2:    // Главный экран уровня
        if (x < 60) {
          //lcd.print ("Right ");
          PrintScreen(3);
          delay(300);
        }
        else if (x < 200) {
          //lcd.print ("Up    ");
          CurrPowerValue += HEATER_POWER / 100;
          delay(300);
        }
        else if (x < 400) {
          //lcd.print ("Down  ");
          CurrPowerValue -= HEATER_POWER / 100;
          delay(300);
        }
        else if (x < 600) {
          //lcd.print ("Left  ");
          delay(300);
        }
        else if (x < 800) {
          //lcd.print ("Select");
          PrintScreen(4);
          delay(300);
        }
        break;

      case 3:    // Расширенный экран уровня
        if (x < 60) {
          //lcd.print ("Right ");
          if (CurrMenuIthem == 13) // Брага НБК, Включаем ручное управление температурой
          {
            if (NBK_Auto == true)
            {
              NBK_Auto = false;
            }
            else
            {
              NBK_Auto = true;
            }
          }
          delay(300);
        }
        else if (x < 200) {
          //lcd.print ("Up    ");
          if (CurrMenuIthem == 13)
          {
            CurrTempHkip += 0.01;
          }
          if (CurrMenuIthem == 14)
          {
            CurrPowerValue += 25;
          }
          else
          {
            Brezenhem(GetBrezVal(tmpFlow + 1), BrezenhemVal);
          }
          delay(300);
        }
        else if (x < 400) {
          //lcd.print ("Down  ");
          if (CurrMenuIthem == 13)
          {
            CurrTempHkip -= 0.01;
          }
          if (CurrMenuIthem == 14)
          {
            CurrPowerValue -= 25;
          }
          else
          {
            Brezenhem(GetBrezVal(tmpFlow - 1), BrezenhemVal);
          }
          delay(300);
        }
        else if (x < 600) {
          //lcd.print ("Left  ");
          PrintScreen(2);
          delay(300);
        }
        else if (x < 800) {
          //lcd.print ("Select");
          PrintScreen(4);
          delay(300);
        }
        break;

      case 4:    // Экран меню уровня
        if (x < 60) {
          //lcd.print ("Right ");
          if (CurrMenuIthem == 41)
          {
            if (CurrLevel != 0)
            {
              SetLevel(0);
            }
            else
            {
              SetLevel(LastLevel);
              timeStopDiff = 0;
            }
          }
          if (CurrMenuIthem == 42)
          {
            if (CurrLevel == 6)
            {
              SetLevel(LastLevel + 1);
              //timeLastLevel = timeNow;
            }
            else
            {
              SetLevel(CurrLevel + 1);
              //timeLastLevel = timeNow;
            }
          }
          if (CurrMenuIthem == 43)
          {
            lcd.clear();
            lcd.print("Open valve ...");
            lcd.setCursor(0, 1);
            lcd.print("Wait!");
            digitalWrite(relayPin, LOW);
            delay(10000);
            digitalWrite(relayPin, HIGH);
            lcd.setCursor(0, 1);
            lcd.print("Done!");
            delay(60000);
            PrintScreen(1);
            break;
          }
          PrintScreen(2);
          delay(300);
          break;
        }
        else if (x < 200) {
          //lcd.print ("Up    ");
          if (CurrMenuIthem != 41)
          {
            CurrMenuIthem--;
          }
          delay(300);
        }
        else if (x < 400) {
          //lcd.print ("Down  ");
          if (CurrMenuIthem != 43)
          {
            CurrMenuIthem++;
          }
          delay(300);
        }
        else if (x < 600) {
          //lcd.print ("Left  ");
          PrintScreen(2);
          delay(300);
          break;
        }
        else if (x < 800) {
          //lcd.print ("Select");
          if (CurrMenuIthem == 41)
          {
            if (CurrLevel != 0)
            {
              SetLevel(0);
            }
            else
            {
              SetLevel(LastLevel);
            }
          }
          if (CurrMenuIthem == 42)
          {
            if (CurrLevel == 6)
            {
              SetLevel(LastLevel + 1);
              //timeLastLevel = timeNow;
            }
            else
            {
              SetLevel(CurrLevel + 1);
              //timeLastLevel = timeNow;
            }
          }
          if (CurrMenuIthem == 43)
          {
            lcd.clear();
            lcd.print("Open valve ...");
            lcd.setCursor(0, 1);
            lcd.print("Wait!");
            digitalWrite(relayPin, LOW);
            delay(60000);
            digitalWrite(relayPin, HIGH);
            lcd.setCursor(0, 1);
            lcd.print("Done!");
            delay(1000);
            PrintScreen(1);
            break;
          }
          PrintScreen(2);
          delay(300);
          break;
        }
        if (x < 900)
        {
          PrintScreen(4);
        }
        break;
    }
  }

  if (SensorIsActive == false and KeyIsActive == true and (timeNow - timeKeyLastActive) > 5000)
  {
    KeyIsActive = false;
    SensorIsActive = true;
    //Serial.println("Key not active");
  }
}


// ТРИГГЕРЫ ===============
void TriggerCubeTemperature (void)
{
  // Обработка событий по Температуре куба
  switch (CurrLevel) {
    case 1:
      SetLevel(2);
      break;
    case 4:
      if (FLOW_CONTROL == 1 or (DistillTasksParams[CurrDistillTask][3] - DistillTasksParams[CurrDistillTask][2]) < 0.5) {
        SetLevel(5);
      }
      else
      {
        SetLevel(6);
      }
      break;
    case 5:
      SetLevel(0);
      break;
  }
}

void TriggerTimeValue (void)
{
  // Обработка событий по истечению времени
  // Принудительный переход на ПРЕДТЕЛО и ТЕЛО
  // При ручном режиме НЕ ПРОЕБАТЬ МОМЕНТ
  switch (CurrLevel) {
    case 2:
      SetLevel(3);
      break;
    case 3:
      if (CurrDistillTask == 4)
      {
        SetLevel(8);
      }
      else
      {
        //SetLevel(9);
        //DELAY
        SetLevel(11);
      }
      break;
    case 9:
      SetLevel(10);
      break;
    case 10:
      SetLevel(4);
      break;
    case 11:
      SetLevel(9);
      break;
  }
}

void TriggerDeflDelta (int CurrValue)
{
  // Обработка событий по превышению Температуры дефлегматора заданной дельты
  if (CurrValue == 0)
  {
    //Тормозим отбор
    Brezenhem(0, BrezenhemVal);
    StopValue = 1;
    StopCount++;
  }
  else if (CurrValue == 1)
  {
    // Возобновляем отбор к прежнему значению потока
    Brezenhem(CurrBrezenhemSize, BrezenhemVal);
    StopValue = 0;
  }
}

void TriggerDeflTemperature (void)
{
  SetLevel(0);
}
// =============== ТРИГГЕРЫ


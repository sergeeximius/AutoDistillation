// Значения для параметров уровней ===============

float GetTimeValue(float FlowValue)
{
  if (CurrLevel == 2)
    // На себя
  {
    // Если время задано в рецепте
    if (DistillTasksParams[CurrDistillTask][4] > 0)
    {
      return DistillTasksParams[CurrDistillTask][4] * 60;
    }
    else
    {
      // Если нет (Куб), то пропускаем этот уровень
      return 5;
    }
  }

  if (CurrLevel == 3 or CurrLevel == 9 or CurrLevel == 10)
    // Головы и Предтело оба
  {
    // Если отбор установлен в рецепте
    if (DistillTasksParams[CurrDistillTask][0] > 0)
    {
      // Головы, отбираем задано по рецепту, первое значение в рецепте
      if (CurrLevel == 3)
      {
        //Serial.print("TimeValue - ");
        //Serial.println(CurrACValue * DistillTasksParams[CurrDistillTask][0] * 1000 * 3600 / FlowValue);
        return CurrACValue * DistillTasksParams[CurrDistillTask][0] * 1000 * 3600 / FlowValue;
      }
      // Предтело 1, отбираем VOL_PRED1 % на половинной скорости
      if (CurrLevel == 9)
      {
        return CurrACValue * VOL_PRED1 * 1000 * 3600 / FlowValue / 100;
      }
      // Предтело 2, отбираем остаток предтела на полной скорости
      if (CurrLevel == 10)
      {
        return CurrACValue * (DistillTasksParams[CurrDistillTask][1] * 100 - VOL_PRED1) * 1000 * 3600 / FlowValue / 100;
      }

    }
    else
    {
      // Если не установлен отбор голов (Куб), то
      // Если спиртуозность в струе =5%, то это брага на СС и без отбора голов
      if (CurrACVolume == STOP_AC_VOLUME)
      {
        return 5;
      }
      else
        // возвращаем время !!!30!!! мин если спиртуозность в струе выше 13%, т.е. это дробная перегонка с отбором голов
      {
        return 30 * 60;
      }
    }
  }
}

// Достаем из рецепта мощность на которой отбирается тело
int GetPowerTeloValue(void)
{
  return DistillTasksParams[CurrDistillTask][5];
}

// Расчет мощности на которой отбираются головы
// Если ФЧ задано, то отбираем головы на колонне, мощность тела
// Иначе отдается мощность тела уменьшенная в 7 раза для Cube
int GetPowerGolvValue(void)
{
  if (GetFNValue() > 0)
  {
    return DistillTasksParams[CurrDistillTask][5];
  }
  else
  {
    return DistillTasksParams[CurrDistillTask][5] / 7;
  }
}

// Достаем из рецепта температуру куба для окончания отбора ТЕЛА
float GetCubeTempValue(void)
{
  return DistillTasksParams[CurrDistillTask][CurrLevel - 2];
}

// Расчет дельты температуры для ДЕФЛЕГМАТОРА при отборе ТЕЛА
float GetDeltaTempValue(void)
{
  return DistillTasksParams[CurrDistillTask][7];
}

// Расчет температуры дефлегматора для окончания отбора ТЕЛА
float GetDeflTempValue(void)
{
  int Temp[7]   = {10,     40,   45,   50,   55,   60,   63};
  float Krep[7] = {99.1, 95.8, 94.8, 93.8, 92.7, 91.3, 90.6};
  byte i;
  
  for (i = 0; i < 7; ++i)
  {
    if (Temp[i] == CurrACVolume) return Krep[i];
  }
  
  return 99.5;
}

// Расчет величины отбора в мл/час в зависимости от мощности и ФЧ
float GetFlowValue(int Power, int FN)
{
  //Serial.print("FlowValue - ");
  //Serial.println(4.84 * (Power - HEATER_LOST) / (FN + 1));
  return 4.84 * (Power - HEATER_LOST) / (FN + 1);
}

// Расчет ШИМ отбора в % в зависимости от мощности и ФЧ
int GetFlowShim(int Power, int FN)
{
  Serial.print(F ("FlowShim - "));
  Serial.println(round((100 * 4.84 * (Power - HEATER_LOST) / (FN + 1)) / CurrFlowSpeed));
  return round((100 * 4.84 * (Power - HEATER_LOST) / (FN + 1)) / CurrFlowSpeed);
}

// Достаем ФЧ из рецепта
int GetFNValue(void)
{
  return DistillTasksParams[CurrDistillTask][6];
}
// =============== Значения для параметров уровней

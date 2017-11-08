unsigned char SetupTimer2(float timeoutFrequency) {
  unsigned char result; //Начальное значение таймера.
  //Подсчет начального значения таймера
  result = (int)((257.0 - (TIMER_CLOCK_FREQ / timeoutFrequency)) + 0.5);
  //257 на самом деле должно быть 256, но я получил лучшие результаты с 257.
  //Установки Таймер2: Делитель частоты /1024, режим 0
  //Частота = 16MHz/1024 = 16khz
  TCCR2A = 0;
  TCCR2B = 1 << CS22 | 1 << CS21 | 1 << CS20;
  //Подключение прерывания по переполнению Timer2
  TIMSK2 = 1 << TOIE2;
  //загружает таймер для первого цикла
  TCNT2 = result;
  return (result);
}

//Timer2 указатель вектора прерывания по переполнению
ISR(TIMER2_OVF_vect) {
  
  if (CurrPowerValue == 0) {
    pinByte = false;
  }
  else if (Timer2Cnt <= GetShimVal(CurrPowerValue)) {
    pinByte = true;
  }
  else {
      pinByte = false;
  }

  //Переключение IO-вывода 
  digitalWrite(rmc3ShimPin, pinByte);

  if (Timer2Cnt < 100)
  {
    Timer2Cnt++;
  }
  else
  {
    Timer2Cnt = 0;
  }
  //Захват текущего значения таймера. Это величина ошибки
  //из-за задержки обработки прерывания и работы этой функции
  latency = TCNT2;
  //Перезагрузка таймера и коррекция по задержке
  TCNT2 = latency + timerLoadValue;
}

// Принимает значение мощности на ТЭНе в Вт, возвращает число импульса ШИМ
int GetShimVal(int PowerValue)
{
  return map(PowerValue, 0, HEATER_POWER, 0, 100);
}

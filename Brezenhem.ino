void Brezenhem(byte Value, byte * ArrayVal)
{
  int error = BrezenhemSize - Value;
  byte x;
  for (x = 0; x < BrezenhemSize; x++) {
    if ( error < BrezenhemSize / 2 ) {
      error += BrezenhemSize;
      ArrayVal[x] = 1;
      //Serial.print(F ("1"));
    } else {
      ArrayVal[x] = 0;
      //Serial.print(F ("0"));
    }
    error -= Value;
  }
  //Serial.println(F ("."));
}

// Принимает значение отбора в %, возвращает кол-во срабатываний клапана в течении цикла
int GetBrezVal(float FlowValue)
{
  CurrBrezenhemSize = map(FlowValue, 0, 100, 0, BrezenhemSize);
  if (CurrScreen == 3)
  {
    PrintScreen(3);
  }
  return CurrBrezenhemSize;
}

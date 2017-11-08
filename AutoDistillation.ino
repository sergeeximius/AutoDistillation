/*
  ГОТОВО. Корректировку Тдеф при отборе тела. Для случая если вдруг она высокая выберется автоматически. Сделано через переменную tempSDiff.

  на доп. экране вывести ожидаемый объем голов, ?тела? и ?хвостов?

  Если за минуту Ткуба упала больше чем на 1*, то прибавляем мощность принудительно. ОТслеживает дельту изменения температуры

  !!!Уровень разгон для браги до Т=90*!!!! Переделать разгон на триггер по достижению Т дефлегматора
  Стоп отбора тела по таймауту дельты старт стопа. Если в течении 3-5 мин она не возвращается, то переходим на отбор хвостов.

  Экран вывода заключительной информации для оценки качества работы
  Отображать исходные данные - объем АС, Время затраченное на перегонку, Ткуба после работы на себя, Кол-во срабатываний старт-стопа, Кол-во корректировок Тдеф с отображением смещения Т и времени когда это было произведено от начала работы
  В идеале сохранять эту информацию для дальнейшей выгрузки и построения графиков. Куда сохранять пока не ясно, можно по сети когда она будет или подуматьпро SD карту

  Уйти от объема АС при задании исходных параметров. Переделать на задание объема навалки общего, на основе Ткуба пересчитывать крепость и соответственно величину АС для расчета голов и предтела.

  Пищалка для оповещения о предстоящих сменах уровней
  Расчитать поправку ФЧ на теплопотери котла. Расчитать через время и мощность нагрева и время остывания. Или через задание такой мощности при которой перестанет расти Ткуба. Будет равна мощности потерь.
  Добавить кроме рецептов возможность ручного задания параметров для задач дистилляции и ректификации, таких как мощность, ФЧ, Тстопа

  Переписать управление кнопками. Сделать switch по коду клавиши и уже внутри разбирать в каком контексте она нажата.
  Разнести в главном меню идеологию задач. Сгрупировать их по оборудованию: НБК/ТК, РК, КУБ. Подумать нужно!!!

  При калибровке из меню сервиса не происходит возврата в экран задания величины потока

  Переделать меню, сейчас в 2х местах приходится писать одни и те же функции для сервиса, вынести их в функции

  НАСОС ОТ ЕВГЕНИЯ, МОЯ ПОДАЧА 130 РПМ

  ??????Корректировку минимальной и максимальной дельты барды, скорее всего одним сдвигом через 0.1*. Сейчас 0.1-0.4, 07.11.17 смог войти в эту дельту только при давлении 30мм.рт.ст., при давлении 20мм.рт.ст. дельта смещается к 0.6*.

  Корректировку автоуменьшения отбора, если число стопов за 5 минут например более 10, то уменьшаем поток в формуле автоумеьшения. Сейчас при хвостатой браге и 18 тарелок и дельте 0.1 залеты очень частые и долго колбасится старт-стоп.
  09.11.17
  - исправил дельту барды, довел ее до действительно приемлимого состояния минимум 6%, максимум 10%.
  - изменил вычисление значения потока с учетом Т куба. Теперь каждое срабатывание старт-стопа уменьшает на 1% ШИМ отбора в дополнение к коррекции по Т куба. Работает при Т в кубе > 80*.
*/


#include <SPI.h>
#include <stdint.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Wire.h>
#include <SFE_BMP180.h>
/*Hardware connections BMP180:
  - (GND) to GND
  + (VDD) to 3.3V
  Any Arduino pins labeled:  SDA  SCL
  Uno, Redboard, Pro:        A4   A5
  Mega2560, Due:             20   21
*/

// Определения работы ПО
#define VERSION "091117.1"  // Версия ПО. ДеньМесяцГод.номер
#define AUTO_POWER 1        // 1 - автоматическое упавление РМЦ-3500, 0 - вручную
#define FLOW_CONTROL 0      // 0 - ручной контрль наполнения приемной емкости
#define HEATER_POWER 2420   // Мощность ТЭНа в Вт
#define START_AC_VALUE 6.8  // Количество АС в баке
#define STOP_AC_VOLUME 5    // Крепость спирта % в струе для завершения отбора.
#define START_SCREEN 0      // Стартовый экран. 0 - меню
#define FLOW_SPEED 1350     // Скорость потока при полностью открытом клапане
#define SENSOR_0   0.10     // Корректировка Т куба
#define SENSOR_1   0.10     // Корректировка Т дефл,                                  по воде была поправка -0.10, но по спирту нужно +0.20 добавить, ДЕЛАЮ ПОПРАВКУ ДЛЯ ОБЕИХ ДАТЧИКОВ, Т.К. НА БУДУЩЕЕ ПЛАНИРУЮ ПО Т КУБА УЗНАВАТЬ КРЕПОСТЬ НАВАЛКИ
#define SENSOR_2   0.00     // Корректировка Т авар 
#define TEMP_CALIBR 100     // Температура кипения воды для калибровки датчиков, зависит от текущего атм. давления, ОНА ЖЕ температура кипения барды!!!!!!!!!! Заложено в LOOP() К ней прибавлять ориентировочно 0,35-0,4* с расчетом на 10мм давления в кубе. От этой величины уже отсчитывать дельту
#define BARDA_MIN_DLT 0.7   // Минимальная дельта для барды, потери 6% л АС. ДОПУСТИМО!!!
#define BARDA_MAX_DLT 1.0   // Максимальная дельта для барды, потери 10% л АС. ДОПУСТИМО!!!
#define NBK_MAX_POWER 1600  // Максимальная мощность нагрева для НБК
#define NBK_MIN_POWER 1420  // Минимальная мощность нагрева для НБК
#define HEATER_LOST 300     // Теплопорери в Вт
#define FN_GOLV_K 11        // Коэффициент ФЧ для голов от ФЧ тела
#define FN_PRED_K 1         // Коэффициент ФЧ для предтела от ФЧ тела
#define VOL_PRED1 6         // Объем предтела 1 в % от АС, он отбирается на 0,5 скорости тела
#define ALTITUDE 88.0       // Высота над уровнем моря, метры

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Подключение термодатчиков, на плате PIN 2
// 1 - дата, 2 - земля, 3 - +5
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 12
#define TEMPERATURE_ARRAY 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

byte CubeThermometer[8] = {0x28, 0x4C, 0x88, 0xB2, 0x01, 0x00, 0x00, 0xDE}; //RED - КРАСНЫЙ
byte DeflThermometer[8] = {0x28, 0xFF, 0xAC, 0xAF, 0x01, 0x00, 0x00, 0xC2};  //BLUE - СИНИЙ
byte AlarThermometer[8] = {0x28, 0xFF, 0xA2, 0x34, 0x92, 0x15, 0x01, 0x94};  //YELLOW - ЖЕЛТЫЙ, 0x28, 0xFF, 0xA2, 0x34, 0x92, 0x15, 0x01, 0x94

byte IsFoundCubeThermometr = 0;
byte IsFoundDeflThermometr = 0;
byte IsFoundAlarThermometr = 0;

#define TIMER_CLOCK_FREQ 16000.0 //16kHz for /1024 prescale from 16MHz
int Timer2Cnt;
unsigned int latency;
boolean pinByte = true;
unsigned char timerLoadValue;

byte rmc3ShimPin = 3; // сигнал ШИМ на РМЦ-3, на разъеме регулятора контакт 2 (земля GND контакт 3) Конфликтует с LCD
byte rmc3StopPin = 11; // сигнал СТОП на РМЦ-3, на разъеме регулятора контакт 1

byte relayPin = 13; // Реле управления клапаном отбора

//int buzzPin = A1; // Пищалка
SFE_BMP180 pressure;
byte IsFoundBMP180 = 0;

float TempSensor_1;
float TempSensor_2;
float TempSensor_3;
float Temp0Array[TEMPERATURE_ARRAY];
float Temp1Array[TEMPERATURE_ARRAY];
float Temp2Array[TEMPERATURE_ARRAY];
byte Temp0ArrayIthem = 0;
byte Temp1ArrayIthem = 0;
byte Temp2ArrayIthem = 0;

// Для выключения сенсоров, если работаем с меню
boolean SensorIsActive = false;
boolean KeyIsActive = false;
unsigned long timeKeyLastActive;

unsigned long timeLastCheckTemp; // Время последнего опроса датчиков температуры
unsigned long timeLastCheckBrez; // Время последнего изменения состояния клапана отбора
unsigned long timeLastCheckCube; // Время последней проверки Т куба
unsigned long timeLastLevel;     // Время от завершения прошлого уровня
unsigned long timeNow;           // Текущее время от начала работы
unsigned long timeStopDiff = 0;  // Время от предыдущего уровна до принудительного стопа, добавляется как поправка при возврате к этому же уровню

byte CurrBrezenhemStep;          // Номер текущего шага состояния клапана отбора
byte BrezenhemSize = 100;        // Дискретность управления отбором, кол-во срабатываний клапана за время цикла
byte BrezenhemVal[100];          // = BrezenhemSize, менять совместно!!!
byte CurrBrezenhemSize;          // Текущее значение отбора
float tmpFlow;                   // ШИМ отбора скорректированный

boolean StopValue = 0;           // Переменная включенного стопа клапана отбора. 1 - стоп активен, 0 - стоп отключен
unsigned int StopCount = 0;      // Кол-во срабатываний старт/стопа

char * Levels[] = {"STOP", "RAZGON", "NA_SEBYA", "GOLOVY", "TELO", "HVOSTY", "DISPLAY", "BRG NBK", "CUBE", "PRDTELO1", "PRDTELO2", "DELAY"}; // Уровни задач дистилляции
byte CurrLevel;                  // Текущий уровень задачи дистилляции, 0-стоп,1-разгон,2-на себя, 3-головы,4-тело,5-хвосты,6-режим отображения температуры,7-брага НБК,8-брага кубовая,9-предело
byte LastLevel;                  // Последний уровень перед стопом

char * DistillTasks[] = {"DIST 92/4", "RECT_1/93/4", "DIST 92/5", "RECT_2/93/4", "CUBE"};  // Задачи дистилляции/ректификации цифра Т куба для завершения отбора тела
byte CurrDistillTask;            // Значение текущей задачи дистилляции/ректификации
float DistillTasksParams[5][8] = {                   // Параметры для задач дистилляции
  // ПроцентГоловИПредтелаДляОтбора, ДельтаТДефлегматораПриОтбореТела, ТемператураКубаДляЗавершенияТела, ТемператураКубаДляСтопа, ВремяРаботыНаСебя, МощностьТЕНаДляТела, ФЧ
  // Если разница между Т куба для стопа и Т куба для стопа тела < 0.5 , то переход с тела на хвосты автоматический, т.к. идет полный отжим АС
  // Скорость отбора голов 50мл на 1кВт мощности
  // Если параметр равен 0, то он не будет обрабатываться в коде, это заглушка
  {0.03, 0.20, 91.6, 97.4, 30, 1400, 4, 0.10}, // Параметры для дистилляции Distill 92/4 (Тстопа_куба/ФЧ) - 1500, ЧТОБЫ ОТБОР БЫЛ НЕ ВЫШЕ 100% ШИМ
  {0.03, 0.20, 92.8, 97.4, 90,  850, 4, 0.10}, // Параметры для ректификации 1 проход 93/4
  {0.02, 0.30, 92.8, 97.4, 60,  850, 4, 0.10}, // Параметры для ректификации 2 проход 93/4
  {0.03, 0.17, 91.6, 97.4, 30, 1400, 5, 0.10},  // Параметры для дистилляции Distill 92/5
  {0, 0, 0, 0, 0, 1750, 0, 0} // КУБ. Мощность 1750 для тела, для голов в 7 раза ниже. НЕ ДВИГАТЬ, НОМЕР ТАСКА 4, ПРИВЯЗАН В КОДЕ
  // Заготовка под сахаро-фрукты
  // {0.02, 0.3, 92.8, 98.6, 20, 1450, 8, 100},  // Параметры для дистилляции Distill 93/0.3
  // {0.02, 0.4, 92.8, 98.6, 20, 1200, 7, 100},  // Параметры для дистилляции Distill 93/0.4
};

boolean TriggerCubeTemp = false;
boolean TriggerTime = false;
boolean TriggerDeflTemp = false;
float TriggerValue;
boolean TriggerDeltaTemp = false;
float TriggerDeltaValue;

// Значения температур в начале уровней
float StartCubeTemp;
float StartDeflTemp;
byte TempReplyUp = 0;  // Кол-во превышений температурой пороговых значений подряд, по умолчанию 3 раз (6 сек) в верх
byte TempReplyDown = 0;  // Кол-во превышений температурой пороговых значений подряд, по умолчанию 15 раз (30 сек) в низ
float CurrTempHkip = TEMP_CALIBR;
float CurrAtmPress = 1;
float StartAtmPress;

// Автоуправление НБК
boolean NBK_Auto = true;

// Текущее значение мощности РМЦ-3500 для выставления вручную
unsigned int CurrPowerValue = 0;

float CurrACValue = START_AC_VALUE;  // Объем АС в кубе
byte CurrACVolume = STOP_AC_VOLUME;  // Крепость спирта в струе
int CurrFlowSpeed = FLOW_SPEED;

// Описание типов экранов смотреть в меню в начале скетча
byte CurrScreen;          // Текущий экран
byte CurrMenuIthem = 10;  // Текущий пункт меню
byte ShowAlarTemp = 0;
byte ShowAtmPress = 0;
byte ShowSDiff = 0;

void setup(void) {
  // start serial port
  Serial.begin(9600);
  Serial.print(F ("Automatic Distillation ver "));
  Serial.println(F (VERSION));

  if (AUTO_POWER == 1) {
    pinMode(rmc3ShimPin, OUTPUT);
    pinMode(rmc3StopPin, OUTPUT);
    // Тут задается период ШИМа в секундах умноженный на 100 для точности регулировки мощности РМЦ в 1%
    // В данном примере это 200/100 = 2Гц или 0,5мс
    timerLoadValue = SetupTimer2(200);
  }

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  //tone(buzzPin, 2000, 500);
  //delay(1000);

  // Значения времени прошлого опроса
  timeLastCheckTemp = 0;
  timeLastCheckBrez = 0;

  InitSensors();
  InitBMP();

  CurrBrezenhemStep = 1;
  CurrBrezenhemSize = 0;

  // Включаем дисплей
  lcd.begin(16, 2);
  PrintStartLogo();

  CurrScreen = START_SCREEN;
  PrintScreen(CurrScreen);
}

// Переменные для работы ===============
boolean GetTempFromSensors(void)
{
  if (SensorIsActive == true) {
    sensors.requestTemperatures(); // Send the command to get temperatures
    float temp1 = sensors.getTempC(CubeThermometer);
    float temp2 = sensors.getTempC(DeflThermometer);
    float temp3 = 0;
    if (IsFoundAlarThermometr == 1) temp3 = sensors.getTempC(AlarThermometer);
    if (temp1 != -127 and temp2 != -127)
    {
      TempSensor_1 = temp1 + SENSOR_0;
      TempSensor_2 = temp2 + SENSOR_1;
      if (IsFoundAlarThermometr == 1 and temp3 != -127) {
        TempSensor_3 = temp3 + SENSOR_2;
      }
      else {
        TempSensor_3 = 0;
      }
      return true;
    }
    else
    {
      TempSensor_1 = 0;
      TempSensor_2 = 0;
      TempSensor_3 = 0;
      return false;
    }
  }
  else
  {
    //Serial.println("Sensors not active");
    return false;
  }
}

float GetStaticCubeTemp(void)
{
  byte i;
  float summ = 0.0;
  for (i = 0; i < TEMPERATURE_ARRAY; i = i + 1) {
    summ = summ + Temp0Array[i];
  }
  return summ / TEMPERATURE_ARRAY;
}

float GetStaticDeflTemp(void)
{
  byte i;
  float summ = 0.0;
  for (i = 0; i < TEMPERATURE_ARRAY; i = i + 1) {
    summ = summ + Temp1Array[i];
  }
  return summ / TEMPERATURE_ARRAY;
}

float GetStaticAlarTemp(void)
{
  byte i;
  float summ = 0.0;
  for (i = 0; i < TEMPERATURE_ARRAY; i = i + 1) {
    summ = summ + Temp2Array[i];
  }
  return summ / TEMPERATURE_ARRAY;
}
// =============== Переменные для работы

void loop(void)
{
  timeNow = millis();

  GetPressKey(CurrScreen);

  // Опрос температурных датчиков каджые 2 сек
  if ((timeNow - timeLastCheckTemp) >= 2000)
  {
    if (GetTempFromSensors() == true)
    {
      //Serial.println("Sensors active");
      // Проверка каждые 5 мин
      if ((timeNow - timeLastCheckCube) >= 300000 and GetStaticCubeTemp() > 80) //GetStaticCubeTemp() > 85
      {
        // Определяем текущее давление для корректировки Т кипения спирта
        if (IsFoundBMP180 == 1) CurrAtmPress = GetAtmPressure();

        // Для ТЕЛА и ХВОСТОВ, уменьшение отбора
        if (CurrLevel == 4 or CurrLevel == 5)
        {
          if (GetStaticCubeTemp() > 85)
          {
            // Вычисляем значение потока с учетом Т куба и кол-ва срабатываний старт-стопа
            tmpFlow = GetFlowShim(GetPowerTeloValue(), GetFNValue()) * (100 - GetStaticCubeTemp()) / 15 - StopCount;
          }
          else
          {
            tmpFlow = GetFlowShim(GetPowerTeloValue(), GetFNValue()) - StopCount;
          }
          // Минимальное ограничение ШИМ отбора 20%
          if (tmpFlow < 20) tmpFlow = 20;
          if (StopValue == 1)
          {
            // Запоминаем поправленное (уменьшенное) значение Брезенхема для восстановления отбора после стопа
            CurrBrezenhemSize = GetBrezVal(tmpFlow);
          }
          else
          {
            // Меняем значение Брезенхема для уменьшения отбора
            Brezenhem(GetBrezVal(tmpFlow), BrezenhemVal);
          }
        }

        // Для БРАГИ корректировка мощности ТЭНа
        if (CurrLevel == 7)
        {
          if (NBK_Auto == true)
          {
            CurrTempHkip = tempHkip();
          }
          else
          {
            CurrTempHkip -= 0.4;  // Убираем поправку на давление, т.к. эта темпратура уже получена при давлении!
          }
          Serial.print(F ("CurrTempHkip - "));
          if ((CurrTempHkip + 0.4 - GetStaticCubeTemp()) > BARDA_MAX_DLT)
          {
            if (CurrPowerValue < NBK_MAX_POWER) CurrPowerValue += round(HEATER_POWER / 100);
          }
          if ((CurrTempHkip + 0.4 - GetStaticCubeTemp()) < BARDA_MIN_DLT)
          {
            if (CurrPowerValue > NBK_MIN_POWER) CurrPowerValue -= round(HEATER_POWER / 100);
          }
        }

        timeLastCheckCube = timeNow;
      }

      // Средняя устоявшаяся Т куба
      Temp0Array[Temp0ArrayIthem] = TempSensor_1;
      Temp0ArrayIthem++;
      if (Temp0ArrayIthem == TEMPERATURE_ARRAY)
      {
        Temp0ArrayIthem = 0;
      }

      // Средняя устоявшаяся Т дефл
      Temp1Array[Temp1ArrayIthem] = TempSensor_2;
      Temp1ArrayIthem++;
      if (Temp1ArrayIthem == TEMPERATURE_ARRAY)
      {
        Temp1ArrayIthem = 0;
      }

      // Средняя устоявшаяся Т аварии
      Temp2Array[Temp2ArrayIthem] = TempSensor_3;
      Temp2ArrayIthem++;
      if (Temp2ArrayIthem == TEMPERATURE_ARRAY)
      {
        Temp2ArrayIthem = 0;
      }

      // Обработка перегрева дефлегматора
      if (GetStaticAlarTemp() > 45)
      {
        SetLevel(0);
      }

      // Триггер температуры куба
      if (TriggerCubeTemp == true and GetStaticCubeTemp() >= TriggerValue)
      {
        //TriggerCubeTemperature(GetStaticCubeTemp());
        TriggerCubeTemperature();
      }

      // Триггер времени работы
      if (TriggerTime == true and (timeNow - timeLastLevel) / 1000 >= TriggerValue)
      {
        //TriggerTimeValue(timeNow);
        //Serial.println("Trigger Time Active");
        //Serial.print("Trigger Value - ");
        //Serial.println(TriggerValue);
        TriggerTimeValue();
      }

      // Триггер дельты дефлегматора
      if (TriggerDeltaTemp == true and (GetStaticDeflTemp() - StartDeflTemp) >= (TriggerDeltaValue + tempSDiff(StartAtmPress)))
      {
        TempReplyDown = 0;
        TempReplyUp++;
        if (TempReplyUp == 3)
        {
          // Температура превышала порог роста 3 раз подряд, это залет! Тормозим отбор
          TriggerDeflDelta(0);
        }
      }
      else if (TriggerDeltaTemp == true and TempReplyUp > 0 and (GetStaticDeflTemp() - StartDeflTemp) < (TriggerDeltaValue + tempSDiff(StartAtmPress)))
      {
        TempReplyDown++;
        if (TempReplyDown == 15)
        {
          // Температура превышала порог снижения 15 раз подряд! Возобновляем отбор
          TriggerDeflDelta(1);
          TempReplyUp = 0;
          TempReplyDown = 0;
        }
      }

      // Триггер дефлегматора для CUBE
      if (TriggerDeflTemp == true and GetStaticDeflTemp() >= TriggerValue)
      {
        //TriggerCubeTemperature(GetStaticCubeTemp());
        TriggerDeflTemperature();
      }
    }

    // Отображение экранов
    if (CurrScreen == 2)
    {
      PrintScreen(2);
    }
    else if (CurrScreen == 3)
    {
      PrintScreen(3);
    }
    else if (CurrScreen == 5)
    {
      PrintScreen(5);
    }
    /*
      else if (CurrScreen == 6)
      {
      PrintScreen(6);
      }
    */

    timeLastCheckTemp = timeNow;
  }

  // Управление клапаном отбора каждые Время_Цикла/Дискретность_Брезенхема сек
  // Значение отбора задается в любом месте программы, здесь только его исполнение
  if ((timeNow - timeLastCheckBrez) >= 300000 / BrezenhemSize)
  {
    if (BrezenhemVal[CurrBrezenhemStep - 1] == 0)
    {
      digitalWrite(relayPin, HIGH);
      //Serial.print(F ("0"));
    }
    else
    {
      digitalWrite(relayPin, LOW);
      //Serial.print(F ("1"));
    }
    timeLastCheckBrez = timeNow;
    if (CurrBrezenhemStep < BrezenhemSize)
    {
      CurrBrezenhemStep++;
    }
    else
    {
      CurrBrezenhemStep = 1;
      //Serial.println(F ("."));
      //Serial.println(F ("---"));
      //Serial.println(F (""));
    }
  }
}


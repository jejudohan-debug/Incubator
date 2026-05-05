#include <Arduino.h>
#include "D_Logic/ActionLogic.h"

ActionLogic _logic;

void setup()
{
  // MCUSR = 0; // MCU 상태 레지스터 초기화
  // dt_disable();

  // 2. 다른 부품들(LCD, RTC)이 안정화될 시간을 줍니다.
  // delay(500);

  Serial.begin(9600);
  Serial.println(F("main Setup"));

  Wire.begin();
  Wire.setWireTimeout(3000, true);

  _logic.init();

  wdt_enable(WDTO_4S); // 4초 타임아웃 설정
}

void loop()
{
  _logic.update();

  wdt_reset();
}

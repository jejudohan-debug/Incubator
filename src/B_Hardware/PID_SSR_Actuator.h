#pragma once

#include <Arduino.h>
#include "A_Core/SystemContext.h"

class PID_SSR_Actuator
{
private:
  uint8_t _pin;
  bool _activeLow;
  uint16_t _output; // PID로부터 전달받을 0~1000 사이의 값
  bool _state = false; // 실제 SSR의 현재 상태 (On/Off)

  //DisplayState &_view = SystemContext::getInstance().getView();

  void controlHeater(uint16_t pidOutput);

public:
  PID_SSR_Actuator(uint8_t pin, bool activeLow = false)
      : _pin(pin), _activeLow(activeLow), _output(0.0) {}

  void init();

  // 실제 SSR 제어: PWM 방식을 사용하거나 Time-Proportioning 방식을 사용
  void update();
  void stop();
  void setOutput(int16_t val);
  bool isRunning() const { return _state; }
};

/*/ PID AutoTune
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include <DHT.h>

// 핀 설정
#define DHTPIN 2
#define SSR_PIN 3
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// PID 변수
double Input, Output, Setpoint;
double kp = 2, ki = 0.5, kd = 2; // 초기값 (의미는 없지만 라이브러리 구동용)

PID myPID(&Input, &Output, &Setpoint, kp, ki, kd, DIRECT);
PID_ATune aTune(&Input, &Output);

// 상태 관리
bool tuning = true;
unsigned long lastReadTime = 0;
const unsigned long READ_INTERVAL = 3000; // 3초 주기

void setup() {
  Serial.begin(9600);
  dht.begin();

  Setpoint = 37.5; // 목표 온도

  // 튜닝 설정
  aTune.SetControlLookAhead(20); // 노이즈 방지 (값이 클수록 보수적)
  aTune.SetNoiseBand(0.2);       // 센서 노이즈 허용 오차 (DHT22는 0.1~0.2 적당)
  aTune.SetOutputStep(100);      // 튜닝 시 변화시킬 출력 폭 (0~255 사이)
  aTune.SetLookbackSec(30);      // 과거 데이터 참조 시간

  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(3000);

  Serial.println("--- PID AutoTune 시작 ---");
  Serial.println("온도가 목표치 근처에서 출렁이며 값을 계산합니다. 수 분이 소요될 수 있습니다.");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastReadTime >= READ_INTERVAL) {
    lastReadTime = currentTime;
    float temp = dht.readTemperature();

    if (!isnan(temp)) {
      Input = (double)temp;

      if (tuning) {
        // --- 자동 튜닝 모드 ---
        byte val = aTune.Runtime();

        if (val != 0) { // 튜닝 완료 시
          tuning = false;
          kp = aTune.GetKp();
          ki = aTune.GetKi();
          kd = aTune.GetKd();

          Serial.println("--- 튜닝 완료! ---");
          Serial.print("Kp: "); Serial.println(kp);
          Serial.print("Ki: "); Serial.println(ki);
          Serial.print("Kd: "); Serial.println(kd);

          myPID.SetTunings(kp, ki, kd); // 계산된 값을 PID에 적용
        }
        analogWrite(SSR_PIN, Output); // 튜닝 중에는 aTune이 Output을 제어함
      } else {
        // --- 실제 PID 운전 모드 ---
        myPID.Compute();
        analogWrite(SSR_PIN, Output);
      }

      // 모니터링
      Serial.print("Temp: "); Serial.print(Input);
      Serial.print(" | Out: "); Serial.println(Output);
    }
  }
}
*/
#include <Arduino.h>

// 온습도 데이터가 10배 곱해진 uint16_t라고 가정 (예: 375 = 37.5도)
//uint16_t currentTemp = 360; 
//uint16_t setpointTemp = 375;

// PID 게인값도 정수로 설정 (실제 값에 10을 곱한 값)
int32_t Kp_int = 50;  // 실제 Kp가 5.0라면 5.0 * 10 = 50
int32_t Ki_int = 1;   // 실제 Ki가 0.1이라면 0.1 * 10 = 1
int32_t Kd_int = 5;  // 실제 Kd가 1.0이라면 1.0 * 10 = 10

int32_t integral = 0;
int32_t lastError = 0;

int16_t computeIntegerPID(uint16_t target, uint16_t current) {
    int32_t error = (int32_t)target - (int32_t)current;

    int32_t pTerm = Kp_int * error;

    static int16_t lastOutput = 0;
    if (lastOutput < 255 || error < 0) {
        integral += error;
    }
    integral = constrain(integral, -2000, 2000); 
    int32_t iTerm = Ki_int * integral;

    int32_t dTerm = Kd_int * (error - lastError);
    int32_t total = (pTerm + iTerm + dTerm + 50) / 100;

    lastOutput = (int16_t)constrain(total, 0, 255);
    lastError = error;

    return lastOutput;
}

/*void loop() {
    // 3초마다 실행 로직 생략...
    int16_t ssrOut = computeIntegerPID(setpointTemp, currentTemp);
    analogWrite(3, ssrOut);
}
    
// loop 함수 내부에 추가
if (currentTemp > 390) { // 39.0도 초과 시
    analogWrite(SSR_PIN, 0); // 히터 강제 종료
} else {
    int16_t output = computeIntegerPID(Setpoint, currentTemp);
    analogWrite(SSR_PIN, output);
}
    
*/
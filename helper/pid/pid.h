#pragma once
#include "../timer/timer.h"

class PID {
private:
    Timer timer;
    float integral = 0;
    float min;
    float max;
    float lastError = 0;

    float p, i, d;

    float calcIntegral(float error, float dt);
    float calcDerivative(float error, float dt);
    float calcProportional(float error);
public:
    

    PID(float p, float i, float d, float min, float max);
    float calculate(float error);
};

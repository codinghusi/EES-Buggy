#include "pid.h"

PID::PID(float p, float i, float d, float min, float max): p(p), i(i), d(d), min(min), max(max)
{
    timer.start();
}

float PID::calculate(float error) {
    float derivative, proportional, dt;
    dt = timer.measure();
    integral += calcIntegral(error, dt);
    derivative = calcDerivative(error, dt);
    proportional = calcProportional(error);
    float output = p * proportional + i * integral + d * derivative;
    if (output > max) {
        output = max;
    } else if (output < min) {
        output = min;
    }
    lastError = error;
    timer.start();
    return output;
}

float PID::calcIntegral(float error, float dt) {
    return error * dt;
}

float PID::calcDerivative(float error, float dt) {
    return (error - lastError) / dt;
}

float PID::calcProportional(float error) {
    return error;
}

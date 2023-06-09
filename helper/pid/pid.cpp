#include "pid.h"

PID::PID(float p, float i, float d, float min, float max): p(p), i(i), d(d), min(min), max(max)
{
    timer.start();
}

float PID::calculate(float error) {
    float c = 1 - (error) * (1.f / 1.f);
    return c < 0 ? 0 : c;
}

// float PID::calculate(float error) {

//     // test *= error / lastError;
//     // return test;

//     float derivative, proportional, dt;
//     dt = timer.getElapsedSeconds();
//     integral += calcIntegral(error, dt);
//     derivative = calcDerivative(error, dt);
//     proportional = calcProportional(error);
//     float output = p * proportional + i * integral + d * derivative;
//     if (output > max) {
//         output = max;
//     } else if (output < min) {
//         output = min;
//     }
//     lastError = error;
//     timer.start();
//     return output;
// }

float PID::calcIntegral(float error, float dt) {
    return error * dt;
}

float PID::calcDerivative(float error, float dt) {
    return (error - lastError) / dt;
}

float PID::calcProportional(float error) {
    return error;
}

void PID::resetTime() {
    timer.start();
}

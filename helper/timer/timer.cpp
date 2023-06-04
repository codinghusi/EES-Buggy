#include "timer.h"

void Timer::start() {
    lastTime = std::chrono::steady_clock::now();
}

// in seconds
float Timer::measure() {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = now - lastTime;
    return duration.count();
}

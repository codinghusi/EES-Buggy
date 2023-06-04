#pragma once
#include <chrono>

class Timer {
private:
    std::chrono::steady_clock::time_point lastTime;

public:
    void start();
    float measure(); // in seconds
};

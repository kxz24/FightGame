#pragma once
#include <chrono>

class Timer {
public:
    Timer();
    void restart();
    double elapsed() const; // √Î
private:
    std::chrono::steady_clock::time_point start_;
};
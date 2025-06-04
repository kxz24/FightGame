#include "Timer.h"

Timer::Timer() { restart(); }
void Timer::restart() {
    start_ = std::chrono::steady_clock::now();
}
double Timer::elapsed() const {
    return std::chrono::duration<double>(
        std::chrono::steady_clock::now() - start_).count();
}
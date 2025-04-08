#include <GLFW/glfw3.h>
#include <iostream>

#include "timer.h"

namespace game {

Timer::Timer(void) {
    start_time_ = 0.0;
    end_time_ = 0.0;
}


Timer::~Timer(void) {

}


void Timer::Start(float end_time) {
    // get current time and calculate corresponding ending time
    start_time_ = glfwGetTime();
    end_time_ = start_time_ + end_time;
}


bool Timer::Finished(void) const {
    return glfwGetTime() >= end_time_;
}

double Timer::TimeLeft(void) const {
    return end_time_ - glfwGetTime();
}

} // namespace game

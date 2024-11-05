#include <iostream>
#include <chrono>
#include <gpiod.hpp>

#include "include_cpp_file/motors.cpp"

gpiod::chip chip("gpiochip0");

Big_Stepper_motor big_motor_row0(23, 24, chip);
Small_Stepper_motor small_motor_row0(20, 21, chip);

// auto thread_start_time = std::chrono::high_resolution_clock::now();

int main() {


    small_motor_row0.go_to_angle(-45);

    usleep(1000000); // 1000ms

    small_motor_row0.go_to_angle(0);

    std::cout << "Hello World" << std::endl;
    return 0;
}

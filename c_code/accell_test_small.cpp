#include <iostream>
#include <tgmath.h>

// For the usleep
#include <chrono>

// For GPIO
#include <gpiod.hpp>
#include <thread>

// For the Motors, Rows and The deque
#include "include_cpp_file/motors.cpp"

gpiod::chip chip("gpiochip0");

int pulse_pin = 20;
int dir_pin = 21;

gpiod::line pulse_line;
gpiod::line dir_line;

// Should be 1600. What is this? 1650
int steps_per_rev = 1600;
int sleep_time = 10000; // 0.1 us

// Retrieve the line handles

int revs = 1;


float f(int x) {
    return 1/x;
}



int main() {

    std::cout << "Version 3: " << std::endl;

    pulse_line = chip.get_line(pulse_pin);
    dir_line = chip.get_line(dir_pin);

    // This the string needed to request the line?
    dir_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});
    pulse_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});

    dir_line.set_value(0);

    for (size_t i {0}; i < 100; ++i) {
        for (size_t j = 0; j < revs*(steps_per_rev); ++j) {
            pulse_line.set_value(1);
            std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_time));
            pulse_line.set_value(0);
            std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_time));

        }
        if (i < 10) {
            sleep_time = sleep_time - 1500;
        }
        std::cout << sleep_time << std::endl;
    }

    pulse_line.release();
    dir_line.release();

    std::cout << "End of file" << std::endl;
    return 0;
}
#include <iostream>
#include <array>
#include <vector>
#include <unistd.h>

#include <gpiod.hpp>
// Absolute path. Should be reviced under revision
// #include "/include_cpp_file/motors.cpp"
#include "library/motors.cpp"

gpiod::chip chip("gpiochip0");

// This needs to be better organized in the future.
int big_motor1_pulse_pin = 23;
int big_motor1_dir_pin = 24;    

int small_motor1_pulse_pin = 20;
int small_motor1_dir_pin = 21;

// Open the GPIO chip
// Connect to the GPIO line
Big_Stepper_motor big_motor1(big_motor1_pulse_pin, big_motor1_dir_pin, chip);
Small_Stepper_motor small_motor1(small_motor1_pulse_pin, small_motor1_dir_pin, chip);


int main() {
    // big_motor1.steps_opperate(10, 1);
    // usleep(100000); // 100 milisec

    // 0 er vekk fra bordet
    // 1 er mot bordet

    // try {
        // for (size_t i = 0; i < 1000; i++)
        // {
        //     big_motor1.steps_opperate(1, 1);
        //     usleep(10000); // 10ms
        // }
        big_motor1.steps_opperate(0, 0);
        // small_motor1.steps_opperate(200, 0);
        // small_motor1.steps_opperate(1600/8, 1);
        // for (size_t i = 1; i < 100; i++) {
        // big_motor1.steps_opperate(1050, 1);
        // big_motor1.steps_opperate(1050, 0);
        // // usleep(1000000); // 1000ms
        // big_motor1.steps_opperate(2100, i%2);
        // }

        sleep(3);

        // big_motor1.reset();

    // big_motor1.steps_opperate(10, 1);

        // big_motor1.steps_opperate(500, 1);

        // big_motor1.steps_opperate(300, 1);


        // big_motor1.steps_opperate(2100, 1);

        // big_motor1.steps_opperate(400, 0);
        // small_motor1.steps_opperate(50, 1);
    // }
    // catch (const std::exception &e)
    // {
    //     std::cout << e.what() << '\n'; // First run, then remove
    //     // Important, release the GPIO chip
    // }
    // gpiod_chip_close(chip);
    std::cout << "End of file" << std::endl;
    return 0;
}

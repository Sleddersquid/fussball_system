#include <iostream>
#include <array>
#include <vector>
#include <unistd.h>

#include <gpiod.hpp>
// Absolute path. Should be reviced under revision
#include "include_cpp_file/motors.cpp"

gpiod::chip chip("gpiochip0");

// This needs to be better organized in the future.
int big_motor1_pulse_pin = 20;
int big_motor1_dir_pin = 21;    

int small_motor1_pulse_pin = 23;
int small_motor1_dir_pin = 24;

// Open the GPIO chip
// Connect to the GPIO line
Big_Stepper_motor big_motor1(big_motor1_pulse_pin, big_motor1_dir_pin, chip, 0);
Small_Stepper_motor small_motor1(small_motor1_pulse_pin, small_motor1_dir_pin, chip, 0);

// Stepper_motor big_motor2(big_motor2_pin, big_motor2_pin, chip);
// Stepper_motor small_motor2(small_motor2_pin, small_motor2_pin, chip);

// Make array of GPIO lines
//std::array<Stepper_motor, 2> motors = {big_motor1, small_motor1};

int main()
{
    usleep(100000); // 100 milisec

    // 0 er vekk fra bordet
    // 1 er mot bordet


    try {
        // for (int i = 0; i < 5; i++) {
        //     big_motor1.opperate(1, i%2);
        //     // small_motor1.go_to_angle(i*90);

        //     usleep(100000); // 1 sec

        // }

        // big_motor1.steps_opperate(1200, 0);
        small_motor1.steps_opperate(10000, 1);
    


    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n'; // First run, then remove
        // Important, release the GPIO chip
    }

    // gpiod_chip_close(chip);
    std::cout << "End of file" << std::endl;
    return 0;
}

#include <iostream>
#include <array>
#include <vector>
#include <unistd.h>

#include <gpiod.hpp>
// Absolute path. Should be reviced under revision
#include "library/main.hpp"

::gpiod::chip chip("gpiochip0");

// This needs to be better organized in the future.
int big_motor1_pulse_pin = 23;
int big_motor1_dir_pin = 24;    

int small_motor1_pulse_pin = 20;
int small_motor1_dir_pin = 21;

// Open the GPIO chip
// Connect to the GPIO line
Big_Stepper_motor big_motor1(big_motor1_pulse_pin, big_motor1_dir_pin, chip);
Small_Stepper_motor small_motor1(small_motor1_pulse_pin, small_motor1_dir_pin, chip);

// Stepper_motor big_motor2(big_motor2_pin, big_motor2_pin, chip);
// Stepper_motor small_motor2(small_motor2_pin, small_motor2_pin, chip);

// Make array of GPIO lines
//std::array<Stepper_motor, 2> motors = {big_motor1, small_motor1};

int main()
{
    // If the chip is not opened, print an error message and exit program with error code 1
    usleep(100000); // 100 milisec

    try{
        for (int i = 0; i < 2; i++)
        {
	            big_motor1.opperate(1, 0);
                small_motor1.opperate(1, 0);
                usleep(100000); // 100 ms
                big_motor1.opperate(1, 1);
                small_motor1.opperate(1, 1);
                usleep(100000); // 100 ms
        }
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

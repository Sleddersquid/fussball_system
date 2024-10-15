#include <iostream>
#include <array>
#include <vector>

// For the threads and such
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// For GPIO and OpenCV
#include <gpiod.hpp>
#include <opencv2/opencv.hpp>

// For the motors and the deque
#include "include_cpp_file/motors.cpp"
#include "library/deque_extra.hpp"


// Opening the GPIO chip
gpiod::chip chip("gpiochip0");

// First row
Big_Stepper_motor big_motor_row0(20, 21, chip, 0);
Small_Stepper_motor small_motor_row0(23, 24, chip ,0);







int main()
{

    try
    {
        while (true) {
            // Do something
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    





    // There looks like there is no need to close the gpio chip.
    // Only need to release the lines, and that is done in the destructor of the motor class
    std::cout << "End of file" << std::endl;
    return 0;
}

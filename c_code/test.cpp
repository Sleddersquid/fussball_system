#include <iostream>
#include <chrono>
#include "include_cpp_file/motors.cpp"


gpiod::chip chip("gpiochip0");

Big_Stepper_motor big_motor_row0(23, 24, chip, 0);
Small_Stepper_motor small_motor_row0(20, 21, chip, 0); 

int main() {


    small_motor_row0.opperate(1, 0);
    small_motor_row0.opperate(1, 1);


    return 0;
}
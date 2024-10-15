#include <iostream>
#include <string>
#include <unistd.h>

#include <gpiod.hpp>

#include "../library/motors.hpp"
// #include "motors.hpp"

// The big motors has a limit of how many steps it can take


// ------------------------------ BIG MOTOR ------------------------------ //

/**
 * @brief 
 * @param pulse_pin - The pin that the pulse signal is connected to
 * @param dir_pin - The pin that the direction signal is connected to
 * @param chip - The GPIO chip that the pins are connected to
 * @param row - The row of the motor
 */
Big_Stepper_motor::Big_Stepper_motor(int pulse_pin, int dir_pin, gpiod::chip chip, int row)
    : m_pulse_pin(pulse_pin), m_dir_pin(dir_pin) , m_row(row) {

    // Retrieve the line handles
    this->pulse_line = chip.get_line(pulse_pin);
    this->dir_line = chip.get_line(dir_pin);

    // This the string needed to request the line?
    dir_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});
    pulse_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});
};

Big_Stepper_motor::~Big_Stepper_motor() {
        pulse_line.release();
        dir_line.release();
};

// Trying with boolean. But try the python module first
// This function makes the motor go in a certain direction in certain direction
void Big_Stepper_motor::opperate(int revs, bool dir) {
    // Check if it will go over under the limit. If it does, throw an exception
    if (steps_taken + revs*(steps_per_rev) > max_steps || steps_taken - revs*(steps_per_rev) < 0) {
        throw MAX_LIMIT_FOR_STEPS_REACHED();
    }

    steps_taken = steps_taken + revs*(steps_per_rev) * (dir ? -1 : 1);
    // Set the direction
    this->dir_line.set_value(dir);
    // Pulse for the motor stepper
    for (int i = 0; i < revs*(steps_per_rev); i++) {
        this->pulse_line.set_value(1);
        usleep(sleep_time); // 100 us
        this->pulse_line.set_value(0);
        usleep(sleep_time); // 100 us
    }
};


void Big_Stepper_motor::steps_opperate(int steps, bool dir) {
    // Check if it will go over under the limit, if it does, throw an exception
    if (steps_taken + steps > max_steps || steps_taken - steps < 0) {
        throw MAX_LIMIT_FOR_STEPS_REACHED();
    }

    // Since forwards is 0 and backwards 1, we need to add or subtract the steps, depending on the direction
    // Should instead use -1**dir, but since it uses a function, i found this more convinient
    steps_taken = steps_taken + steps * (dir ? -1 : 1);
    // Set the direction
    this->dir_line.set_value(dir);
    // Pulse for the motor stepper
    for (int i = 0; i < steps; i++) {
        this->pulse_line.set_value(1);
        usleep(sleep_time); // 100 us
        this->pulse_line.set_value(0);
        usleep(sleep_time); // 100 us
    }
};

int Big_Stepper_motor::get_row() {
    return this->m_row;
};





// The small motor has no limit of how many steps it can take



// ------------------------------ SMALL MOTOR ------------------------------ //
Small_Stepper_motor::Small_Stepper_motor(int pulse_pin, int dir_pin, gpiod::chip chip, int row) 
    : m_pulse_pin(pulse_pin), m_dir_pin(dir_pin) , m_row(row) {
    
    // Retrieve the line handles
    this->pulse_line = chip.get_line(pulse_pin);
    this->dir_line = chip.get_line(dir_pin);

    // This the string needed to request the line?
    dir_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});
    pulse_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});
};

Small_Stepper_motor::~Small_Stepper_motor() {
        pulse_line.release();
        dir_line.release();
};

// Trying with boolean. But try the python module first
void Small_Stepper_motor::opperate(int revs, bool dir) {
    // Set the direction
    this->dir_line.set_value(dir);
    // Pulse the motor
    for (int i = 0; i < revs*(this->steps_per_rev); i++) {
        this->pulse_line.set_value(1);
        usleep(1); // 100 us
        this->pulse_line.set_value(0);
        usleep(1); // 100 us
    }
};

int Small_Stepper_motor::get_row() {
    return this->m_row;
};
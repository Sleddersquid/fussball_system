#include <iostream>
#include <string>
#include <unistd.h>
#include <math.h>

#include <gpiod.hpp>

#include "motors.hpp"

// The big motors has a limit of how many steps it can take

// ------------------------------ BIG MOTOR ------------------------------ //

/**
 * @brief 
 * @param pulse_pin - The pin that the pulse signal is connected to
 * @param dir_pin - The pin that the direction signal is connected to
 * @param chip - The GPIO chip for opening the lines 
 */
Big_Stepper_motor::Big_Stepper_motor(int pulse_pin, int dir_pin, gpiod::chip chip)
    : m_pulse_pin(pulse_pin), m_dir_pin(dir_pin) {

    // Retrieve the line handles
    this->pulse_line = chip.get_line(pulse_pin);
    this->dir_line = chip.get_line(dir_pin);

    this->m_last_coord = this->m_start_coord;

    // This the string needed to request the line?
    dir_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});
    pulse_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});
};

Big_Stepper_motor::~Big_Stepper_motor() {
        // std::cout << this->m_last_coord << std::endl;
        pulse_line.release();
        dir_line.release();
};

float Big_Stepper_motor::smoothnging(int new_min, int new_max, int old_min, int old_max, int x) {
    return new_min + ((x - old_min) / (old_max - old_min)) * (new_max - new_min);
};



void Big_Stepper_motor::opperate(int revs, bool dir) {
    // Check if it will go over under the limit. If it does, throw an exception
    // if (steps_taken + revs*(steps_per_rev) > max_steps || steps_taken - revs*(steps_per_rev) < 0) {
    //     throw MAX_LIMIT_FOR_STEPS_REACHED();
    // }

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
    this->dir_line.set_value(0);
};


void Big_Stepper_motor::steps_opperate(int steps, bool dir) {
    // Check if it will go over under the limit, if it does, throw an exception
    if (steps_taken + steps*(dir ? -1 : 1) > max_steps) {
        throw MAX_LIMIT_FOR_STEPS_REACHED();
    }

    steps_taken = steps_taken + steps * (dir ? 1 : -1);
    // Since forwards is 0 and backwards 1, we need to add or subtract the steps, depending on the direction
    // Should instead use -1**dir, but since it uses a function, i found this more convinient
    // Set the direction
    this->dir_line.set_value(dir);
    // Pulse for the motor stepper
    for (int i = 0; i < steps; i++) {
        this->pulse_line.set_value(1);
        usleep(sleep_time); // 100 us
        this->pulse_line.set_value(0);
        usleep(sleep_time); // 100 us
    }
    this->dir_line.set_value(0);
    // std::cout << "Steps taken: " << steps_taken << std::endl;
};

void Big_Stepper_motor::go_to_angle(int new_angle) {
    // d_new - d_old = angle to move to
    // The direction the motor should go. False (0) is forward, True (1) is backwards
    // Have to cumpute this first
    int angle = new_angle - this->m_last_angle;
    int steps = int(abs(angle)*(steps_per_rev)/360);

    // if (steps_taken + steps > max_steps || steps_taken - steps < 0) {
    //     throw MAX_LIMIT_FOR_STEPS_REACHED();
    // }

    int dir = 0;
    // 0 is forwards, 1 is backwards
    if (angle > 0) { // if positive, go backwards (counter_clockwise)
        dir = 1;
    } // Else dir stays zero and forwards (clockwise)

    // Steps needed to go to the angle
    // Since angle can be negative, steps cannot. Therefor abs(angle)
    
    this->steps_opperate(steps, dir);

    this->m_last_angle = new_angle;
    this->dir_line.set_value(0);
};

void Big_Stepper_motor::go_to_coord(int new_coord) {
    // Add jitter to the motor (10 steps?)
    
    if(new_coord > m_end_coord) {
        new_coord = this->m_end_coord;
    }
    if (new_coord < m_start_coord) {
        new_coord = this->m_start_coord;
    }

    int coord = new_coord - this->m_last_coord;
    // float smoothing = (this->m_last_coord - this->m_start_coord)/coord;

    // If coord positive, dir = 1
    int dir = 0;
    if (coord > 0) {
        dir = 1;
    }

    int steps = abs(coord) * steps_per_coord;

    // if coord less than 10, dont opperate motor. 
    // if (steps < 19) {
    //     return; 
    // }

    // if (steps_taken + steps*(dir ? -1 : 1) > max_steps) {
    //     // throw MAX_LIMIT_FOR_STEPS_REACHED();
    //     std::cout << "Max limit reached" << std::endl;
    //     this->reset();
    // }

    this->steps_taken = this->steps_taken + steps * (dir ? -1 : 1);

    this->dir_line.set_value(dir);

    // float smmoo = smoothnging(1, 5, 0, 2100, steps);

    for (int i = 0; i < steps; i++) {
        this->pulse_line.set_value(1);
        usleep(sleep_time); // 100 us
        this->pulse_line.set_value(0);
        usleep(sleep_time); // 100 us
    }
    
    this->m_last_coord = new_coord;
    this->dir_line.set_value(0);
};

void Big_Stepper_motor::reset() {
    // std::cout << "Steps taken: " << this->steps_taken << std::endl;
    // this->go_to_angle(0);
    // this->go_to_coord(m_end_coord);

    this->dir_line.set_value(0);
    for (int i = 0; i < abs(this->steps_taken); i++) {
        this->pulse_line.set_value(1);
        usleep(sleep_time); // 100 us
        this->pulse_line.set_value(0);
        usleep(sleep_time); // 100 us
    }

    // this->dir_line.set_value(0);
    this->steps_taken = 0;
};



// The small motor has no limit of how many steps it can take
// ------------------------------ SMALL MOTOR ------------------------------ //
Small_Stepper_motor::Small_Stepper_motor(int pulse_pin, int dir_pin, gpiod::chip chip) 
    : m_pulse_pin(pulse_pin), m_dir_pin(dir_pin) {
    
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

void Small_Stepper_motor::opperate(int revs, bool dir) {
    // Set the direction
    this->dir_line.set_value(dir);
    // Pulse the motor
    for (int i = 0; i < revs*(this->steps_per_rev); i++) {
        this->pulse_line.set_value(1);
        usleep(sleep_time);
        this->pulse_line.set_value(0);
        usleep(sleep_time);
    }
    this->dir_line.set_value(0);
};

void Small_Stepper_motor::steps_opperate(int steps, bool dir) {
    // Set the direction
    this->dir_line.set_value(dir);
    // Pulse the motor
    for (int i = 0; i < steps; i++) {
        this->pulse_line.set_value(1);
        usleep(sleep_time);
        this->pulse_line.set_value(0);
        usleep(sleep_time);
        // std::cout << "Step: " << i << std::endl;
    }
    this->dir_line.set_value(0);
};

void Small_Stepper_motor::go_to_angle(int new_angle_deg) {
    
    // d_new - d_old = angle to move to
    // The direction the motor should go. False (0) is forward, True (1) is backwards
    // Have to cumpute this first
    int angle = new_angle_deg - this->m_last_angle;
    int steps = int(abs(angle)*(steps_per_rev)/360);

    // if (steps < 100) {
    //     return;
    // }

    int dir = 0;
    // 0 is forwards, 1 is backwards
    if (angle > 0) { // if positive, go backwards (counter_clockwise)
        dir = 1;
    } // Else dir stays zero and forwards (clockwise)

    // Steps needed to go to the angle
    // Since angle can be negative, steps cannot. Therefor abs(angle)
    
    this->steps_opperate(steps, dir);
    // std::cout << "Steps: " << steps << ", dir: " << dir << std::endl;

    this->m_last_angle = new_angle_deg;
    this->dir_line.set_value(0);
};

void Small_Stepper_motor::reset() {
    this->go_to_angle(0);
};
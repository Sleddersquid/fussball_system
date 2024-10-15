#ifndef MYCLASSES_HPP  // Include guard to prevent multiple inclusions
#define MYCLASSES_HPP


#include <iostream>
#include <string>
#include <unistd.h>

#include <gpiod.hpp>

// These classes are used for custom errors
class MAX_LIMIT_FOR_STEPS_REACHED {};


class Big_Stepper_motor {
private:
    int m_pulse_pin;
    int m_dir_pin;

    int m_row;

    gpiod::line pulse_line;
    gpiod::line dir_line;

    // Should be 1600. What is this?
    int steps_per_rev = 1650;
    float sleep_time = 0.000040; // 40 us
    int steps_taken = 0;

    // Sets a threshold of 2000 steps
    // This is to contrain the motor to not be able to go over or under the limit of steps
    // max number of steps is between [0, 2000] 
    int max_steps = 2000;

public:
    Big_Stepper_motor(int pulse_pin, int dir_pin, gpiod::chip chip, int row);

    ~Big_Stepper_motor();

    void opperate(int revs, bool dir);

    void steps_opperate(int steps, bool dir);

    int get_row();
};

class Small_Stepper_motor {
private:
    int m_pulse_pin;
    int m_dir_pin;

    int m_row;

    gpiod::line pulse_line;
    gpiod::line dir_line;

    // Should be 1600. What is this?
    int steps_per_rev = 1610;
    float sleep_time = 0.000040; // 40 us
    int steps_taken = 0;

public:
    Small_Stepper_motor(int pulse_pin, int dir_pin, gpiod::chip chip, int row);

    ~Small_Stepper_motor();

    void opperate(int revs, bool dir);

    // Don't think this one is needed. Maybe just do full revoloutions
    // void steps_opperate(int steps, bool dir);

    int get_row();
};

#endif // MYCLASSES_HPP
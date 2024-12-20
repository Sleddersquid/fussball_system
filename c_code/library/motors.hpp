#pragma once

#include <iostream>
#include <string>
#include <unistd.h>

#include <gpiod.hpp>

// #include "motors.cpp"

// These classes are used for custom errors
class MAX_LIMIT_FOR_STEPS_REACHED {};

// ------------------------------ MOTORS ------------------------------ //
// The difference between the small and big motors are
// 1 - The big motors needs a bigger delay between the pulses than the small motor
// 2 - The big motors has a limit of how many steps it can take. The small motors does not have this limit.

/**
 * @brief This motor has a limit of how many steps it can take, and a bigger delay between the pulses.
 * @param pulse_pin - The pin (GPIO) that the pulse signal is connected to
 * @param dir_pin - The pin (GPIO) that the direction signal is connected to
 * @param chip - The GPIO chip for opening the lines 
 */
class Big_Stepper_motor {
private:
    int m_pulse_pin;
    int m_dir_pin;

    gpiod::line pulse_line;
    gpiod::line dir_line;

    // Should be 1600. What is this? 1650
    int steps_per_rev = 1600;
    float sleep_time = 73; // In us (nanoseconds)
    int steps_taken = 0;

    float steps_per_coord = 7;

    // Sets a threshold of 2000 steps
    // This is to contrain the motor to not be able to go over or under the limit of steps
    // max number of steps is between [0, 2100] 
    int max_steps = 290;
    
    int m_last_angle = 0;
    int m_last_coord = 0;

    int m_start_coord = 455; // 161, 440, Works with 424
    int m_end_coord = 813; // 1217, 800, Works with 777

public:
    Big_Stepper_motor(int pulse_pin, int dir_pin, gpiod::chip chip);

    /**
     * @brief Releases the pulse and the dir line when destroyed
     */
    ~Big_Stepper_motor();

    /**
     * @brief Operates the motor in revolutions (360 degrees). Is limited to 2000 steps.
     * 1 is into the table, and 0 is away from the table
     * @param revs - The number of revolutions the motor should do.
     * @param dir - The direction the motor should go. False is forward, True is backwards
     */
    void opperate(int revs, bool dir);


    /**
     * @brief Operates the motor in steps (0.225 degrees). Is limited to max_steps
     * @param steps - The number of steps the motor should do.
     * @param dir - The direction the motor should go. False (0) is forward, True (1) is backwards
     */
    void steps_opperate(int steps, bool dir);


    /**
     * @brief Makes the motor go to a certain angle, given theta (in degrees). Is limited to max_steps
     * @param angle - The angle the motor should go to.
     */
    void go_to_angle(int theta);


    /**
     * @brief Makes the motor go to a certain coordinate, given x. Is limited to max_steps
     * @param x - The coordinate the motor should go to.
     */
    void go_to_coord(int new_coord);

    /**
     * @brief Resets the motor to the starting position, that being all the way way back. 
     */
    void reset();

    void move_to_pos(double new_coord);
};


/**
 * @brief This motor doesn't have a limit of how many steps it can take, and smaller delay between the pulses.
 * 1 is into the table, and 0 is away from the table
 * @param pulse_pin - The pin (GPIO) that the pulse signal is connected to
 * @param dir_pin - The pin (GPIO) that the direction signal is connected to
 * @param chip - The GPIO chip for opening the lines 
 */
class Small_Stepper_motor {
private:
    int m_pulse_pin;
    int m_dir_pin;

    gpiod::line pulse_line;
    gpiod::line dir_line;

    // Should be 1600. What is this? // 1610
    int steps_per_rev = 1600;
    float sleep_time = 1;  // In us (nanoseconds)
    int steps_taken = 0;

    int m_last_angle = 0;

public:
    Small_Stepper_motor(int pulse_pin, int dir_pin, gpiod::chip chip);

    ~Small_Stepper_motor();

    /**
     * @brief Operates the motor in revolutions (360 degrees). Is has no limit.
     * 1 is into the table, and 0 is away from the table.
     * @param revs - The number of revolutions the motor should do.
     * @param dir - The direction the motor should go. False is forward, True is backwards
     */
    void opperate(int revs, bool dir);

    // Don't think this one is needed. Maybe just do full revoloutions
    void steps_opperate(int steps, bool dir);

    void go_to_angle(int new_angle);

    void reset();
};

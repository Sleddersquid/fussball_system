#include <gpiod.hpp>
#include <iostream>
#include <unistd.h>

class Big_Stepper_motor {
private:
    int pulse_pin;
    int dir_pin;
    gpiod::line pulse_line;
    gpiod::line dir_line;

    // Should be 1600. What is this?
    int steps_per_rev = 1650;

public:
    Big_Stepper_motor(int pulse_pin, int dir_pin, gpiod::chip chip) : pulse_pin(pulse_pin), dir_pin(dir_pin) {
        // Retrieve the line handles

        this->pulse_line = chip.get_line(pulse_pin);
        this->dir_line = chip.get_line(dir_pin);

        dir_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});
        pulse_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});
    }

    ~Big_Stepper_motor() {
            pulse_line.release();
            dir_line.release();
    }

    // Trying with boolean. But try the python module first
    void opperate(int revs, bool dir) {
        // Set the direction
        this->dir_line.set_value(dir);
        // Pulse the motor
        for (int i = 0; i < revs*(this->steps_per_rev); i++) {
            this->pulse_line.set_value(1);
            usleep(40); // 100 us
            this->pulse_line.set_value(0);
            usleep(40); // 100 us
        }
    }
};

class Small_Stepper_motor {
private:
    int pulse_pin;
    int dir_pin;
    gpiod::line pulse_line;
    gpiod::line dir_line;

    // Should be 1600. What is this?
    int steps_per_rev = 1610;

public:
    Small_Stepper_motor(int pulse_pin, int dir_pin, gpiod::chip chip) : pulse_pin(pulse_pin), dir_pin(dir_pin) {
        // Retrieve the line handles
        this->pulse_line = chip.get_line(pulse_pin);
        this->dir_line = chip.get_line(dir_pin);

        dir_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});
        pulse_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});
    }

    ~Small_Stepper_motor() {
            pulse_line.release();
            dir_line.release();
    }

    // Trying with boolean. But try the python module first
    void opperate(int revs, bool dir) {
        // Set the direction
        this->dir_line.set_value(dir);
        // Pulse the motor
        for (int i = 0; i < revs*(this->steps_per_rev); i++) {
            this->pulse_line.set_value(1);
            usleep(1); // 100 us
            this->pulse_line.set_value(0);
            usleep(1); // 100 us
        }
    }
};
#include <gpiod.hpp>
#include <iostream>
#include <unistd.h>
#include <string>

class Big_Stepper_motor {
private:
    int m_pulse_pin;
    int m_dir_pin;

    std::string name;
    int row;

    gpiod::line pulse_line;
    gpiod::line dir_line;

    // Should be 1600. What is this?
    int steps_per_rev = 1650;
    float sleep_time = 0.000040; // 40 us
    int steps_taken = 0;

public:
    Big_Stepper_motor(int pulse_pin, int dir_pin, gpiod::chip chip, std::string name, int row)
        : m_pulse_pin(pulse_pin), m_dir_pin(dir_pin), name(name), row(row) {
        // Retrieve the line handles

        this->pulse_line = chip.get_line(pulse_pin);
        this->dir_line = chip.get_line(dir_pin);

        // This the string needed to request the line?
        dir_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});
        pulse_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});
    }

    ~Big_Stepper_motor() {
            pulse_line.release();
            dir_line.release();
    }

    // Trying with boolean. But try the python module first
    // This function makes the motor go in a certain direction in certain direction
    void opperate(int revs, bool dir) {
        steps_taken = steps_taken + revs*(steps_per_rev) * (dir ? -1 : 1);
        // Set the direction
        this->dir_line.set_value(dir);
        // Pulse the motor
        for (int i = 0; i < revs*(steps_per_rev); i++) {
            this->pulse_line.set_value(1);
            usleep(sleep_time); // 100 us
            this->pulse_line.set_value(0);
            usleep(sleep_time); // 100 us
        }
    }


    void steps_opperate(int steps, bool dir) {
        // Since forwards is 0 and backwards 1, we need to add or subtract the steps, depending on the direction
        // Should instead use -1**dir, but since it uses a function, i found this more convinient
        steps_taken = steps_taken + steps * (dir ? -1 : 1);
        // Set the direction
        this->dir_line.set_value(dir);
        // Pulse the motor
        for (int i = 0; i < steps; i++) {
            this->pulse_line.set_value(1);
            usleep(sleep_time); // 100 us
            this->pulse_line.set_value(0);
            usleep(sleep_time); // 100 us
        }
    }


    std::string get_name() {
        return this->name;
    }

    int get_row() {
        return this->row;
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

        // This the string needed to request the line?
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
// From https://github.com/brgl/libgpiod

#include <gpiod.hpp>
#include <iostream>
#include <unistd.h>

int main() {

    // Attempt to open gpiochip0
    gpiod::chip chip("gpiochip0");

    gpiod::line pulse_line = chip.get_line(23);  // GPIO20 pulse pin
    gpiod::line dir_line = chip.get_line(24);  // GPIO21 dir pin

    pulse_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});  
    dir_line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});

    pulse_line.set_value(1);

    int sleep_time = 850;

    // usleep(5000000); // 1 sec
    for(int i = 0; i < 4; i++) {
        dir_line.set_value(i%2);
        for (int j = 0; j < 290; j++) {
        pulse_line.set_value(1);
        usleep(sleep_time); // 1 ms
        pulse_line.set_value(0);
        usleep(sleep_time); // 1 ms
        }
    }

    pulse_line.set_value(1);
    usleep(sleep_time); // 1 ms
    pulse_line.set_value(0);
    usleep(sleep_time); // 1 ms

    pulse_line.set_value(0);
    dir_line.set_value(0);
    
    pulse_line.release();
    dir_line.release();
    std::cout << "End of file" << std::endl;
    return 0;
}

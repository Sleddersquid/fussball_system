#include <gpiod.hpp>
#include <iostream>
#include <unistd.h>

int main() {

    // Attempt to open gpiochip0
    gpiod::chip chip("gpiochip0");

    gpiod::line line1 = chip.get_line(20);  // GPIO20 pulse pin
    gpiod::line line2 = chip.get_line(21);  // GPIO21 dir pin

    line1.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});  
    line2.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});

    line2.set_value(0);


    for (size_t i = 0; i < 1600; i++)
    {
        line1.set_value(0);
        std::cout << "Pulse set" << std::endl;
        usleep(1); // 1us
        line1.set_value(1);
        std::cout << "Pulse reset" << std::endl;
        usleep(1); // 1us
    }

    
    line1.release();
    line2.release();
    std::cout << "End of file" << std::endl;
    return 0;
}

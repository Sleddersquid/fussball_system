#include <gpiod.hpp>
#include <iostream>
#include <unistd.h>

int main() {

    // Attempt to open gpiochip0
    gpiod::chip chip("gpiochip0");

    gpiod::line line1 = chip.get_line(23);  // GPIO20 pulse pin
    gpiod::line line2 = chip.get_line(24);  // GPIO21 dir pin

    line1.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});  
    line2.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0});

    line1.set_value(1);
    line2.set_value(1);

    // usleep(5000000); // 1 sec

    line1.set_value(0);
    line2.set_value(0);
    
    line1.release();
    line2.release();
    std::cout << "End of file" << std::endl;
    return 0;
}

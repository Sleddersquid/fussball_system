#include <iostream>
#include <gpiod.h>


int main() {

    std::cout << "Hello, World!" << std::endl;
    gpiod_chip_close(chip);
    return 0;
}

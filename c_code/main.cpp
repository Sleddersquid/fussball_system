#include <iostream>
#include <array>
#include <vector>

#include <gpiod.hpp>
#include <opencv2/opencv.hpp>

#include "library/motors.hpp"
#include "library/deque_extra.hpp"

int main()
{

    gpiod::chip chip("gpiochip0");

    std::cout << "End of file" << std::endl;
    return 0;
}

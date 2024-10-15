#include <math.h>
#include <iostream>

int main() {

    int y1 = 300;
    int x1 = 300;

    int y2 = 350;
    int x2 = 350;


    // From formula tan(theta) = (y2 - y1) / (x2 - x1)
    int theta = atan2(y2 - y1, x2 - x1) * 180 / 3.14159265;

    std::cout << "Theta: " << theta << std::endl;

    return 0; 

}
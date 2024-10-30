#include <iostream>
#include <chrono>


auto thread_start_time = std::chrono::high_resolution_clock::now();

int main() {

    std::cout << typeid(thread_start_time).name() << std::endl;


    std::cout << "Hello World" << std::endl;
    return 0;
}

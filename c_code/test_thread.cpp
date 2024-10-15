// From https://stackoverflow.com/questions/25524775/delayed-start-of-a-thread-in-c-11

#include <iostream>
#include <thread>

void thread_func(const int i) {
    std::cout << "hello from thread: " << i << std::endl;
}

int main() {
    std::thread t;
    std::cout << "t exists" << std::endl;

    t = std::thread{ thread_func, 7 };
    t.join();

    std::cout << "done!" << std::endl;
}
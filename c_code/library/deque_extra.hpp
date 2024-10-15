#include <deque>
#include <string>


/**
 * @brief A deque that has a max size. If the deque is full, the first element is removed
 * @param max_size Sets the max size of the deque. 
 */
template <typename T>
class max_deque {
private:
    std::deque<T> deque;
    int max_size;
public:
    max_deque(int max_size) : max_size(max_size) {
        std::cout << "created a deque with max length of" << std::to_string(max_size) << std::endl;
    }

    void push_back(T val) {
        // Double check if > or >=
        if (deque.size() >= max_size) {
            deque.pop_front();
        }
        deque.push_back(val);
    }

    void push_front(T val) {
        // Double check if > or >=
        if (deque.size() >= max_size) {
            deque.pop_back();
        }
        deque.push_front(val);
    }

    T pop_front() {
        T val = deque.front();
        deque.pop_front();
        return val;
    }

    T pop_back() {
        T val = deque.back();
        deque.pop_back();
        return val;
    }

    int get_size() {
        return deque.size();
    }
};
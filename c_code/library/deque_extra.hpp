#include <deque>
#include <string>


/**
 * @brief A deque that has a max size. If the deque is full, the first element is removed
 * @param T The type of the deque
 * @param m_max_size Sets the max size of the deque. 
 */
template <typename T, int max_size>
class max_deque {
private:
    std::deque<T> deque;
    int m_max_size = max_size;
public:
    max_deque() {
        std::cout << "created a deque with max length of" << std::to_string(m_max_size) << std::endl;
    }

    void push_front(T val) {
        // Double check if > or >=
        if (deque.size() >= m_max_size) {
            deque.pop_back();
        }
        deque.push_front(val);
    }

    T pop_front() {
        T val = deque.front();
        deque.pop_front();
        return val;
    }

    T front() {
        return deque.front();
    }

    void push_back(T val) {
        // Double check if > or >=
        if (deque.size() >= m_max_size) {
            deque.pop_front();
        }
        deque.push_back(val);
    }

    T pop_back() {
        T val = deque.back();
        deque.pop_back();
        return val;
    }

    T back() {
        return deque.back();
    }

    int get_size() {
        return deque.size();
    }
};
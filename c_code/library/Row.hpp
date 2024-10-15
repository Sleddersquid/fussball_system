#include "motors.hpp"
#include <array>



// This class uses template to set the size of the array. It also set the number of players in the row
/**
 * @brief This class is used to controll both motors, the small and big one, and does the decision making for the row
 * @param array_size, m_number_of_players  - The number of players in the row
 * @param big_motor - The big motor in the row
 * @param small_motor - The small motor in the row
 * @param number_of_players - The number of players in the row
 */
template <int array_size>
class Row {
private:
    Big_Stepper_motor m_big_motor;
    Small_Stepper_motor m_small_motor;
    int m_number_of_players = array_size;
    std::array<std::pair<int, int>, array_size> m_player_positions;

public:
    Row(Big_Stepper_motor big_motor, Small_Stepper_motor small_motor)  {
        this->m_big_motor = big_motor;
        this->m_small_motor = small_motor;

        // Intialize the player positions
        for (int i = 0; i < array_size; i++) {
            m_player_positions[i] = std::make_pair(0, 0);
        }
    }

    void f(int radius, int theta) {
        // f(r, theta)
        // if r nagative, then go backwards
        // if theta negative, then go left
    }
};
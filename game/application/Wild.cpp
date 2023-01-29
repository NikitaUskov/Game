#include "Wild.h"

namespace RPG {

Wild::Wild(std::string name, coordinate start_pos):
                                                    Unit(std::move(name), WILD, start_pos),
                                                    range(1)
                                                    {}

int Wild::get_num_damage() const noexcept{
    return damage;
}

int Wild::get_range() const noexcept{
    return range;
}

int Wild::get_accuracy() const noexcept {
    return accuracy;
}

int Wild::attack() {
    if (params.current_time <= 0) {
        return -3; // haven't time
    }
    params.current_time -= time_to_attack;
    if (GetRandomNumber(0, 100) > accuracy) {
        return 1; // missing
    }
    return 0;
}

} // RPG
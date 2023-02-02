# include "Ammo_container.h"

namespace RPG {

std::unordered_map<AMMO_WEIGHT, std::string> Ammo_container::TYPE_NAME_ammo = {{machine_gun, "Machine gun"},
                                                               {shotgun, "Shotgun"},
                                                               {pistol, "Pistol"},
                                                               {empty, "empty"}};


Ammo_container::Ammo_container(AMMO_WEIGHT type0) : // initialisation params
                                                    Item(TYPE_NAME_ammo[type0], 0, AMMO_CONTAINER),
                                                    type(type0),
                                                    current_count(0)
                                                    {}

bool Ammo_container::take_ammo(int number) { // take some ammo from container
    if (number > current_count) {
        return false;
    }
    current_count -= number;
    weight -= number * ((int) type);

    return true;
}
bool Ammo_container::put_ammo(int number) { // put some ammo to container

    if (current_count + number > max_count) {
//        current_count += number;
//        weight += number * ((int) type);
//        return false;
        number = max_count - current_count;
        weight += number * ((int) type);
        return false;
    }
    current_count += number;
    weight += number * ((int) type);
    return true;
}

int Ammo_container::num_ammo() const {
    return current_count;
}

int Ammo_container::max_ammo() const {
    return max_count;
}

AMMO_WEIGHT Ammo_container::get_type() {
    return type;
}

} // RPG
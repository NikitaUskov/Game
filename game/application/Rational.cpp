#include "Rational.h"

namespace RPG {

Rational::Rational(std::string name, coordinate pos):
                                                    Unit(std::move(name), RATIONAL, pos),
                                                    Wild(std::move(name), pos)
                                                    {
    damage = 0;
    range = 0;
    time_to_attack = 0;
                                                    }

Rational::Rational(std::string name, coordinate pos, Weapon* currentWeapon):
                                                    Unit(std::move(name), RATIONAL, pos)
                                                    , Wild(std::move(name), pos)
                                                    , current_weapon(currentWeapon)
                                                    {
    damage = current_weapon->get_damage();
    range = current_weapon->get_params().bas_params.range;
    time_to_attack = current_weapon->time_to_shot();
                                                    }

int Rational::attack() {
    if (params.current_time <= 0) {
        return -3; // haven't time
    }
    if (current_weapon == nullptr) {
        return -1; // haven't weapon
    }
    if (current_weapon->shot() < 0) {
        return -2; // hasn't ammo's
    }

    params.current_time -= 1;
    if (GetRandomNumber(0, 100) > accuracy) {
        return 1; // missing
    }
    return 0;
}

int Rational::take_weapon(Map& map_) {
    std::vector<Weapon*> weapons = {new Weapon(RPK_74), new Weapon(AK_74)
                                    , new Weapon(PM), new Weapon(TO3_34)};

    Item* probably_weapon = nullptr;
    for (auto& weapon : weapons) {
        probably_weapon = map_[position]->take_item(dynamic_cast<Item*>(weapon));
        if (probably_weapon != nullptr) break;
    }

    if (probably_weapon == nullptr) {
        for (auto currWeapon: weapons) {
            delete currWeapon;
        }
        return -1; // bad take
    }
    if (current_weapon != nullptr) {
        (*this).put_current_weapon(map_);
    }
    current_weapon = dynamic_cast<Weapon*>(probably_weapon);
    damage = current_weapon->get_damage();
    range = current_weapon->get_params().bas_params.range;
    time_to_attack = current_weapon->time_to_shot();
    for (auto currWeapon: weapons) {
        delete currWeapon;
    }
    return 0;
}

int Rational::put_current_weapon(RPG::Map &map_){
    if (map_[position]->put_item(current_weapon)) {
        return -1; // bad put ?wtf?
    }
    current_weapon = nullptr;
    return 0;
}

Weapon* Rational::get_current_weapon() {
    return current_weapon;
}

Rational::~Rational() {
    delete current_weapon;
}

} // RPG
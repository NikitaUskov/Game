#include "Weapon.h"

namespace RPG {

std::unordered_map<WEAPON_TYPE, WEAPON_PARAMS_WEIGHT> Weapon::weapon_type_params = {{AK_74,  {{15, 4, 2, machine_gun, 10}, 4000}},
                                                                                         {RPK_74, {{20, 4, 3, machine_gun, 15}, 5000}},
                                                                                         {PM,     {{10, 2, 1, pistol,      8}, 800}},
                                                                                         {TO3_34, {{30, 1, 2, shotgun,     2}, 3000}},
                                                                                         {Empty, {{0, 0, 0, empty, 0}, 0}}};

std::unordered_map<WEAPON_TYPE, std::string> Weapon::TYPE_NAME_weapon = {{AK_74, "AK_74"},
                                                                              {RPK_74, "RPK_74"},
                                                                              {PM, "PM"},
                                                                              {TO3_34, "TO3_34"},
                                                                              {Empty, "Empty"}};

Weapon::Weapon():
                Item(TYPE_NAME_weapon[Empty], 0, WEAPON),
                params(weapon_type_params[Empty]),
                ammo_number(0),
                type(Empty) {}


Weapon::Weapon(WEAPON_TYPE type0, int ammo_number):
                                                    Item
                                                        (TYPE_NAME_weapon[type0],
                                                         weapon_type_params[type0].weight,
                                                         WEAPON
                                                         ),
                                                    params(weapon_type_params[type0]),
                                                    ammo_number(ammo_number),
                                                    type(type0)
                                                    {}

WEAPON_TYPE Weapon::get_type() {
    return type;
}

int Weapon::shot() {
    // questions for shot...
    if (!shot_result()) {
        return -1;
    }
    ammo_number -= 1;

    return params.bas_params.range;
}

// how to get result...
bool Weapon::shot_result() const {
    if (ammo_number == 0) {
        return false;
    }
    return true;
}


int Weapon::get_damage() const {
    return params.bas_params.damage;
}

int Weapon::time_to_shot() noexcept {
    return shot_time;
}

int Weapon::reload(Ammo_container* container) {
    // take ammo's from container
    //if (don't have ammo) { return -1;}
    if (type == Empty) {
        return -2;
    }
    int get_ammo = params.bas_params.max_ammo - ammo_number;
    int curr_ammo = container->num_ammo();
    if (curr_ammo < get_ammo && curr_ammo > 0) {
        get_ammo = curr_ammo;
    }
    else if (curr_ammo <= 0) {
        return -1;
    }
    container->take_ammo(get_ammo);
    ammo_number += get_ammo;

    return params.bas_params.reload_time;
}

WEAPON_PARAMS_WEIGHT Weapon::get_params() const noexcept {
    return params;
}

int Weapon::get_ammo_num() const noexcept{
    return ammo_number;
}

} // RPG
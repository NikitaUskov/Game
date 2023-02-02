#include "Operative.h"

namespace RPG {

Operative::Operative(coordinate position, Weapon* basic_weapon,
                     std::string name) :
                                         Unit(std::move(name), OPERATIVE, position),
                                         Forager(std::move(name), position),
                                         Rational(std::move(name), position),
                                         time_to_reload(basic_weapon->get_params().bas_params.reload_time)
                                         {
    current_weapon = basic_weapon;
    damage = current_weapon->get_damage();
    range = current_weapon->get_params().bas_params.range;
    time_to_attack = current_weapon->time_to_shot();
}

Operative::Operative(coordinate position, Weapon* basic_weapon
                     , std::vector<Item*>& items, std::string name):
                                        Unit(std::move(name), OPERATIVE, position),
                                        Forager(std::move(name), position, items),
                                        Rational(std::move(name), position),
                                        time_to_reload(basic_weapon->get_params().bas_params.reload_time)
{
    current_weapon = basic_weapon;
    damage = current_weapon->get_damage();
    range = current_weapon->get_params().bas_params.range;
    time_to_attack = current_weapon->time_to_shot();
}

int Operative::choice_weapon(Weapon* new_weapon) {
    auto probably_weapon = inventory->change_item(current_weapon, new_weapon);
    if (probably_weapon == nullptr) {
        return -1;
    }
    current_weapon = dynamic_cast<Weapon*>(probably_weapon);
    damage = current_weapon->get_damage();
    range = current_weapon->get_params().bas_params.range;
    time_to_attack = current_weapon->time_to_shot();
    time_to_reload = current_weapon->get_params().bas_params.reload_time;
    return 0;
}

int Operative::heal(Medicine_Kit* use_kit) {
    Item* curr_item = inventory->get_item(dynamic_cast<Item*>(use_kit));
    if (curr_item == nullptr) {
        return -1; // don't have item
    }
    auto use_curr_kit = dynamic_cast<Medicine_Kit*>(curr_item);
    coordinate med_params = use_curr_kit->use();
    if (params.current_time < med_params.second) {
        inventory->put_item(curr_item);
        return -2; //don`t have time
    }
    params.current_health += med_params.first;
    if (params.current_health > params.max_health) {
        params.current_health = params.max_health;
    }
    params.current_time -= med_params.second;
//    if (params.current_time == 0) {
//        return -1; //spend all of time
//}
    delete use_curr_kit;
    return 0;
}

int Operative::reload() {
    if (current_weapon->get_params().weight == 0) {
        return -2; // don`t weapon
    }
    if (time_to_reload > params.current_time) {
        return -1;
    }

    auto container = Ammo_container(current_weapon->get_params().bas_params.ammo_type);
    auto cont = (Ammo_container *) (inventory->get_item(dynamic_cast<Item *>(&container)));

    if (cont == nullptr) {
        return -2; // not container
    }

    int res = current_weapon->reload(cont);
    inventory->put_item(dynamic_cast<Item *>(cont));

    if (res > 0) {
        params.current_time -= time_to_reload;
    }
    return res; // -1 - bad reload, time_reload - good
}

int Operative::attack() {
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


} // RPG
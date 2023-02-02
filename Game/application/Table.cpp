#include "Table.h"

#include <utility>

namespace RPG {

Table::Table(): weight(0), inventory() {}

Table::Table(std::vector<Item*> items): weight(0), inventory(std::move(items)) {
    (*this).update_params();
}

int Table::number_of_items() {
    return (int) inventory.size();
}

Item* Table::change_item(Item* put_item_in, Item* item_take_from) {
    auto curr_item = inventory.begin();
    Item* res;
    while(curr_item != inventory.end()) {
        ITEM_TYPE type_curr_item = (*curr_item)->get_type();
        if (type_curr_item == item_take_from->get_type()) {
            if (((type_curr_item == WEAPON) &&
                 (dynamic_cast<Weapon*>(*curr_item)->get_type() == dynamic_cast<Weapon*>(item_take_from)->get_type()))
                or
                ((type_curr_item == AMMO_CONTAINER) &&
                 (dynamic_cast<Ammo_container*>(*curr_item)->get_type() == dynamic_cast<Ammo_container*>(item_take_from)->get_type()))
                or
                ((type_curr_item == MEDICINE_KIT) &&
                 (dynamic_cast<Medicine_Kit*>(*curr_item)->get_type() == dynamic_cast<Medicine_Kit*>(item_take_from)->get_type())))
            {
                res = *curr_item;
                *curr_item = put_item_in;
                weight += -item_take_from->get_weight() + put_item_in->get_weight();
                //inventory.erase(curr_item);
                return res;
            }
        }
        ++curr_item;
    }
    return nullptr; // not have old
}

Item* Table::get_item(Item* item) {
    auto curr_item = inventory.begin();
    Item* res;
    while(curr_item != inventory.end()) {
        ITEM_TYPE type_curr_item = (*curr_item)->get_type();
        if (type_curr_item == item->get_type()) {
            if (((type_curr_item == WEAPON) &&
                (dynamic_cast<Weapon*>(*curr_item)->get_type() == dynamic_cast<Weapon*>(item)->get_type()))
                or
                ((type_curr_item == RPG::AMMO_CONTAINER) &&
                 (dynamic_cast<Ammo_container*>(*curr_item)->get_type() == dynamic_cast<Ammo_container*>(item)->get_type()))
                or
                ((type_curr_item == MEDICINE_KIT) &&
                 (dynamic_cast<Medicine_Kit*>(*curr_item)->get_type() == dynamic_cast<Medicine_Kit*>(item)->get_type())))
            {
                res = *curr_item;
                inventory.erase(curr_item);
                weight -= res->get_weight();
                return res;
            }
        }
        ++curr_item;
    }
    return nullptr; // not have old
}

int Table::put_item(Item* item) {
    weight += item->get_weight();
    (*this).inventory.push_back(item);
    return 0;
}

int Table::get_curr_weight() const noexcept {
    return weight;
}

int Table::change_weight(int loss) {
    if (loss > weight) {
        return -1; // wtf ?!
    }
    weight -= loss;
    return 0;
}

int Table::update_params() noexcept{
    auto curr_item = inventory.begin();
    weight = 0;
    while(curr_item != inventory.end()) {
        weight += (*curr_item)->get_weight();
        ++curr_item;
    }
    return 0;
}

std::vector<Item*>::iterator Table::get_iter() {
    return inventory.begin();
}

Table::~Table() {
    for (auto curr_item: inventory) {
        ITEM_TYPE type_curr_item = curr_item->get_type();
        if (type_curr_item == WEAPON) {
            delete dynamic_cast<Weapon*>(curr_item);
        }
        else if (type_curr_item == MEDICINE_KIT) {
            delete dynamic_cast<Medicine_Kit*>(curr_item);
        }
        else if (type_curr_item == AMMO_CONTAINER) {
            delete dynamic_cast<Ammo_container*>(curr_item);
        }
        else {
            delete curr_item;
        }
    }
}

} // RPG
#include "Forager.h"

#include <utility>

namespace RPG {

Forager::Forager(std::string name, RPG::coordinate pos):
                                                        Unit(std::move(name),
                                                              FORAGER,
                                                              pos),
                                                        inventory(new Table())
                                                        {
    //inventory = new Table();
                                                        }

Forager::Forager(std::string name, coordinate pos, std::vector<Item*>& items): Unit(std::move(name),
                                                                                   FORAGER,
                                                                                   pos),
                                                                              inventory(new Table(std::move(items)))
{
    //inventory = new Table();
}

int Forager::get_strength() const noexcept{
    return strength;
}

int Forager::get_weight() const noexcept{
    return inventory->get_curr_weight();
}

int Forager::get_num_of_items() const noexcept{
    return inventory->number_of_items();
}

int Forager::take_item_to_inventory(Item *item, Map& map_) {
    Item* took_item = map_[position]->take_item(item);
    if (took_item == nullptr) {
        return -2; // not object
    }

    if (strength - inventory->get_curr_weight() < took_item->get_weight()) {
        map_[position]->put_item(took_item);
        return -1; // tired
    }

    inventory->put_item(took_item);
    return 0;
}

int Forager::take_item_to_inventory(Map& map_) {
    Item* took_item = map_[position]->take_item();
    if (took_item == nullptr) {
        return -2; // not object
    }

    if (strength - inventory->get_curr_weight() < took_item->get_weight()) {
        map_[position]->put_item(took_item);
        return -1; // tired
    }

    inventory->put_item(took_item);
    return 0;
}

int Forager::put_item_from_inventory(Item *item, Map& map_) {
    Item* put_item = inventory->get_item(item);
    if (put_item == nullptr) {
        return -1; // not object
    }
    map_[position]->put_item(put_item);
    return 0;
}

int Forager::put_all_items_from_inventory(Map& map_) {
    int itemNums = inventory->number_of_items();
    for (int i = 0; i < itemNums; ++i) {
        Item *put_item = inventory->get_item(*inventory->get_iter());
        if (put_item == nullptr) {
            return -1; // not object
        }
        map_[position]->put_item(put_item);
    }
    return 0;
}

Forager::~Forager() {
    delete inventory;
}

Table* Forager::see_inventory() const noexcept {
    return inventory;
}

} // RPG
#include "Item.h"

namespace RPG {

Item::Item(): name("empty"), weight(0), type(EMPTY) {}

Item::Item(std::string name, int weight, ITEM_TYPE type0): name(std::move(name)), weight(weight), type(type0) {}

ITEM_TYPE Item::get_type() const noexcept{
    return type;
}

int Item::get_weight() const noexcept {
    return weight;
}

}
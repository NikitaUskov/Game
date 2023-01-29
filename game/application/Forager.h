#ifndef FORAGER_H
#define FORAGER_H

//#include "Unit.h"
//#include "Table.h"
#include "Rational.h"

namespace RPG {

/**
 * @class Forager hear of Wild - can take items in inventory, but can't attack
 */
class Forager : virtual public Unit {
public:
    /**
     * @brief constructor of Forager
     * @param name (string) - name of forager
     * @param pos (coordinate) - start position of forager
     */
    Forager(std::string name, coordinate pos); // initialisation inventory
    Forager(std::string name, coordinate pos, std::vector<Item*>& items); // initialisation inventory

    /**
     * @brief check strength of forager
     * @return (int) - strength of forager
     */
    [[nodiscard]] int get_strength() const noexcept;

    /**
     * @brief check current number of items in inventory
     * @return (int) - number of items
     */
    [[nodiscard]] int get_num_of_items() const noexcept;

    /**
     * @brief check current weight of inventory
     * @return (int) - current weight
     */
    [[nodiscard]] int get_weight() const noexcept;

    /**
     * @brief check inventory
     * @return inventory
     */
    [[nodiscard]] Table* see_inventory() const noexcept;

    /**
     * @brief take item from floor to inventory
     * @param item (Item *) - ptr of similar item
     * @param map_ (Map&) - map on which happen action
     * @return (int) - -2 - not object, \n
     * -1 - tired (weight > strength), \n
     * 0 - successfully take
     */
    int take_item_to_inventory(Item* item, Map& map_);

    /**
     * @brief take first item from cell to inventory
     * @param map_ map where happening actions
     * @return flag - result of action
     */
    int take_item_to_inventory(Map& map_);

    /**
     * @brief put item to floor from inventory
     * @param item (Item *) - ptr of item
     * @param map_ (Map&) - map on which happen action
     * @return (int) - -1 - not object, \n
     * 0 - successfully put
     */
    int put_item_from_inventory(Item* item, Map& map_);

    /**
     * @brief put all items from inventory to cell
     * @param map_ map where happening actions
     * @return flag - result of action
     */
    int put_all_items_from_inventory(Map& map_);

    /**
     * @brief destructor - delete ptr of inventory
     */
    ~Forager();
protected:
    /**
     * @brief (const int) - strength - maximum of weight = 100 kg
     */
    const int strength = 10000;
    /**
     * (Table *) - inventory with all items
     */
    Table* inventory{};
};

} // RPG

#endif //FORAGER_H

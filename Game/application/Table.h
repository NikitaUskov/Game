#ifndef TABLE_H
#define TABLE_H


#include "Weapon.h"
//#include "Ammo_container.h"
//#include "Medicine_Kit.h"

namespace RPG {

/**
 * @class Table - inventory of unit
 */
class Table {
public:
    /**
     * @brief default constructor of Table
     */
    Table();
    Table(std::vector<Item*> items);
    /**
     * @brief return number of current things
     * @return (int) - number of items
     */
    int number_of_items();
    /**
     * @brief change item in table
     * @param put_item_in (Item *) - ptr with similar item which we put in table
     * @param item_take_from (Item *) - ptr with similar item which we tae from table
     * @return (Item *) - ptr of item which we take or nullptr
     */
    Item* change_item(Item* put_item_in, Item* item_take_from);
    /**
     * @brief take item from inventory
     * @param item (Item *) - ptr with similar item which we want to take
     * @return (Item *) - ptr of need item or nullptr
     */
    Item* get_item(Item* item);
    /**
     * @brief put item to inventory
     * @param item (Item *) - ptr with similar item
     * @return 0 - we always can put item
     */
    int put_item(Item* item);
    /**
     * @brief check current weight of table
     * @return (int) - current weight
     */
    [[nodiscard]] int get_curr_weight() const noexcept;
    /**
     * @brief change weight of table
     * @param loss (int) - number of weight which we loss
     * @return (int) - -1 - we can't change (?wtf - unreal?)
     * 0 - successfully change
     */
    int change_weight(int loss);
    /**
     * @brief recount current weight of table
     * @return (int) - 0 - successfully update
     */
    int update_params() noexcept;
    /**
     * @brief get iterator of inventory
     * @return iterator of inventory
     */
    std::vector<Item*>::iterator get_iter();
    /**
     * @brief destructor - delete ptr of all items
     */
    ~Table();
protected:
    /**
     * @brief (int) - current weight
     */
    int weight;
    /**
     * @brief (vector<Item*>) - inventory with all items
     */
    std::vector<Item *> inventory;
};

} // RPG

#endif //TABLE_H
#ifndef ITEM_H
#define ITEM_H

#include "RPG_Object.h"

namespace RPG {

/**@brief This is helping class
 * @enum MEDICINE_KIT = 0
 * @enum AMMO_CONTAINER = 1
 * @enum WEAPON = 2
 * @enum EMPTY = 3
 */
enum ITEM_TYPE {
    MEDICINE_KIT = 0,
    AMMO_CONTAINER,
    WEAPON,
    EMPTY
};
/**
 * @class Item abstract class for Weapon, Med_kit and ammo_cont
 */
class Item {
public:
    Item();
    /** @brief constructor of class Item
    * @param name string name of item
    * @param weight weight of item
    * @param type0 type of item - enum class
    */
    Item(std::string name, int weight, ITEM_TYPE type0);
    /**
     * @brief check type of item
     * @return enum ITEM_TYPE - type of item
     */
    [[nodiscard]] virtual ITEM_TYPE get_type() const noexcept;
    /**
     * @brief check weight of item
     * \return (int) - weight of item
     */
    [[nodiscard]] int get_weight() const noexcept;
protected:
    /**
     * @brief (ITEM_TYPE) - type of item
     */
    const ITEM_TYPE type;
    /**
     * @brief (string) - name of item
     */
    const std::string name;
    /**
     * @brief (int) - weight of item
     */
    int weight;
};

} //RPG

#endif //ITEM_H

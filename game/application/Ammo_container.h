#ifndef AMMO_CONTAINER_H
#define AMMO_CONTAINER_H

#include "Item.h"

namespace RPG {

/**
 * @brief type of ammo's and their correspondence to the weight in g
 * @enum
 * machine_gun = 6, \n
 * pistol = 5, \n
 * shotgun = 7, \n
 * empty = 0 \n
 */
enum AMMO_WEIGHT {
    machine_gun = 6,
    pistol = 5,
    shotgun = 7,
    empty = 0
};

/**
 * @class Ammo_container hear of Item
 */
class Ammo_container : public Item {
public:
    /**
     * @brief constructor of class Ammo_container
     * @param type - type of ammo
     */
    explicit Ammo_container(AMMO_WEIGHT type); // initialisation params
    /**
     * @brief take some ammo's from container
     * @param number - number of takes ammo's
     * @return true or false - can you take needed ammo's
     */
    bool take_ammo(int number); // take some ammo from container
    /**
     * @brief put some ammo's to container
     * @param number - number of puts ammo's
     * @return true or false - can you put needed ammo's
     * (anyway you put maximum ammo's how you can)
     */
    bool put_ammo(int number); // put some ammo to container
    /**
     * @brief number of ammo's in container
     * @return int - number of ammo's in container
     */
    [[nodiscard]] int num_ammo() const; // show how much ammo you have

    /**
     * @brief number of maximum ammo's in container
     * @return int - number of maximum ammo's in container
     */
    [[nodiscard]] int max_ammo() const;
    /**
     * @brief check type of ammo's
     * @return AMMO_WEIGHT - type of ammo's
     */
    AMMO_WEIGHT get_type();

    /**
     * @brief map with type weapon and their name
     */
    static std::unordered_map<AMMO_WEIGHT, std::string> TYPE_NAME_ammo;

protected:
    /**
     * @brief type of ammo's in container
     */
    AMMO_WEIGHT type;
    /**
     * @brief max count of ammo's in container
     */
    int max_count = 100;
    /**
     * @brief current count of ammo's in container (\<= max_count)
     */
    int current_count;
};

} // RPG
#endif //AMMO_CONTAINER_H

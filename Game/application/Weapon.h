#ifndef WEAPON_H
#define WEAPON_H

//#include "Ammo_container.h"
//#include "Table.h"
#include "Medicine_Kit.h"

namespace RPG {

/**
 * @brief struct of basic params of weapon: \n
 * (int) damage of shot, \n
 * (int) range of shot, \n
 * (int) time needs for reload, \n
 * (AMMO_WEIGHT) type of uses ammo, \n
 * (int) number of maximum ammo's \n
 */
struct WEAPON_PARAMS {
    int damage;
    int range;
    int reload_time;
    AMMO_WEIGHT ammo_type;
    int max_ammo;
};

/**
 * @brief struct of basic params + (int) weight
 */
struct WEAPON_PARAMS_WEIGHT {
    WEAPON_PARAMS bas_params;
    int weight;
};

/**
 * @brief type of weapon
 * @enum
 * AK_74\n
 * RPK_74\n
 * PM\n
 * TO3_34\n
 * Empty
 */
enum WEAPON_TYPE {
    AK_74 = 0,
    RPK_74,
    PM,
    TO3_34,
    Empty
};



/**
 * @class Weapon hear of Item
 */
class Weapon : public Item {
public:
    /**
     * @brief default constructor
     */
    Weapon();
    /**
     * @brief constructor with 1-2 params
     * @param type - (WEAPON_TYPE) type of weapon
     * @param ammo_number - (int) number of ammo's
     */
    explicit Weapon(WEAPON_TYPE type, int ammo_number = 0); // initialisation params
    /**
     * @brief check type of weapon
     * @return (WEAPON_TYPE) type of weapon
     */
    WEAPON_TYPE get_type();
    /**
     * @brief make attack with gun - loss one of ammo's
     * \return range of attack - successfull shot\n
     * -1 - can't shot (haven't ammo's)
     */
    int shot();
    /**
     * @brief check can you do shot
     * @return true or false - have ammo
     */
    [[nodiscard]] bool shot_result() const;
    /**
     * @brief get damage, which gun can make
     * @return (int) damage number
     */
    [[nodiscard]] int get_damage() const;
    /**
     * @brief reload of gun. Take ammo's from container and put they in weapon
     * @param container - pointer to the container from which ammo is taken
     * @return (int) reload_time - successfully reload, \n
     * -2 - have empty weapon,\n
     * -1 - don't have ammo's
     */
    int reload(Ammo_container* container); // update ammo in gun, return time_to_reload
    /**
     * @brief check current parameters of weapon
     * @return struct with params
     */
    [[nodiscard]] WEAPON_PARAMS_WEIGHT get_params() const noexcept;
    /**
     * @brief check time needs to shot
     * @return (int) time of shot
     */
    [[nodiscard]] static int time_to_shot() noexcept;
    /**
     * @brief check number of current ammo's
     * @return (int) number of current ammo's
     */
    [[nodiscard]] int get_ammo_num() const noexcept;
    /**
     * @brief table with type of weapon and their correspondence to parameters
     */
    static std::unordered_map<WEAPON_TYPE, WEAPON_PARAMS_WEIGHT> weapon_type_params;
    /**
     * @brief table with type of weapon and their correspondence to name
     */
    static std::unordered_map<WEAPON_TYPE, std::string> TYPE_NAME_weapon;
protected:
    /**
     * @brief (WEAPON_TYPE) type of weapon
     */
    const WEAPON_TYPE type;
    /**
     * @brief (WEAPON_PARAMS_WEIGHT) struct params
     */
    const WEAPON_PARAMS_WEIGHT params;
    /**
     * @brief (int) time needs for shot
     */
    static const int shot_time = 1;
    /**
     * @brief (int) number of current ammo's
     */
    int ammo_number;
};

} // RPG

#endif
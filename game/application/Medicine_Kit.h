#ifndef MEDICINE_KIT_H
#define MEDICINE_KIT_H

//#include "Item.h"
#include "Ammo_container.h"

namespace RPG {
/**
 * @brief type of medicine kit and their correspondence to the health points\n
 * @enum
 * Basic = 20, \n
 * Army = 50, \n
 * Scientific = 80, \n
 */
enum MED_KIT_HEALTH {
    Basic = 20,
    Army = 50,
    Scientific = 80
};



/**
 * @class Medicine_kit hear of Item
 */
class Medicine_Kit : virtual public Item {
public:
    /**
     * @brief constructor of Medicine kit
     * @param type0 type of medicine kit
     */
    explicit Medicine_Kit(MED_KIT_HEALTH type0); // initialisation type and params
    /**
    * @brief  just return need params for use and after you must delete it
    * \return pair of int params: (score_health, time_to_use)
    */
    [[nodiscard]] std::pair<int, int> use() const noexcept;
    /**
     * @brief check type of medicine kit
     * @return MED_KIT_HEALTH - type of medicine kit
     */
    [[nodiscard]] MED_KIT_HEALTH get_type();

    /**
     * @brief map with correspond of type of MedKit and name
     */
    static std::unordered_map<MED_KIT_HEALTH, std::string> TYPE_NAME_med;
protected:
    /**
     * @brief type of medicine kit
     */
    MED_KIT_HEALTH type;
    /**
     * @brief number of health points
     */
    const int score_health;
    /**
     * @brief time needs for use medicine kit
     */
    const int time_to_use = 1;
};

} // RPG

#endif //MEDICINE_KIT_H

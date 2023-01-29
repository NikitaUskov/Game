#ifndef WILD_H
#define WILD_H

#include "Unit.h"

namespace RPG {

/**
 * @class Wild hear of Unit - can attack with close distant
 */
class Wild : virtual public Unit {
public:
    /**
     * @brief constructor of Wild
     * @param name (string) - name of wild
     * @param start_pos (coordinate) - start position of wild
     */
    Wild(std::string name, coordinate start_pos);
    /**
     * @brief check number of damage
     * @return (int) - number of damage
     */
    [[nodiscard]] int get_num_damage() const noexcept;
    /**
     * @brief check range of attack
     * @return (int) - range of attack
     */
    [[nodiscard]] int get_range() const noexcept;
    /**
     * @brief check accuracy of attack
     * @return (int) - accuracy of attack
     */
    [[nodiscard]] int get_accuracy() const noexcept;
    /**
     * @brief attack of unit
     * @return (int) - -3 - haven't time, \n
     * 1 - missing, \n
     * 0 - successfully attack
     */
    virtual int attack();
protected:
    /**
     * @brief number of damage
     */
    int damage = 15;
    /**
     * @brief range of attack
     */
    int range = 1;
    /**
     * @brief time needs for attack
     */
    int time_to_attack = 1;
    /**
     * @brief accuracy of attack in %
     */
    const int accuracy = 90;
};

} // RPG

#endif //WILD_H

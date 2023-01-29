#ifndef RATIONAL_H
#define RATIONAL_H

#include "Wild.h"

namespace RPG {

/**
 * @class Rational hear of Wild - can shot with weapon
 */
class Rational : public Wild {
public:
    /**
     * @brief constructor of rational
     * @param name (string) - name of rational
     * @param pos (coordinate) - start position
     */
    explicit Rational(std::string name, coordinate pos);
    explicit Rational(std::string name, coordinate pos, Weapon* currentWeapon);
    /**
     * @brief check current weapon
     * @return (Weapon *) - ptr to current weapon
     */
    Weapon* get_current_weapon();
    /**
     * @brief take item from Floor
     * @param item (Item *) - ptr with similar item
     * @param map_ (Map&) - map on which you do action
     * @return (int) - -2 - not weapon, \n
     * -1 - not on floor, \n
     * 0 - successfully take
     */
    int take_weapon(Map& map_);
    /**
     * @brief put item on Floor
     * @param item (Item *) - ptr with similar item
     * @param map_ (Map&) - map on which you do action
     * @return - -1 - don't put (?wtf? don't real), \n
     * 0 - successfully put
     */
    int put_current_weapon(Map& map_);
    /**
     * @brief method attack: loss time and check accuracy
     * @return int - result of attack: \n
     * 0 - all of good \n
     * 1 - you are missing
     * -1 - you don't have weapon
     */
    int attack() override;
    /**
     * @brief destructor - delete ptr on current weapon
     */
    ~Rational();

protected:
    /**
     * @brief (Weapon *) - ptr to current weapon
     */
    Weapon* current_weapon{};
};

} // RPG

#endif //RATIONAL_H

#ifndef OPERATIVE_H
#define OPERATIVE_H

//#include "Rational.h"
#include "Forager.h"
//#include "Medicine_Kit.h"

namespace RPG {

/**
 * @class Operative hear of Rational and Forager - can all actions of Rational and Forager
 */
class Operative : public Rational, public Forager {
public:
    /**
     * @brief constructor of Operative
     * @param position (coordinate) - start position
     * @param basic_weapon (Weapon*) - start weapon
     * @param name (string) - name of operative
     */
    explicit Operative(coordinate position, Weapon* basic_weapon, std::string name);
    explicit Operative(coordinate position, Weapon* basic_weapon, std::vector<Item*>& items, std::string name);
    /**
     * @brief change current weapon to weapon from inventory
     * @param new_weapon (Weapon*) ptr to similar weapon
     * @return (int) - -1 - hasn't this weapon, \n
     * 0 - successfully changed
     */
    int choice_weapon(Weapon* new_weapon);
    /**
     * @brief heal with med_kit
     * @param use_kit (Medicine_kit) - ptr to similar med_kit
     * @return (int) - -1 - doesn't have item, \n
     * -2 - hasn't time, \n
     * 0 - successfully heal
     */
    int heal(Medicine_Kit* use_kit);
    /**
     * @brief reload current weapon
     * @return (int) - -2 - doesn't have need items, \n
     * -1 - doesn't have ammo's in container, \n
     * > 0 - successfully reload = time_reload
     */
    int reload();
    /**
     * @brief attack with current weapon
     * @return (int) - -3 - doesn't have time, \n
     * -2 - doesn't have ammo's, \n
     * -1 - doesn't have weapon, \n
     * 1 - missing, \n
     * 0 - successfully attack
     */
    int attack() override;
protected:
    /**
     * @brief (int) - time needed for reload
     */
    int time_to_reload;
};

} // RPG

#endif //OPERATIVE_H

#ifndef UNIT_H
#define UNIT_H

//#include "Weapon.h"
//#include "help_func.h"
#include "Cell.h"

namespace RPG {

/**
 * @brief types of units
 * @enum WILD, \n
 * RATIONAL, \n
 * FORAGER, \n
 * OPERATIVE
 */
enum UNIT_TYPE {
    WILD = 0,
    RATIONAL,
    FORAGER,
    OPERATIVE
};

/**
 * @brief struct with basic params of units
 */
struct basic_unit_parameters {
    int max_health = 100;
    int current_health = 100;
    int time_to_step = 1;
    int max_time = 10;
    int current_time = 10;
    int view_radius = 4;
};

/**
 * @class Unit hear of RPG_Object
 */
class Unit: public RPG_Object {
public:
    /**
     * @brief constructor of Unit
     * @param name (string) - name of unit
     * @param type (UNIT_TYPE) - type of unit
     * @param position (coordinate) - start position of unit
     */
    explicit Unit(std::string name, UNIT_TYPE type, coordinate position); // initialisation all params of Units
    /**
     * @brief check type of unit
     * @return (UNIT_TYPE) - type of unit
     */
    UNIT_TYPE get_type();
    /**
     * @brief check params of unit
     * @return (basic_unit_parameters) - parameters of unit
     */
    [[nodiscard]] basic_unit_parameters get_params() const noexcept;
    /**
     * @brief check position of unit
     * @return (coordinate) - current position of unit
     */
    [[nodiscard]] coordinate get_position() const noexcept;
    /**
     * @brief unit get damage after attack
     * @param loss (int) - number of hp which unit loss
     * @return (int) 100 - died, \n
     * 1 - get damage and leave
     */
    int get_damage(int loss) override;
    /**
     * @brief change position of unit
     * @param new_position (coordinate) - new position of unit
     * @param map_ (Map&) - map on which unit step
     * @return (int) - 1 - unit haven't time, \n
     * -1 - can't step, \n
     * 0 - successfully step
     */
    int step(Direction new_position, Map& map_);
    /**
     * @brief update time (current time = 10)
     */
    void setParams(int health, int time);
    void update_time();
protected:
    /**
     * @brief (UNIT_TYPE) - type of unit
     */
    UNIT_TYPE type;
    /**
     * @brief (string) - name of unit
     */
    std::string name;
    /**
     * @brief parameters of unit
     */
    basic_unit_parameters params;
    /**
     * @brief (coordinate) - current position of unit
     */
    coordinate position;
};

} // RPG
#endif //UNIT_H

#ifndef RPG_OBJECT_H
#define RPG_OBJECT_H

#include "help_func.h"
#include <unordered_map>

namespace RPG {

/**
 * @brief side of motion
 * @enum Left, \n
 * Down, \n
 * Up, \n
 * Right
 */
enum Direction {
    Left = 0,
    Down,
    Up,
    Right
};

/**
 * @brief Type of Object
 * @enum ITEM, \n
 * UNIT
 */
enum TYPE_OBJECT {
    CELL = 0,
    UNIT
};

/**
 * @class RPG_Object - virtual class of Unit and Cell
 */
class RPG_Object {
public:
    /**
     * @brief virtual method describe process getting of damage
     * @return (int) - result of shot (kill or not or can't shot)
     */
    virtual int get_damage(int) = 0;
    /**
     * @brief side of motion and it's correspondence to coordinate
     */
    static std::unordered_map<Direction, coordinate> DIR_POS;
    /**
     * @brief coordinate and it's correspondence to direction
     */
    static std::unordered_map<coordinate, Direction, mhash> POS_DIR;
};

} // RPG

#endif //RPG_OBJECT_H

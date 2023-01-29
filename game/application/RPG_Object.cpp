#include "RPG_Object.h"

namespace RPG {
    std::unordered_map<Direction, coordinate> RPG_Object::DIR_POS = {{Left, std::make_pair(0, -1)},
                                                                {Down, std::make_pair(1, 0)},
                                                                {Up, std::make_pair(-1, 0)},
                                                                {Right, std::make_pair(0, 1)}};

    std::unordered_map<coordinate, Direction, mhash> RPG_Object::POS_DIR = {{std::make_pair(0, -1), Left},
                                                                {std::make_pair(1, 0), Down},
                                                                {std::make_pair(-1, 0), Up},
                                                                {std::make_pair(0, 1), Right}};
} // RPG
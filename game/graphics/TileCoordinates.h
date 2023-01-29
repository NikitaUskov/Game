#ifndef OOP_SEM5_TILECOORDINATES_H
#define OOP_SEM5_TILECOORDINATES_H

#include "Game.h"

static const std::unordered_map<RPG::CELL_TYPE, std::pair<int, int>> cell_tile_coords {
        {RPG::Floor, std::make_pair(68, 0)},
        {RPG::Wall, std::make_pair(170, 289)},
        {RPG::Glass, std::make_pair(34, 68)},
        {RPG::Partition, std::make_pair(17, 51)},
        {RPG::Storage_point, std::make_pair(187, 102)},
        {RPG::Have_item, std::make_pair(153, 442)}
};

static const std::unordered_map<RPG::UNIT_TYPE, std::pair<int, int>> unit_tile_coords = {
        {RPG::WILD, std::make_pair(476, 102)},
        {RPG::RATIONAL, std::make_pair(527, 102)},
        {RPG::FORAGER, std::make_pair(442, 102)},
        {RPG::OPERATIVE, std::make_pair(408, 0)}
};

static const std::unordered_map<RPG::WEAPON_TYPE, std::pair<int, int>> weapon_tile_coords {
        {RPG::AK_74, std::make_pair(136, 527)},
        {RPG::RPK_74, std::make_pair(170, 527)},
        {RPG::PM, std::make_pair(85, 527)},
        {RPG::TO3_34, std::make_pair(119, 527)}
};

static const std::unordered_map<RPG::MED_KIT_HEALTH, std::pair<int, int>> medicine_kit_tile_coords {
        {RPG::Basic, std::make_pair(306, 510)},
        {RPG::Army, std::make_pair(289, 476)},
        {RPG::Scientific, std::make_pair(272, 425)}
};

static const std::unordered_map<RPG::ITEM_TYPE, std::pair<int, int>> container_tile_coord {
        {RPG::AMMO_CONTAINER, std::make_pair(204, 527)}
};

#endif //OOP_SEM5_TILECOORDINATES_H

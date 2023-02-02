#include "Level.h"

#include <utility>
#include <fstream>
#include <algorithm>

namespace RPG {

std::unordered_map<CELL_TYPE, char> Level::cell2Char = {{Floor, '.'}
                                                                    , {Wall, '#'}
                                                                    , {Glass, '-'}
                                                                    , {Partition, '='}
                                                                    , {Storage_point, ')'}
                                                                    , {Have_item, '('}};

std::unordered_map<UNIT_TYPE, char> Level::enemy2Char = {{WILD, 'W'}
                                                                , {FORAGER, 'F'}
                                                                , {RATIONAL, 'R'}};


Level::Level(): size(std::make_pair(0, 0)), finish_flag(0) {
//    enemies = std::vector<Unit*>();
//    operatives = std::vector<Operative*>();
//    map_ = Map();
}

Level::Level(coordinate size0): size(std::move(size0)), finish_flag(0) {}

Level::Level(coordinate size0,
             std::vector<Operative*> operatives,
             std::vector<Unit*> enemies)
                                        :
                                        size(std::move(size0)),
                                        operatives(std::move(operatives)),
                                        enemies(std::move(enemies)),
                                        finish_flag(0)
                                        {}

coordinate Level::get_size() {
    return size;
}

int Level::change_size(coordinate new_size) {
    if (new_size.first < 0 && new_size.second < 0) {
        return -1;
    }
    size = new_size;
    return 0;
}

void Level::updateChests() {
    std::vector<coordinate> deleteChests;
    for (auto& chest: chestsCoord) {
        if (map_[chest]->get_type() != Storage_point) {
            deleteChests.push_back(chest);
        }
    }
    for (auto& chestCoord: deleteChests) {
        chestsCoord.erase(std::find(chestsCoord.begin(), chestsCoord.end(), chestCoord));
    }
}

std::vector<coordinate> Level::getChestsCoord() const noexcept{
    return chestsCoord;
}

[[nodiscard]] coordinate Level::getStorageCoord() const noexcept {
    return storageCoord;
}

void Level::start(const std::string& path_to_map) {
    std::ifstream fin(path_to_map);
    if (fin.is_open()) {
        fin >> size.first >> size.second;
        std::string str;
        for (int i = 0; i < size.first; ++i) {
            fin >> str;
            for (int j = 0; j < size.second; ++j) {
                if (str[j] == '.') {
                    map_[std::make_pair(i, j)] = new Cell(Floor);
                }
                else if (str[j] == '#') {
                    map_[std::make_pair(i, j)] = new Cell(Wall);
                }
                else if (str[j] == '=') {
                    map_[std::make_pair(i, j)] = new Cell(Partition);
                }
                else if (str[j] == '-') {
                    map_[std::make_pair(i, j)] = new Cell(Glass);
                }
                else if (str[j] == ')') {
                    int med_type = GetRandomNumber(0, 80), weapon_type = GetRandomNumber(0, 3);
                    auto med = (MED_KIT_HEALTH) ((med_type <= 20) ? 20 : ((med_type <= 50) ? 50 : 80));

                    auto cont = new Ammo_container(
                            (AMMO_WEIGHT) ((weapon_type <= 1) ? 6
                                                              : (weapon_type == 2) ? 5 : 7));
                    cont->put_ammo(GetRandomNumber(10, 100));

                    int max_ammo = 0;
                    if (weapon_type == RPG::WEAPON_TYPE::AK_74) max_ammo = 10;
                    else if (weapon_type == RPG::WEAPON_TYPE::RPK_74) max_ammo = 15;
                    else if (weapon_type == RPG::WEAPON_TYPE::PM) max_ammo = 8;
                    else if (weapon_type == RPG::WEAPON_TYPE::TO3_34) max_ammo = 2;

                    std::vector<Item *> items = {dynamic_cast<Item *>(new Medicine_Kit(med)),
                                                 dynamic_cast<Item *>(new Weapon((WEAPON_TYPE) weapon_type,
                                                                                 GetRandomNumber(0
                                                                                                 , max_ammo))),
                                                 dynamic_cast<Item *>(cont)};

                    map_[std::make_pair(i, j)] = new Cell(Storage_point, items);
                    chestsCoord.emplace_back(i, j);
                }
                else if ('a' <= str[j] and str[j] <= 'i') {
                    map_[std::make_pair(i, j)] = new Cell(Floor);
                    operatives.push_back(new Operative(std::make_pair(i, j)
                                                       , new Weapon(AK_74, 10)
                                                       , "player"));
                }
                else if ('W' == str[j]) {
                    map_[std::make_pair(i, j)] = new Cell(Floor);
                    enemies.push_back(dynamic_cast<Unit *>(new Wild("wild", std::make_pair(i, j))));
                }
                else if ('R' == str[j]) {
                    map_[std::make_pair(i, j)] = new Cell(Floor);
                    enemies.push_back(dynamic_cast<Unit *>(new Rational("rational", std::make_pair(i, j))));
                }
                else if ('F' == str[j]) {
                    map_[std::make_pair(i, j)] = new Cell(Floor);
                    enemies.push_back(dynamic_cast<Unit *>(new Forager("forager", std::make_pair(i, j))));
                    storageCoord = std::make_pair(i, j);
                }
            }
        }
    }
    fin.close();
    finish_flag = 0;
}

void Level::save(const std::string& savePath) {
    const std::string saveCell = "cells";
    const std::string saveOperatives = "operatives";
    const std::string saveEnemies = "enemies";
    const std::string saveMap = "map";
    std::string mapInfo;
//    std::ifstream fin(path_to_map);

    int numOper = 0, numCells = 0;
    std::string operName = "abcdefghi";
    for (int i = 0; i < size.first; ++i) {
        for (int j = 0; j < size.second; ++j) {
            mapInfo += cell2Char[map_[std::make_pair(i, j)]->get_type()];
            if (map_[std::make_pair(i, j)]->get_type() == Have_item
            || map_[std::make_pair(i, j)]->get_type() == Storage_point) {
                ++numCells;
            }
        }
        mapInfo += "\n";
    }
    for (auto& currEnemy: enemies) {
        int x = currEnemy->get_position().first, y = currEnemy->get_position().second;
        mapInfo[x * (size.second + 1) + y] = enemy2Char[currEnemy->get_type()];
    }
    for (auto& currOper: operatives) {
        int x = currOper->get_position().first, y = currOper->get_position().second;
        mapInfo[x * (size.second + 1) + y] = operName[numOper];
        ++numOper;
    }
    std::ofstream fout(savePath + saveMap + ".txt");
    if (fout.is_open()) {
        fout << size.first << " " << size.second << "\n" << mapInfo;
    }
    fout.close();

    fout.open(savePath + saveCell + ".txt");
    if (fout.is_open()) {
        fout << numCells << "\n";
        if (numCells > 0) {
            for (int i = 0; i < size.first; ++i) {
                for (int j = 0; j < size.second; ++j) {
                    CELL_TYPE type = map_[std::make_pair(i, j)]->get_type();
                    if (type != Have_item && type != Storage_point) continue;

                    Cell *currCell = map_[std::make_pair(i, j)];
                    fout << (int) type << " " << i << " " << j << " " << currCell->getItems().size() << "\n";
                    for (auto &currItem: currCell->getItems()) {
                        fout << currItem->get_type() << " ";
                        if (currItem->get_type() == MEDICINE_KIT) {
                            fout << (int) dynamic_cast<Medicine_Kit *>(currItem)->get_type() << "\n";
                        } else if (currItem->get_type() == AMMO_CONTAINER) {
                            auto cont = dynamic_cast<Ammo_container *>(currItem);
                            fout << cont->get_type() << " " << cont->num_ammo() << "\n";
                        } else {
                            auto weapon = dynamic_cast<Weapon *>(currItem);
                            fout << weapon->get_type() << " " << weapon->get_ammo_num() << "\n";
                        }
                    }
                }
            }
        }
    }
    fout.close();

    fout.open(savePath + saveOperatives + ".txt");
    if (fout.is_open()) {
        fout << operatives.size() << "\n";
        for (auto currOper: operatives) {
            fout << currOper->get_position().first << " " << currOper->get_position().second << " "
            << currOper->get_params().current_health << " " << currOper->get_params().current_time << "\n";
            Weapon* currWeapon = currOper->get_current_weapon();
            fout << currWeapon->get_type() << " " << currWeapon->get_ammo_num() << "\n";
            int numItems = currOper->get_num_of_items();
            fout << numItems << "\n";
            if (numItems > 0) {
                auto inventoryIter = currOper->see_inventory()->get_iter();
                for (int i = 0; i < numItems; ++i) {
                    auto currItem = *(inventoryIter + i);
                    fout << currItem->get_type() << " ";
                    if (currItem->get_type() == MEDICINE_KIT) {
                        fout << (int) dynamic_cast<Medicine_Kit *>(currItem)->get_type() << "\n";
                    } else if (currItem->get_type() == AMMO_CONTAINER) {
                        auto cont = dynamic_cast<Ammo_container *>(currItem);
                        fout << cont->get_type() << " " << cont->num_ammo() << "\n";
                    } else {
                        auto weapon = dynamic_cast<Weapon *>(currItem);
                        fout << weapon->get_type() << " " << weapon->get_ammo_num() << "\n";
                    }
                }
            }
        }
    }
    fout.close();

    fout.open(savePath + saveEnemies + ".txt");
    if (fout.is_open()) {
        fout << enemies.size() << "\n";
        for (auto currEnemy: enemies) {
            fout << currEnemy->get_type() << " "
            << currEnemy->get_position().first << " " << currEnemy->get_position().second << " "
            << currEnemy->get_params().current_health << " " << currEnemy->get_params().current_time << "\n";
            UNIT_TYPE type = currEnemy->get_type();
            if (type == RATIONAL) {
                Weapon *currWeapon = dynamic_cast<Rational*>(currEnemy)->get_current_weapon();
                if (currWeapon == nullptr) fout << 0 << "\n";
                else {
                    fout << 1 << "\n";
                    fout << currWeapon->get_type() << " " << currWeapon->get_ammo_num() << "\n";
                }
            }
            else if (type == FORAGER) {
                auto forager = dynamic_cast<Forager*>(currEnemy);
                int numItems = forager->get_num_of_items();
                fout << numItems << "\n";
                if (numItems > 0) {
                    auto inventoryIter = forager->see_inventory()->get_iter();
                    for (int i = 0; i < numItems; ++i) {
                        auto currItem = *(inventoryIter + i);
                        fout << currItem->get_type() << " ";
                        if (currItem->get_type() == MEDICINE_KIT) {
                            fout << (int) dynamic_cast<Medicine_Kit *>(currItem)->get_type() << "\n";
                        } else if (currItem->get_type() == AMMO_CONTAINER) {
                            auto cont = dynamic_cast<Ammo_container *>(currItem);
                            fout << cont->get_type() << " " << cont->num_ammo() << "\n";
                        } else {
                            auto weapon = dynamic_cast<Weapon *>(currItem);
                            fout << weapon->get_type() << " " << weapon->get_ammo_num() << "\n";
                        }
                    }
                }
            }
        }
    }
    fout.close();
}

void Level::load(const std::string& loadPath) {
    const std::string loadCell = "cells";
    const std::string loadOperatives = "operatives";
    const std::string loadEnemies = "enemies";
    const std::string loadMap = "map";
    std::ifstream fin(loadPath + loadCell + ".txt");

    if (fin.is_open()) {
        int numCells;
        fin >> numCells;
        if (numCells > 0) {
            for (int j = 0; j < numCells; ++j) {
                int typeCell, x, y, numItems;
                fin >> typeCell >> x >> y >> numItems;
                std::vector<Item *> items;
                int typeItem, type, ammoNum;
                for (int i = 0; i < numItems; ++i) {
                    fin >> typeItem >> type;
                    if (typeItem > 0) fin >> ammoNum;
                    if (typeItem == MEDICINE_KIT) {
                        items.push_back(dynamic_cast<Item *>(new Medicine_Kit((MED_KIT_HEALTH) type)));
                    } else if (typeItem == AMMO_CONTAINER) {
                        auto cont = new Ammo_container((AMMO_WEIGHT) type);
                        cont->put_ammo(ammoNum);
                        items.push_back(dynamic_cast<Item *>(cont));
                    } else {
                        items.push_back(dynamic_cast<Item *>(new Weapon((WEAPON_TYPE) type, ammoNum)));
                    }
                }
                map_[std::make_pair(x, y)] = new Cell((CELL_TYPE) typeCell, items);
            }
        }
    }
    fin.close();

    fin.open(loadPath + loadMap + ".txt");
    if (fin.is_open()) {
        fin >> size.first >> size.second;
        for (int i = 0; i < size.first; ++i) {
            std::string str;
            fin >> str;
            for (int j = 0; j < size.second; ++j) {
                if (str[j] == '.' || str[j] == 'W' || str[j] == 'R' || str[j] == 'F'
                || ('a' <= str[j] && str[j] <= 'i')) {
                    if (map_[std::make_pair(i, j)]) continue;
                    map_[std::make_pair(i, j)] = new Cell(Floor);
                }
                else if (str[j] == '#') {
                    map_[std::make_pair(i, j)] = new Cell(Wall);
                }
                else if (str[j] == '=') {
                    map_[std::make_pair(i, j)] = new Cell(Partition);
                }
                else if (str[j] == '-') {
                    map_[std::make_pair(i, j)] = new Cell(Glass);
                }
            }
        }
    }
    fin.close();


    fin.open(loadPath + loadOperatives + ".txt");
    if (fin.is_open()) {
        int numOperatives;
       fin >> numOperatives;
       int x, y, health, time;
       for (int i = 0; i < numOperatives; ++i) {
           fin >> x >> y >> health >> time;
           int typeWeapon, ammoNum, itemsNum;
           fin >> typeWeapon >> ammoNum;
           auto currWeapon = new Weapon((WEAPON_TYPE) typeWeapon, ammoNum);
           fin >> itemsNum;
           if (itemsNum > 0) {
               std::vector<Item*> items;
               int typeItem, type;
               for (int j = 0; j < itemsNum; ++j) {
                   fin >> typeItem >> type;
                   if (typeItem > 0) fin >> ammoNum;
                   if (typeItem == MEDICINE_KIT) {
                       items.push_back(dynamic_cast<Item*>(new Medicine_Kit((MED_KIT_HEALTH) type)));
                   }
                   else if (typeItem == AMMO_CONTAINER) {
                       auto cont = new Ammo_container((AMMO_WEIGHT) type);
                       cont->put_ammo(ammoNum);
                       items.push_back(dynamic_cast<Item*>(cont));
                   }
                   else {
                       items.push_back(dynamic_cast<Item*>(new Weapon((WEAPON_TYPE) type, ammoNum)));
                   }
               }
               operatives.push_back(new Operative(std::make_pair(x, y)
                       , currWeapon
                       , items
                       , "player"));
           }
           else {
               operatives.push_back(new Operative(std::make_pair(x, y)
                                                  , currWeapon
                                                  , "player"));
           }
           operatives[i]->setParams(health, time);
       }
    }
    fin.close();


    fin.open(loadPath + loadEnemies + ".txt");
    if (fin.is_open()) {
        int numEnemies;
        fin >> numEnemies;
        int typeEnemy, x, y, health, time;
        for (int i = 0; i < numEnemies; ++i) {
            fin >> typeEnemy >> x >> y >> health >> time;
            if (typeEnemy == WILD) {
                auto wild = new Wild("wild", std::make_pair(x, y));
                wild->setParams(health, time);
                enemies.push_back(dynamic_cast<Unit *>(wild));
            } else if (typeEnemy == RATIONAL) {
                int typeWeapon, ammoNum, flag;
                fin >> flag;
                Weapon* currWeapon = nullptr;
                if (flag > 0) {
                    fin >> typeWeapon >> ammoNum;
                    currWeapon = new Weapon((WEAPON_TYPE) typeWeapon, ammoNum);
                }
                Rational* rational;
                if (currWeapon != nullptr) {
                    rational = new Rational("rational", std::make_pair(x, y), currWeapon);
                }
                else {
                    rational = new Rational("rational", std::make_pair(x, y));
                }
                rational->setParams(health, time);
                enemies.push_back(dynamic_cast<Unit *>(rational));
            } else if (typeEnemy == FORAGER) {
                int itemsNum, ammoNum;
                fin >> itemsNum;
                if (itemsNum > 0) {
                    std::vector<Item *> items;
                    int typeItem, type;
                    for (int j = 0; j < itemsNum; ++j) {
                        fin >> typeItem >> type;
                        if (typeItem > 0) fin >> ammoNum;
                        if (typeItem == MEDICINE_KIT) {
                            items.push_back(dynamic_cast<Item*>(new Medicine_Kit((MED_KIT_HEALTH) type)));
                        } else if (typeItem == AMMO_CONTAINER) {
                            auto cont = new Ammo_container((AMMO_WEIGHT) type);
                            cont->put_ammo(ammoNum);
                            items.push_back(dynamic_cast<Item*>(cont));
                        } else {
                            items.push_back(dynamic_cast<Item*>(new Weapon((WEAPON_TYPE) type, ammoNum)));
                        }
                    }
                    auto forager = new Forager("forager", std::make_pair(x, y), items);
                    forager->setParams(health, time);
                    enemies.push_back(dynamic_cast<Unit*>(forager));
                } else {
                    auto forager = new Forager("forager", std::make_pair(x, y));
                    forager->setParams(health, time);
                    enemies.push_back(dynamic_cast<Unit*>(forager));
                }
            }
        }
    }
    fin.close();

    finish_flag = 0;
}

int Level::check_flag() const noexcept{
    return finish_flag;
}

int Level::shoot(Wild* who, Direction where) {
    int res_attack = who->attack();
    if (res_attack != 0) {
        return res_attack;
    }
    int range_ = who->get_range();
    std::vector<RPG_Object*> targets;
    //targets.reserve(range_);
    for (int i = 0; i < range_; ++i) {
        targets.push_back(nullptr);
    }
    auto curr_target = targets.begin();
    coordinate pos = who->get_position(), max_pos;
    max_pos.first = pos.first + RPG_Object::DIR_POS[where].first * (range_ + 1);
    max_pos.second = pos.second + RPG_Object::DIR_POS[where].second * (range_ + 1);
    if ((max_pos.first > size.first) or (max_pos.second > size.second)) {
        max_pos.first = (max_pos.first > size.first) ? size.first : max_pos.first;
        max_pos.second = (max_pos.second > size.second) ? size.second : max_pos.second;
    }
    else if ((max_pos.first <= 0) or (max_pos.second <= 0)) {
        max_pos.first = (max_pos.first < 0) ? -1 : max_pos.first;
        max_pos.second = (max_pos.second < 0) ? -1 : max_pos.second;
    }

    for (pos += RPG_Object::DIR_POS[where]; pos != max_pos; pos += RPG_Object::DIR_POS[where]) {
        for (auto curr_oper: operatives) {
            if (curr_oper->get_position() == pos) {
                *curr_target = dynamic_cast<RPG_Object*>(dynamic_cast<Unit*>(curr_oper));
            }
        }
        for (auto curr_enemy: enemies) {
            if (curr_enemy->get_position() == pos) {
                *curr_target = dynamic_cast<RPG_Object*>(dynamic_cast<Unit*>(curr_enemy));
            }
        }

        if (map_[pos]->can_go_through() == false) {
            *curr_target = dynamic_cast<RPG_Object*>(map_[pos]);
        }
        ++curr_target;
    }
    curr_target = targets.begin();
    res_attack = 1;
    while (res_attack == 1 && curr_target < targets.end()) {
        if (*curr_target != nullptr) {
            res_attack = (*curr_target)->get_damage(who->get_num_damage());
        }
        else {
            ++curr_target;
        }
    }

    // delete died unit...
    if (res_attack == 100) {
        Unit *curr_unit = dynamic_cast<Unit*>(*curr_target);
        UNIT_TYPE type_curr_enemy = curr_unit->get_type();
        if (type_curr_enemy == WILD) {
            enemies.erase(std::find(enemies.begin(), enemies.end(), dynamic_cast<Unit*>(curr_unit)));
            delete dynamic_cast<Wild*>(curr_unit);
        }
        else if (type_curr_enemy == RATIONAL) {
            enemies.erase(std::find(enemies.begin(), enemies.end(), dynamic_cast<Unit*>(curr_unit)));
            delete dynamic_cast<Rational*>(curr_unit);
        }
        else if (type_curr_enemy == FORAGER) {
            dynamic_cast<Forager*>(curr_unit)->put_all_items_from_inventory(map_);
            enemies.erase(std::find(enemies.begin(), enemies.end(), dynamic_cast<Unit*>(curr_unit)));
            delete dynamic_cast<Forager*>(curr_unit);
        }
        else if (type_curr_enemy == OPERATIVE) {
            dynamic_cast<Operative*>(curr_unit)->put_all_items_from_inventory(map_);
            operatives.erase(std::find(operatives.begin(), operatives.end(), dynamic_cast<Operative*>(curr_unit)));
            delete dynamic_cast<Operative*>(curr_unit);
        }
    }
    if (operatives.empty()) {
        finish_flag = 2;
    }
    else if (enemies.empty()) {
        finish_flag = 1;
    }
    return (res_attack < 0 ? 0: res_attack);
}

int Level::step_by_unit(Unit* who, Direction where) {
    coordinate probably_pos = who->get_position();
    probably_pos += RPG_Object::DIR_POS[where];
    int can_step = 0; // 0 - can, 1 - can't
    for (auto curr_oper: operatives) {
        if (curr_oper->get_position() == probably_pos) {
            can_step = 1;
        }
    }
    for (auto curr_enemy: enemies) {
        if (curr_enemy->get_position() == probably_pos) {
            can_step = 1;
        }
    }
    if (can_step == 1) {
        return -1;
    }
    int res_step = who->step(where, map_);
    return res_step;
}

Level::~Level() {
    for (auto& curr_cell: map_) {
        delete curr_cell.second;
    }
    for (auto& curr_operative: operatives) {
        delete curr_operative;
    }
    for (auto& curr_enemy: enemies) {
        UNIT_TYPE type_curr_enemy = curr_enemy->get_type();
        if (type_curr_enemy == WILD) {
            delete dynamic_cast<Wild*>(curr_enemy);
        }
        else if (type_curr_enemy == RATIONAL) {
            delete dynamic_cast<Rational*>(curr_enemy);
        }
        else if (type_curr_enemy == FORAGER) {
            delete dynamic_cast<Forager*>(curr_enemy);
        }
        else {
            delete curr_enemy;
        }
    }
}

}// RPG
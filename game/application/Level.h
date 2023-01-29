#ifndef LEVEL_H
#define LEVEL_H

#include "Operative.h"

namespace RPG {



/**
 * @brief Level - has map, operatives and enemies and manage to they
 */
class Level {
public:
    /**
     * @brief default constructor of Level
     */
    Level();
    /**
     * @brief constructor of Level with size only
     * @param size0 (coordinate) - size of Level
     */
    explicit Level(coordinate size0);
    /**
     * @brief full constructor of level
     * @param size0 (coordinate) - size of level map
     * @param operatives (vector<Operative*>) - vector of operatives on level
     * @param enemies (vector<Unit*>) - vector of enemies on level
     */
    explicit Level(coordinate size0, std::vector<Operative*> operatives, std::vector<Unit *> enemies);

    /**
     * @brief check size of map
     * @return (coordinate) - size of map
     */
    coordinate get_size();
    /**
     * @brief change size and rebuild level
     * @param new_size (coordinate) - new size of map
     * @return (int) - -1 - incorrect data, \n
     * 0 - successfully changed
     */
    int change_size(coordinate new_size);
    /**
     * @brief attack of unit
     * @param who (Wild*) - who does the damage
     * @param where (Direction) - where unit do attack
     * @return (int) - result of attack: \n
     * \<0 - attacker mob can't do it, \n
     *  1 - attacker missing, \n
     *  0 - successfully shot, \n
     *  100 - kill and delete unit
     */
    int shoot(Wild* who, Direction where);
    /**
     * @brief step with check map (two unit can't stay in one cell)
     * @param who (Unit*) - who is stepping
     * @param where (Direction) - where unit want to step
     * @return (int) - result of step: \n
     * -1 - can't step, \n
     * 1 - haven't time, \n
     * 0 - successfully step
     */
    int step_by_unit(Unit* who, Direction where);

    void updateChests();

    [[nodiscard]] std::vector<coordinate> getChestsCoord() const noexcept;

    [[nodiscard]] coordinate getStorageCoord() const noexcept;
    /**
     * @brief initialize basic params
     * @param path_to_map (string) - path fo config file
     */
    void start(const std::string& path_to_map);

    void save(const std::string& savePath);
    void load(const std::string& loadPath);
    /**
     * @brief check finish flag
     * @return (int) - finish flag: \n
     * 0 - don't finish, \n
     * 1 - win, \n
     * 2 - lose
     */
    [[nodiscard]] int check_flag() const noexcept;

    /**
     * @brief map of level
     */
    Map map_;
    /**
     * @brief all operatives on level
     */
    std::vector<Operative*> operatives;
    /**
     * @brief all enemies on level
     */
    std::vector<Unit *> enemies;
    static std::unordered_map<CELL_TYPE, char> cell2Char;
    static std::unordered_map<UNIT_TYPE, char> enemy2Char;
    /**
     * @brief destructor - delete all Cells, operatives and enemies
     */
    ~Level();
protected:
    /**
     * @brief (coordinate) - size of map
     */
    coordinate size;
    /**
     * @brief (int) - finish flag: \n
     * 0 - don't finish, \n
     * 1 - win, \n
     * 2 - lose
     */
    int finish_flag;

    std::vector<coordinate> chestsCoord;

    coordinate storageCoord;
};

} // RPG
#endif //LEVEL_H

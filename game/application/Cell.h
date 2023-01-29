#ifndef CELL_H
#define CELL_H

//#include "Weapon.h"
#include "Table.h"

namespace RPG {

/**
 * @brief enum class with all types of Cell
 * @enum
 *  Floor,         // basic cell \n
 *  Wall,          // can't destroy and see \n
 *  Glass,         // can destroy and see \n
 *  Partition,     // can destroy and can't see \n
 *  Storage_point, // special cell \n
 *  Have_item      // have items
 */
enum CELL_TYPE {
    Floor = 0,
    Wall,          // can't destroy and see
    Glass,         // can destroy and see
    Partition,     // can destroy and can't see
    Storage_point, // have items
    Have_item      // if anything put item, will have it
};

/**
 * @brief parameters of cell
 * @struct
 * (bool) visible; \n
 * (bool) destroy; \n
 * (bool) go_through; \n
 */
struct cell_params {
    bool visible;
    bool destroy;
    bool go_through;
};

/**
 * @class Cell hear of RPG_Object
 */
class Cell : public RPG_Object {
public:
    /**
     * @brief default constructor - floor
     */
    Cell();
    /**
     * @brief constructor with only type (for wall, glass, partition)
     * @param type0 - (CELL_TYPE) - type of cell
     */
    explicit Cell(CELL_TYPE type0);
    /**
     * @brief constructor with type and items (for storage and have_items)
     * @param type0 - (CELL_TYPE) - type of cell
     * @param items - (vector<Item*>) - all_items in the cell
     */
    explicit Cell(CELL_TYPE type, std::vector<Item*> items);
    /**
     * @brief check type of cell
     * @return (CELL_TYPE) - type of cell
     */
    [[nodiscard]] CELL_TYPE get_type() const;
    /**
     * @brief take item from cell
     * @param item (Item*) pointer to item with same parameters
     * @return (Item*) pointer of item - successfully take \n
     * nullptr - cell doesn't have this item
     */
    Item* take_item(Item* item);
    /**
     * @brief take first item from cell
     * @return (Item*) pointer of item - successfully take \n
     * nullptr - cell doesn't have this item
     */
    Item* take_item();
    /**
     * @brief put item to cell
     * @param item - (Item*) pointer to item with same parameters
     * @return (int) 0 - successfully put \n
     * -1 - you try put to wall/glass/partition
     */
    int put_item(Item* item);
    /**
     * @brief check can you see through cell
     * @return true or false - can you see
     */
    [[nodiscard]] bool is_visible() const;
    /**
     * @brief get damage by cell (destroy, fly through or don't destroy)
     * @return (int) 1 - destroy cell (change type to floor)
     * -1 - you can't destroy it
     * 0 - you can go through it
     */
    int get_damage(int) override;
    /**
     * @brief check can you go through cell
     * @return true or false - can you go
     */
    [[nodiscard]] bool can_go_through() const;
    /**
     * @brief table with type of cell and their correspondence to parameters
     */
    static std::unordered_map<CELL_TYPE, cell_params> CELL_TYPE_PARAMS;
    std::vector<Item*> getItems();
    /**
     * @brief just destructor which delete items in cell
     */
    ~Cell();

protected:
    /**
     * @brief (CELL_TYPE) - type of cell
     */
    CELL_TYPE type;
    /**
     * @brief (vector<Item*>) - all items in cell in current moment
     */
    std::vector<Item *> all_items;
    /**
     * @brief (cell_parameters) - basic parameters of cell
     */
    cell_params parameters;
    /**
     * @brief special methods for change type of cell
     * @param new_type (CELL_TYPE) - new type of cell
     * @return (int) 0 - successfully
     */
    int change_type(CELL_TYPE new_type); // change type, when we destroy cell
};

/**
 * @brief short name of MyUnorderedMap<coordinate, Cell*, mhash>
 */
using Map = std::unordered_map<coordinate, Cell*, mhash>;

}//RPG
#endif //CELL_H

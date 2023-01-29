#include "Game.h"
#include <utility>

namespace RPG {

Game::Game(Level *level0) : level(level0) {}

static std::unordered_map<char, RPG::Direction> char_direction = {
                                                                    {sf::Keyboard::W, RPG::Up},
                                                                    {sf::Keyboard::S, RPG::Down},
                                                                    {sf::Keyboard::A, RPG::Left},
                                                                    {sf::Keyboard::D, RPG::Right}};

int Game::turn_operatives(sf::Keyboard::Key choice, sf::RenderWindow &window, int *diff) {
    int resTurn = 0;
    auto currOper = level->operatives.begin() + *diff;
    // e - end turn
    if (choice == sf::Keyboard::E) {
        return -100;
    }
        // r - reload weapon
    else if (choice == sf::Keyboard::R) {
        resTurn = (*currOper)->reload();
    }
    else if (choice == sf::Keyboard::Q) {
        auto scientific = new Medicine_Kit(Scientific);
        auto army = new Medicine_Kit(Army);
        auto basic = new Medicine_Kit(Basic);
        resTurn = (*currOper)->heal(scientific);
        if (resTurn == -1) {
            resTurn = (*currOper)->heal(army);
            if (resTurn == -1) {
                resTurn = (*currOper)->heal(basic);
            }
        }
        delete scientific;
        delete army;
        delete basic;
    }
        // f - choice direction and shoot
    else if (choice == sf::Keyboard::F) {
        sf::Keyboard::Key direction = get_input(window);
        if (direction == sf::Keyboard::W or direction == sf::Keyboard::S
            or direction == sf::Keyboard::A or direction == sf::Keyboard::D) {
            resTurn = level->shoot(*currOper, char_direction.at(direction));
        }
    }
        // g - take item
    else if (choice == sf::Keyboard::G) {
        resTurn = (*currOper)->take_item_to_inventory(level->map_);
    }
        // i - inventory mode
    else if (choice == sf::Keyboard::I) {
        resTurn = 77;
    }
    else if ((sf::Keyboard::Num1 <= choice and choice <= sf::Keyboard::Num9)
            or (sf::Keyboard::Numpad1 <= choice and choice <= sf::Keyboard::Numpad9)) {
        int oper_num = (choice > sf::Keyboard::Numpad0) ? choice - sf::Keyboard::Numpad1 : choice - sf::Keyboard::Num1;
        if (oper_num >= 0 and oper_num < level->operatives.size()) {
            *diff = oper_num;
            //currOper = level.operatives.begin() + oper_num;
        }
        else {
            resTurn = -1;
        }
    }
    else if (choice == sf::Keyboard::W or choice == sf::Keyboard::S
            or choice == sf::Keyboard::A or choice == sf::Keyboard::D) {
        resTurn = level->step_by_unit(*currOper, char_direction[choice]);
    }
    return resTurn;
}

int Game::inventoryActions(Operative* currOperative, sf::Texture& texture, sf::Text& text) {
    auto inventory = currOperative->see_inventory();
    int resActions = 1, sizeInventory = inventory->number_of_items(), indexCurrItem = 0;
    int inventoryWidth = 5;
    coordinate sizeWindow = std::make_pair((sizeInventory / inventoryWidth) + 1 + 2 + 2, inventoryWidth);
    // if (sizeInventory == 0) return resActions;
    sf::RenderWindow windowInventory(
            sf::VideoMode(inventoryWidth * RPG::tile_size.y * RPG::scale
                          , sizeWindow.first * RPG::tile_size.x * RPG::scale)
            , "Inventory");

    while(windowInventory.isOpen()) {

        RPG::TileOnMap::drawFullRow(windowInventory, 0
                                    , sizeWindow.second, std::make_pair(0, 0), texture);

        RPG::TileOnMap::drawFullRow(windowInventory, 1
                                    , sizeWindow.second
                                    , std::make_pair((tile_size.x + 1) * 23, (tile_size.y + 1) * 2)
                                    , texture);

        RPG::TileOnMap::drawFullRow(windowInventory, sizeWindow.first - 2
                                    , sizeWindow.second
                                    , std::make_pair((tile_size.x + 1) * 23, (tile_size.y + 1) * 2)
                                    , texture);

        RPG::TileOnMap::drawFullRow(windowInventory, sizeWindow.first - 1
                                    , sizeWindow.second, std::make_pair(0, 0), texture);

        itemActions(dynamic_cast<Item*>(currOperative->get_current_weapon())
                    , windowInventory
                    , texture
                    , text
                    , 0
                    , false);

        sf::Text text_;
        std::ostringstream status;
        status << "rel: " << currOperative->get_current_weapon()->get_params().bas_params.reload_time
        << "s\n" << "weight:\n" << currOperative->get_weight() << " /\n" << currOperative->get_strength()
        << "\nhp: " << currOperative->get_params().current_health << std::endl;
        text_.setString(status.str());
        text_.setFillColor(sf::Color::Yellow);
        text_.setPosition(4 * tile_size.y * scale, 0);
        text_.setFont(*text.getFont());
        text_.setCharacterSize(RPG::font_size);
        windowInventory.draw(text_);

        std::vector<sf::Sprite> itemSprites;
        auto inventoryIter = inventory->get_iter();
        for (int i = 0; i < sizeInventory; ++i) {
            sf::Sprite itemSprite = sf::Sprite(texture);
            ITEM_TYPE type = (*(inventoryIter + i))->get_type();
            RPG::coordinate sprite_coord;
            if (type == WEAPON) {
                sprite_coord = weapon_tile_coords.at(dynamic_cast<Weapon *>(*(inventoryIter + i))->get_type());
            }
            else if (type == MEDICINE_KIT) {
                sprite_coord = medicine_kit_tile_coords.at(dynamic_cast<Medicine_Kit *>(*(inventoryIter + i))->get_type());
            }
            else {
                sprite_coord = container_tile_coord.at(type);
            }

            itemSprite.setTextureRect({sprite_coord.first, sprite_coord.second
                                       , tile_size.x, tile_size.y});

            itemSprite.setPosition((i % inventoryWidth) * tile_size.y * scale,
                                   (i / inventoryWidth + 2) * tile_size.x * scale);
            itemSprite.setScale(scale, scale);
            itemSprites.push_back(itemSprite);
        }
        int sizeFullInventory = ((sizeInventory / inventoryWidth) + 1) * inventoryWidth;
        for (int i = sizeInventory; i < sizeFullInventory; ++i) {
            sf::Sprite itemSprite = sf::Sprite(texture);
            itemSprite.setTextureRect({(tile_size.x + 1) * 4, (tile_size.y + 1) * 6
                                              , tile_size.x, tile_size.y});

            itemSprite.setPosition((i % inventoryWidth) * tile_size.y * scale,
                                   (i / inventoryWidth + 2) * tile_size.x * scale);
            itemSprite.setScale(scale, scale);
            itemSprites.push_back(itemSprite);
        }

        if (sizeInventory > 0) {
            itemSprites[indexCurrItem].setTextureRect(
                    {(tile_size.x + 1) * 20, (tile_size.y + 1) * 22
                     , tile_size.x, tile_size.y});
        }

        RPG::TileOnMap::drawInventory(windowInventory, itemSprites);
        if (sizeInventory > 0) {
            itemActions(*(inventoryIter + indexCurrItem)
                        , windowInventory
                        , texture
                        , text
                        , sizeWindow.first - 1);
        }

        windowInventory.display();

        sf::Keyboard::Key choice = RPG::get_input(windowInventory);

        if (choice == sf::Keyboard::Escape or choice == sf::Keyboard::Tilde) {
            windowInventory.close();
        }
        else if (sizeInventory > 0) {
            if (choice == sf::Keyboard::A) {
                indexCurrItem = (indexCurrItem > 0) ? indexCurrItem - 1 : sizeInventory - 1;
            }
            else if (choice == sf::Keyboard::D) {
                indexCurrItem = (indexCurrItem + 1) % sizeInventory;
            }
            else if (choice == sf::Keyboard::H) {
                if ((*(inventoryIter + indexCurrItem))->get_type() == WEAPON) {
                    currOperative->choice_weapon(dynamic_cast<Weapon *>(*(inventoryIter + indexCurrItem)));
                    indexCurrItem = 0;
                    sizeInventory = inventory->number_of_items();
                }
            }
            else if (choice == sf::Keyboard::Q) {
                if ((*(inventoryIter + indexCurrItem))->get_type() == MEDICINE_KIT) {
                    currOperative->heal(dynamic_cast<Medicine_Kit*>(*(inventoryIter + indexCurrItem)));
                    indexCurrItem = 0;
                    sizeInventory = inventory->number_of_items();
                }
            }
            else if (choice == sf::Keyboard::T) {
                currOperative->put_item_from_inventory(*(inventoryIter + indexCurrItem), level->map_);
                sizeInventory = inventory->number_of_items();
                indexCurrItem = 0;
            }
        }
    }


    return resActions;
}

void Game::itemActions(Item* currItem
                       , sf::RenderWindow& window
                       , sf::Texture& texture
                       , sf::Text& text
                       , int shift
                       , bool printOptions) {

    sf::Text textName, textParams, textOptions;
    std::vector<sf::Text> texts;
    std::ostringstream status, type, options;
    coordinate sprite_coord;
    std::vector<sf::Text> itemTexts;

    if (currItem != nullptr) {
        ITEM_TYPE typeItem = currItem->get_type();
        sf::Sprite itemSprite = sf::Sprite(texture);

        status << "weight: " << currItem->get_weight() << "\n";
        if (typeItem == MEDICINE_KIT) {
            auto medKit = dynamic_cast<Medicine_Kit *>(currItem);
            type << RPG::Medicine_Kit::TYPE_NAME_med[medKit->get_type()] << std::endl;
            status << "heal point: " << medKit->get_type() << std::endl;
            options << "use:\ntap Q\n\n" << "drop:\ntap T" << std::endl;
            sprite_coord = medicine_kit_tile_coords.at(medKit->get_type());
        } else if (typeItem == WEAPON) {
            auto weapon = dynamic_cast<Weapon *>(currItem);
            type << RPG::Weapon::TYPE_NAME_weapon[weapon->get_type()] << std::endl;
            status << "damage: " << weapon->get_params().bas_params.damage << "\n" << "ammo: " << weapon->get_ammo_num()
                   << " / " << weapon->get_params().bas_params.max_ammo << "\n" << "type ammo:\n"
                   << RPG::Ammo_container::TYPE_NAME_ammo[weapon->get_params().bas_params.ammo_type] << std::endl;
            options << "change:\ntap H\n\n" << "drop:\ntap T" << std::endl;
            sprite_coord = weapon_tile_coords.at(weapon->get_type());
        } else {
            auto ammoCont = dynamic_cast<Ammo_container *>(currItem);
            type << "Ammo\ncont" << std::endl;
            status << "ammo: " << ammoCont->num_ammo() << " / " << ammoCont->max_ammo() << "\n"
                   << "type ammo:\n" << RPG::Ammo_container::TYPE_NAME_ammo[ammoCont->get_type()] << std::endl;
            options << "drop:\ntap T" << std::endl;
            sprite_coord = container_tile_coord.at(typeItem);
        }

        itemSprite.setTextureRect({sprite_coord.first, sprite_coord.second, tile_size.x, tile_size.y});
        itemSprite.setPosition(0, shift * tile_size.x * scale);
        itemSprite.setScale(scale, scale);
        window.draw(itemSprite);

        textName.setString(type.str());
        textName.setFillColor(sf::Color::Green);
        itemTexts.push_back(textName);

        textParams.setString(status.str());
        itemTexts.push_back(textParams);

        if (printOptions) {
            textOptions.setString(options.str());
            textOptions.setFillColor(sf::Color::Magenta);
            itemTexts.push_back(textOptions);
        }

        int i = 1;
        for (auto &partText: itemTexts) {
            partText.setPosition(i * tile_size.y * scale, shift * tile_size.x * scale);
            partText.setFont(*text.getFont());
            partText.setCharacterSize(RPG::font_size);
            window.draw(partText);
            if (i == 2) ++i;
            ++i;
        }
    }


}

int Game::turn_enemies(sf::RenderWindow& window, sf::Texture& texture, sf::Text& text) {
    for (auto& currEnemy: level->enemies) {
        if (currEnemy->get_type() == WILD) {
            wildTurn(dynamic_cast<Wild*>(currEnemy), window, texture, text);
        }
    }
    for (auto& currEnemy: level->enemies) {
        if (currEnemy->get_type() == FORAGER) {
            foragerTurn(dynamic_cast<Forager*>(currEnemy), window, texture, text);
        }
    }
    for (auto& currEnemy: level->enemies) {
        if (currEnemy->get_type() == RATIONAL) {
            rationalTurn(dynamic_cast<Rational*>(currEnemy), window, texture, text);
        }
    }
    return 0;
}

void Game::wildTurn(Wild* currWild, sf::RenderWindow& window, sf::Texture& texture, sf::Text& text) {

    int timeWait = 300;
    currWild->update_time();
    RPG::GenerateTables();

    std::vector<coordinate> operativesPos;
    for (auto& operative : level->operatives) {
        operativesPos.push_back((*operative).get_position());
    }
    int flagDie, currTime;
    bool flagOperAround;

    do {
        currTime = currWild->get_params().current_time;
        RPG::draw(window, texture, text, *level);
        flagOperAround = true;
        while (flagOperAround) {
            flagDie = 0;  // operator is live
            for (auto operPos: operativesPos) {
                coordinate currPos = currWild->get_position();
                coordinate diffCoor = operPos - currPos;
                // if operative around wild - attack
                if (diffCoor == RPG_Object::DIR_POS[Left] || diffCoor == RPG_Object::DIR_POS[Down]
                    || diffCoor == RPG_Object::DIR_POS[Up] || diffCoor == RPG_Object::DIR_POS[Right]) {

                    while (currWild->get_params().current_time > 0 && flagDie != 100) {
                        flagDie = level->shoot(currWild, RPG_Object::POS_DIR[diffCoor]);
                        RPG::draw(window, texture, text, *level);
                        std::this_thread::sleep_for(std::chrono::milliseconds( timeWait));
                    }
                    if (flagDie == 100) {
                        operativesPos.erase(std::find(operativesPos.begin(), operativesPos.end(), operPos));
                        break;
                    }
                }
            }
            if (level->check_flag() > 0) break;
            flagOperAround = false;

            if (currWild->get_params().current_time <= 0) break;
            for (auto operPos: operativesPos) {
                // if wild can see operative - go to him
                if (isSeeUnit(currWild->get_position(), operPos)) {
                    std::vector<Direction> path2Oper;
                    if (pathToPoint(path2Oper, currWild->get_position(), operPos)) {
                        for (auto whereStep: path2Oper) {
                            if (currWild->get_params().current_time <= 0) break;
                            coordinate currPos = currWild->get_position();
                            if (operPos - currPos == RPG_Object::DIR_POS[whereStep]) {
                                level->shoot(currWild, whereStep);
                                flagOperAround = true;
                            }
                            else level->step_by_unit(currWild, whereStep);

                            RPG::draw(window, texture, text, *level);
                            std::this_thread::sleep_for(std::chrono::milliseconds( timeWait));
                        }
                    }
                }
                if (flagOperAround) break;
            }
        }
        if (currWild->get_params().current_time <= 0 || level->check_flag() > 0) break;
        // if wild alone
        else {
            // destroy all objects
            RPG::coordinate c;
            std::set<coordinate> visibleCells;
            int x = currWild->get_position().first, y = currWild->get_position().second;
            int height = level->get_size().first, width = level->get_size().second;
            for (int i = 0; i < RPG::numOfRays; ++i) {
                c = RPG::CastRay(y, height - x - 1
                                 , currWild->get_params().view_radius
                                 , i * RPG::graduate
                                 , height, width);

                visibleCells = RPG::TileOnMap::LoS(level->map_
                        , visibleCells
                        , y, height - x - 1
                        , c.first, c.second
                        , height);
            }

            int timeBeforeActions = currWild->get_params().current_time;
            for (auto currCellCoord: visibleCells) {
                if (currWild->get_params().current_time <= 0
                || timeBeforeActions != currWild->get_params().current_time) break;

                else if (RPG::Cell::CELL_TYPE_PARAMS[level->map_[currCellCoord]->get_type()].destroy) {
                    std::vector<Direction> path2Cell;
                    if (pathToPoint(path2Cell, currWild->get_position(), currCellCoord)) {
                        for (auto whereStep: path2Cell) {
                            if (currWild->get_params().current_time <= 0) break;
                            coordinate currPos = currWild->get_position();
                            if (currCellCoord - currPos == RPG_Object::DIR_POS[whereStep]) {
                                level->shoot(currWild, whereStep);
                            }
                            else level->step_by_unit(currWild, whereStep);

                            RPG::draw(window, texture, text, *level);
                            std::this_thread::sleep_for(std::chrono::milliseconds( timeWait));
                        }
                    }
                }
            }

            // go to random point
            if (timeBeforeActions == currWild->get_params().current_time) {
                if (currWild->get_params().current_time <= 0) break;
                std::vector<coordinate> canGoCoord;
                for (auto currCellCoord: visibleCells) {
                    if (level->map_[currCellCoord]->can_go_through()) {
                        canGoCoord.push_back(currCellCoord);
                    }
                }
                coordinate goalCellCoord = canGoCoord[RPG::GetRandomNumber(0, (int) canGoCoord.size())];
                std::vector<Direction> path2Cell;
                if (pathToPoint(path2Cell, currWild->get_position(), goalCellCoord)) {
                    for (auto whereStep: path2Cell) {
                        if (currWild->get_params().current_time <= 0) break;
                        else level->step_by_unit(currWild, whereStep);

                        RPG::draw(window, texture, text, *level);
                        std::this_thread::sleep_for(std::chrono::milliseconds(timeWait));
                    }
                }
            }
        }
    } while (currTime - currWild->get_params().current_time > 0 && currWild->get_params().current_time > 0);
}

void Game::foragerTurn(Forager* currForager, sf::RenderWindow& window, sf::Texture& texture, sf::Text& text) {
    int timeWait = 300;
    currForager->update_time();
    RPG::GenerateTables();
    int currTimeBeforeTurn;
    do {
        currTimeBeforeTurn = currForager->get_params().current_time;
        level->updateChests();
        std::vector<coordinate> chests = level->getChestsCoord();
        if (!chests.empty() && currForager->get_weight() < currForager->get_strength() * 0.5) {
            std::vector<std::vector<Direction>> path2Chests;
            for (int i = 0; i < chests.size(); ++i) {
                path2Chests.emplace_back();
                pathToPoint(path2Chests[i], currForager->get_position(), chests[i]);
            }
            std::sort(path2Chests.begin(), path2Chests.end(),
                      [](std::vector<Direction> &first, std::vector<Direction> &second) {
                          return first.size() < second.size();
                      }
            );

            int i = 0, currTime, chestsNum;
            while (i < path2Chests.size() && path2Chests[i].empty()) {
                ++i;
            }
            if (i < path2Chests.size()) {
                do {
                    currTime = currForager->get_params().current_time;
                    chestsNum = (int) chests.size();
                    std::vector<Direction> path = path2Chests[i];
                    for (auto &stepDirection: path) {
                        level->step_by_unit(currForager, stepDirection);
                        RPG::draw(window, texture, text, *level);
                        std::this_thread::sleep_for(std::chrono::milliseconds(timeWait));
                        if (currForager->get_params().current_time <= 0) break;
                    }
                    int res;
                    do {
                        res = currForager->take_item_to_inventory(level->map_);
                    } while (res == 0);
                    std::this_thread::sleep_for(std::chrono::milliseconds(timeWait));
                    ++i;
                } while ((currTime - currForager->get_params().current_time > 0) &&
                         currForager->get_params().current_time > 0
                         && (chestsNum - chests.size() > 0) && (i < path2Chests.size()));
            }
            if (level->map_[currForager->get_position()]->get_type() == Storage_point) {
                int res;
                do {
                    res = currForager->take_item_to_inventory(level->map_);
                } while (res == 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(timeWait));
            }

            if (currForager->get_params().current_time <= 0) break;
        }
        else if (currForager->get_weight() < currForager->get_strength() * 0.5) {
            // find money
            RPG::coordinate c;
            std::set<coordinate> visibleCells;
            int x = currForager->get_position().first, y = currForager->get_position().second;
            int height = level->get_size().first, width = level->get_size().second;
            for (int i = 0; i < RPG::numOfRays; ++i) {
                c = RPG::CastRay(y, height - x - 1
                        , currForager->get_params().view_radius
                        , i * RPG::graduate
                        , height, width);

                visibleCells = RPG::TileOnMap::LoS(level->map_
                        , visibleCells
                        , y, height - x - 1
                        , c.first, c.second
                        , height);
            }

            int timeBeforeActions = currForager->get_params().current_time;
            for (auto& currCellCoord: visibleCells) {
                if (currForager->get_params().current_time <= 0
                    || timeBeforeActions != currForager->get_params().current_time) break;

                else if (level->map_[currCellCoord]->get_type() == Have_item && currCellCoord != level->getStorageCoord()) {
                    std::vector<Direction> path2Cell;
                    if (pathToPoint(path2Cell, currForager->get_position(), currCellCoord)) {
                        for (auto& whereStep: path2Cell) {
                            if (currForager->get_params().current_time <= 0) break;
                            level->step_by_unit(currForager, whereStep);
                            RPG::draw(window, texture, text, *level);
                            std::this_thread::sleep_for(std::chrono::milliseconds( timeWait));
                        }
                        while (currForager->take_item_to_inventory(level->map_) == 0);
                    }
                }
            }
            // go to random point
            if (timeBeforeActions == currForager->get_params().current_time) {
                if (currForager->get_params().current_time <= 0) break;
                std::vector<coordinate> canGoCoord;
                for (auto currCellCoord: visibleCells) {
                    if (level->map_[currCellCoord]->can_go_through()) {
                        canGoCoord.push_back(currCellCoord);
                    }
                }
                coordinate goalCellCoord = canGoCoord[RPG::GetRandomNumber(0, (int) canGoCoord.size())];
                std::vector<Direction> path2Cell;
                if (pathToPoint(path2Cell, currForager->get_position(), goalCellCoord)) {
                    for (auto whereStep: path2Cell) {
                        if (currForager->get_params().current_time <= 0) break;
                        else level->step_by_unit(currForager, whereStep);
                        RPG::draw(window, texture, text, *level);
                        std::this_thread::sleep_for(std::chrono::milliseconds(timeWait));
                    }
                }
            }
        }
        else {
            std::vector<Direction> path2Storage;
            pathToPoint(path2Storage, currForager->get_position(), level->getStorageCoord());
            for (auto &stepDirection: path2Storage) {
                level->step_by_unit(currForager, stepDirection);
                RPG::draw(window, texture, text, *level);
                std::this_thread::sleep_for(std::chrono::milliseconds( timeWait));
                if (currForager->get_params().current_time <= 0) break;
            }
            if (currForager->get_position() == level->getStorageCoord()) {
                currForager->put_all_items_from_inventory(level->map_);
            }
        }

    } while ((currTimeBeforeTurn - currForager->get_params().current_time > 0) &&
            currForager->get_params().current_time > 0);
}

void Game::rationalTurn(Rational* currRational, sf::RenderWindow& window, sf::Texture& texture, sf::Text& text) {
    int timeWait = 300;
    currRational->update_time();
    RPG::GenerateTables();
    RPG::coordinate c;
    std::set<coordinate> visibleCells;
    int x, y;
    int height = level->get_size().first, width = level->get_size().second;

    std::vector<coordinate> operativesPos;
    for (auto& operative : level->operatives) {
        operativesPos.push_back((*operative).get_position());
    }
    int flagDie, currTime;
    bool flagOperAround;

    do {
        currTime = currRational->get_params().current_time;
        RPG::draw(window, texture, text, *level);
        if (currRational->get_current_weapon() != nullptr && currRational->get_current_weapon()->get_ammo_num() > 0) {
            flagOperAround = true;
            while (flagOperAround) {
                flagDie = 0;  // operator is live
                for (auto operPos: operativesPos) {
                    coordinate currPos = currRational->get_position();
                    coordinate diffCoor = operPos - currPos;
                    int distance = coord2Dir(diffCoor).second;
                    coordinate diff = coord2Dir(diffCoor).first;
                    // if operative around rational - attack
                    if ((distance > 0 && distance <= currRational->get_range())
                    && (diff == RPG_Object::DIR_POS[Left] || diff == RPG_Object::DIR_POS[Down]
                        || diff == RPG_Object::DIR_POS[Up] || diff == RPG_Object::DIR_POS[Right])) {

                        while (currRational->get_params().current_time > 0
                        && currRational->get_current_weapon()->get_ammo_num() > 0
                        && flagDie != 100) {
                            flagDie = level->shoot(currRational, RPG_Object::POS_DIR[diff]);
                            RPG::draw(window, texture, text, *level);
                            std::this_thread::sleep_for(std::chrono::milliseconds( timeWait));
                        }
                        if (flagDie == 100) {
                            operativesPos.erase(std::find(operativesPos.begin(), operativesPos.end(), operPos));
                            break;
                        }
                    }
                }
                if (level->check_flag() > 0) break;
                flagOperAround = false;

                if (currRational->get_params().current_time <= 0 || currRational->get_current_weapon()->get_ammo_num() == 0) break;
                for (auto operPos: operativesPos) {
                    // if rational can see operative - go to him
                    if (isSeeUnit(currRational->get_position(), operPos)) {
                        std::vector<Direction> path2Oper;
                        if (pathToPoint(path2Oper, currRational->get_position(), operPos)) {
                            for (auto whereStep: path2Oper) {
                                if (currRational->get_params().current_time <= 0) break;
                                coordinate currPos = currRational->get_position();
                                coordinate diffCoor = operPos - currPos;
                                int distance = coord2Dir(diffCoor).second;
                                coordinate diff = coord2Dir(diffCoor).first;

                                if (diff == RPG_Object::DIR_POS[whereStep]
                                && (distance > 0 && distance <= currRational->get_range())
                                && isSeeUnit(currPos, operPos)) {
                                    level->shoot(currRational, whereStep);
                                    flagOperAround = true;
                                }
                                else level->step_by_unit(currRational, whereStep);

                                RPG::draw(window, texture, text, *level);
                                std::this_thread::sleep_for(std::chrono::milliseconds( timeWait));
                            }
                        }
                    }
                    if (flagOperAround) break;
                }
            }
            if (currRational->get_params().current_time <= 0 || level->check_flag() > 0) break;
        }
        if (currRational->get_current_weapon() != nullptr && currRational->get_current_weapon()->get_ammo_num() == 0) {
            currRational->put_current_weapon(level->map_);
        }
        if (currRational->get_current_weapon() == nullptr) {

            x = currRational->get_position().first;
            y = currRational->get_position().second;
            for (int i = 0; i < RPG::numOfRays; ++i) {
                c = RPG::CastRay(y, height - x - 1, currRational->get_params().view_radius, i * RPG::graduate, height,
                                 width);

                visibleCells = RPG::TileOnMap::LoS(level->map_, visibleCells, y, height - x - 1, c.first, c.second,
                                                   height);
            }
            for (auto& currCellCoord: visibleCells) {
                if (currRational->get_params().current_time <= 0) break;
                else if (level->map_[currCellCoord]->get_type() == Have_item
                || level->map_[level->getStorageCoord()]->get_type() == Storage_point) {
                    std::vector<Direction> path2Cell;
                    if (pathToPoint(path2Cell, currRational->get_position(), currCellCoord)) {
                        for (auto& whereStep: path2Cell) {
                            if (currRational->get_params().current_time <= 0) break;
                            level->step_by_unit(currRational, whereStep);
                            RPG::draw(window, texture, text, *level);
                            std::this_thread::sleep_for(std::chrono::milliseconds( timeWait));
                        }
                        currRational->take_weapon(level->map_);
                    }
                }
            }
            if (currRational->get_current_weapon() == nullptr
            && level->map_[level->getStorageCoord()]->get_type() == Have_item) {
                std::vector<Direction> path2Storage;
                if (pathToPoint(path2Storage, currRational->get_position(), level->getStorageCoord())) {
                    for (auto& whereStep: path2Storage) {
                        if (currRational->get_params().current_time <= 0) break;
                        level->step_by_unit(currRational, whereStep);
                        RPG::draw(window, texture, text, *level);
                        std::this_thread::sleep_for(std::chrono::milliseconds( timeWait));
                    }
                    currRational->take_weapon(level->map_);
                }
            }
        }

        x = currRational->get_position().first;
        y = currRational->get_position().second;
        for (int i = 0; i < RPG::numOfRays; ++i) {
            c = RPG::CastRay(y, height - x - 1, currRational->get_params().view_radius, i * RPG::graduate, height,
                             width);

            visibleCells = RPG::TileOnMap::LoS(level->map_, visibleCells, y, height - x - 1, c.first, c.second,
                                               height);
        }
        // go to random point
        std::vector<coordinate> canGoCoord;
        for (auto currCellCoord: visibleCells) {
            if (level->map_[currCellCoord]->can_go_through()) {
                canGoCoord.push_back(currCellCoord);
            }
        }
        coordinate goalCellCoord = canGoCoord[RPG::GetRandomNumber(0, (int) canGoCoord.size())];
        std::vector<Direction> path2Cell;
        if (pathToPoint(path2Cell, currRational->get_position(), goalCellCoord)) {
            for (auto whereStep: path2Cell) {
                if (currRational->get_params().current_time <= 0) break;
                else level->step_by_unit(currRational, whereStep);

                RPG::draw(window, texture, text, *level);
                std::this_thread::sleep_for(std::chrono::milliseconds(timeWait));
            }
        }
    } while (currTime - currRational->get_params().current_time > 0 && currRational->get_params().current_time > 0);
}

bool Game::pathToPoint(std::vector<Direction> &path, coordinate from, coordinate to) {
    coordinate size = level->get_size();
    const int WALL = -1;
    const int BLANK = -2;
    std::vector<std::vector<int>> grid;
    std::vector<coordinate> displacement = {RPG_Object::DIR_POS[Left], RPG_Object::DIR_POS[Down],
                                            RPG_Object::DIR_POS[Up], RPG_Object::DIR_POS[Right]};
    bool stop;

    //grid.reserve(size.first);
    for (int i = 0; i < size.first; ++i) {
        grid.emplace_back();
        for (int j = 0; j < size.second; ++j) {
            coordinate x_y = std::make_pair(i, j);
            grid[i].push_back(level->map_.at(x_y)->can_go_through() ? BLANK : WALL);
            if (!level->map_[std::make_pair(i, j)]->can_go_through() && std::make_pair(i, j) == to) {
                grid[i][j] = BLANK;
            }
            //grid[i][j] = level->map_.at(x_y)->can_go_through() ? BLANK : WALL;
        }
    }
    for (auto currEnemy: level->enemies) {
        coordinate currPos = currEnemy->get_position();
        if (currPos != from && currPos != to) {
            grid[currPos.first][currPos.second] = WALL;
        }
    }
    for (auto currOper: level->operatives) {
        coordinate currPos = currOper->get_position();
        if (currPos != from && currPos != to) {
            grid[currPos.first][currPos.second] = WALL;
        }
    }

    // распространение волны
    int d = 0;
    grid[from.first][from.second] = 0; // стартовая ячейка помечена 0
    do {
        stop = true;               // предполагаем, что все свободные клетки уже помечены
        for (int i = 0; i < size.first; ++i) {
            for (int j = 0; j < size.second; ++j) {
                if (grid[i][j] == d) { // если построили путь до текущей ячейки
                    for (int k = 0; k < 4; ++k) { // проходим по всем непомеченным соседям
                        int x = i + displacement[k].first, y = j + displacement[k].second;

                        if (x >= 0 && x < size.first && y >= 0 && y < size.second && grid[x][y] == BLANK) {
                            stop = false; // найдены непомеченные клетки
                            grid[x][y] = d + 1; // распространяем волну
                        }
                    }
                }
            }
        }
        ++d;
    } while (!stop && grid[to.first][to.second] == BLANK);

    if (grid[to.first][to.second] == BLANK) return false; // путь не найден

    // восстанавливаем путь
    const int len = grid[to.first][to.second];
    coordinate currPos = to;
    d = len;
    std::vector<coordinate> coorPath;
    for (int i = 0; i < len + 1; ++i) {
        coorPath.emplace_back();
    }
    while (d > 0) {
        coorPath[d] = currPos;
        --d;
        for (int k = 0; k < 4; ++k) {
            coordinate probPos = currPos + displacement[k];

            if (probPos.first >= 0 && probPos.first < size.first
                && probPos.second >= 0 && probPos.second < size.second
                && grid[probPos.first][probPos.second] == d) {
                currPos = probPos;
                break;
            }
        }
    }

    coorPath[0] = from;

    // заполнение пути для персонажа
    for (int i = 0; i < len; ++i) {
        if (coorPath[i] + RPG_Object::DIR_POS[Left] == coorPath[i + 1]) {
            path.push_back(Left);
        } else if (coorPath[i] + RPG_Object::DIR_POS[Down] == coorPath[i + 1]) {
            path.push_back(Down);
        } else if (coorPath[i] + RPG_Object::DIR_POS[Up] == coorPath[i + 1]) {
            path.push_back(Up);
        } else if (coorPath[i] + RPG_Object::DIR_POS[Right] == coorPath[i + 1]) {
            path.push_back(Right);
        }
    }

    return true;
}

bool Game::isSeeUnit(RPG::coordinate unitCoorFrom, RPG::coordinate coorTo) {
    int t, x, y, absX, absY, sX, sY, dx, dy;

    dx = unitCoorFrom.first - coorTo.first;
    dy = unitCoorFrom.second - coorTo.second;

    if (sqrt(pow(dx, 2) + pow(dy, 2)) > level->enemies[0]->get_params().view_radius) return false;

    // ax & ay: absolute values dx & dy divided by 2
    absX = abs(dx) / 2;
    absY = abs(dy) / 2;

    // sX & sY: signum of dx & dy
    sX = RPG::Sgn(dx);
    sY = RPG::Sgn(dy);

    x = coorTo.first;
    y = coorTo.second;

    if (absX > absY) {

        t = absY - (absX / 2);
        do {
            if (t >= 0) {

                y += sY;
                t -= absX;
            }

            x += sX;
            t += absY;

            if (x == unitCoorFrom.first && y == unitCoorFrom.second) return true;

        } while (level->map_[std::make_pair(x, y)]->is_visible());

        return false;
    } else {

        t = absX - (absY / 2);
        do {
            if (t >= 0) {
                x += sX;
                t -= absY;
            }
            y += sY;
            t += absX;
            if (x == unitCoorFrom.first && y == unitCoorFrom.second) return true;

        } while (level->map_[std::make_pair(x, y)]->is_visible());
    }
    return false;
}

int Game::startMenu(sf::Texture &texture, sf::Text &text) {
    int resMenu;
    resMenu = RPG::TileOnMap::drawMessage(texture, RPG::tile_size, "operation", text);

    return resMenu;
}

int Game::helpMenu(sf::Texture& texture, sf::Text& text, bool pause) {
    coordinate size = std::make_pair(10, 14);
    sf::RenderWindow window(sf::VideoMode(size.second * RPG::tile_size.y * RPG::scale
                                    , size.first * RPG::tile_size.x * RPG::scale)
            , "Help menu");

    int num = 0, res = 0;
    sf::Keyboard::Key choice;
    do {
        for (int i = 0; i < size.first; ++i) {
            RPG::TileOnMap::drawFullRow(window, i, size.second, std::make_pair(0, 0), texture);
        }
    //    std::vector<std::function<void(sf::RenderWindow&, sf::Texture&, sf::Text&, coordinate)>> helpMenuFunctions =
    //            {manageMenu, charactersMenu, objectsMenu};
        if (num == 0) {
            manageMenu(window, texture, text, size);
        }
        else if (num == 1) {
            charactersMenu(window, texture, text, size);
        }
        else if (num == 2) {
            objectsMenu(window, texture, text, size);
        }

        sf::Text comment;
        comment.setFont(*text.getFont());
        comment.setCharacterSize(RPG::font_size + 4);
        std::ostringstream status;

        if (pause) {
            status << "--- A\t\t\t\t\tPress enter or escape or tilde to continue\t\t\t\t\tD ---" << std::endl;
        }
        else {
            status << "--- A\t\t\t\t\t\t\t\tPress enter to continue\t\t\t\t\t\t\t\tD ---" << std::endl;
        }
        comment.setString(status.str());
        comment.setPosition(tile_size.x * RPG::scale, (float) ((float) size.first - 0.5) * tile_size.y * RPG::scale);
        comment.setFillColor(sf::Color::Green);
        window.draw(comment);

        window.display();
        choice = RPG::get_input(window);
        if (sf::Keyboard::A == choice) {
            num = (num > 0) ? num - 1 : 2;
        }
        else if (sf::Keyboard::D == choice) {
            num = (num + 1) % 3;
        }
    } while (choice != sf::Keyboard::Enter && choice != sf::Keyboard::Escape && choice != sf::Keyboard::Tilde);
    window.close();
    if (choice == sf::Keyboard::Enter) res = 1;
    return res;
}

void Game::manageMenu(sf::RenderWindow& window, sf::Texture& texture, sf::Text& text, coordinate size) {

    RPG::TileOnMap::drawStringNumbers(window, texture, std::make_pair(0, 1), "game control");
    RPG::TileOnMap::drawStringNumbers(window, texture, std::make_pair(1, 4), "fight");
    RPG::TileOnMap::drawStringNumbers(window, texture, std::make_pair((size.first - 3), 1), "inventory");

    sf::Text ruleFight, ruleInventory;
    ruleFight.setFont(*text.getFont());
    ruleFight.setCharacterSize(RPG::font_size + 5);
    std::ostringstream statusFight, statusInventory;
    statusFight << "\tA, W, S, D - left, up, down, right\n\n"
                << "\tG - take item\n\n"
                << "\tF - shoot (before that you need choice direction)\n\n"
                << "\tR - reload\n\n"
                << "\tQ - heal (choice the biggest medicine kit)\n\n"
                << "\t1, 2, .., 9 - choice active operative\n\n"
                << "\tI - open inventory mode\n\n"
                << "\tE - change turn\n\n"
                << "\tEscape - open help menu, "
                << "\tTilde - quit from game" << std::endl;
    ruleFight.setString(statusFight.str());
    ruleFight.setPosition(0, 2 * tile_size.y * RPG::scale);
    window.draw(ruleFight);

    ruleInventory.setFont(*text.getFont());
    ruleInventory.setCharacterSize(RPG::font_size + 4);
    statusInventory << "\tA, D - left, right\n\n"
                    << "\tEscape or Tilde - quit" << std::endl;
    ruleInventory.setString(statusInventory.str());
    ruleInventory.setPosition(0, (size.first - 2) * tile_size.y * RPG::scale);
    window.draw(ruleInventory);

}

void Game::charactersMenu(sf::RenderWindow& window, sf::Texture& texture, sf::Text& text, coordinate size) {

    RPG::TileOnMap::drawStringNumbers(window, texture, std::make_pair(0, 2), "characters");
    std::vector<sf::Sprite> charactersSprites;
    sf::Sprite operative(texture), wild(texture), forager(texture), rational(texture);
    operative.setTextureRect({unit_tile_coords.at(OPERATIVE).first
                              , unit_tile_coords.at(OPERATIVE).second
                              , tile_size.x, tile_size.y});
    wild.setTextureRect({unit_tile_coords.at(WILD).first
                              , unit_tile_coords.at(WILD).second
                              , tile_size.x, tile_size.y});
    forager.setTextureRect({unit_tile_coords.at(FORAGER).first
                              , unit_tile_coords.at(FORAGER).second
                              , tile_size.x, tile_size.y});
    rational.setTextureRect({unit_tile_coords.at(RATIONAL).first
                              , unit_tile_coords.at(RATIONAL).second
                              , tile_size.x, tile_size.y});
    charactersSprites.push_back(operative), charactersSprites.push_back(wild);
    charactersSprites.push_back(forager), charactersSprites.push_back(rational);

    sf::Text comment1, comment2, comment3, comment4;
    std::vector<sf::Text> comments = {comment1, comment2, comment3, comment4};
    std::ostringstream describe1, describe2, describe3, describe4;

    describe1 << "\nOperative - your character." << std::endl;
    comments[0].setString(describe1.str());
    describe2 << "\nWild - basic enemy.\nCan attack in close distance." << std::endl;
    comments[1].setString(describe2.str());
    describe3 << "\nForager - searches for and steals items,\nafter which he takes them to the base." << std::endl;
    comments[2].setString(describe3.str());
    describe4 << "\nRational - an enemy that can only attack with a weapon,\nbut cannot reload it." << std::endl;
    comments[3].setString(describe4.str());

    for (int i = 2; i < size.first; i += 2) {
        charactersSprites[(i - 2) / 2].setPosition(0, i * tile_size.y * scale);
        charactersSprites[(i - 2) / 2].setScale(scale, scale);
        comments[(i - 2) / 2].setFont(*text.getFont());
        comments[(i - 2) / 2].setCharacterSize(RPG::font_size + 4);
        comments[(i - 2) / 2].setPosition(2 * tile_size.x * scale, i * tile_size.y * scale);
        window.draw(charactersSprites[(i - 2) / 2]);
        window.draw(comments[(i - 2) / 2]);
    }

}

void Game::objectsMenu(sf::RenderWindow& window, sf::Texture& texture, sf::Text& text, coordinate size) {
    RPG::TileOnMap::drawStringNumbers(window, texture, std::make_pair(0, 3), "objects");
    std::vector<sf::Sprite> objectsSprites;
    std::vector<sf::Text> comments;

//    int i = 2;
    for (int j = 0; j < cell_tile_coords.size(); ++j) {
        objectsSprites.emplace_back();
        comments.emplace_back();
    }

    for (auto& cell: cell_tile_coords) {
        sf::Sprite cellSprite(texture);
        cellSprite.setTextureRect({cell.second.first, cell.second.second
                                   , tile_size.x, tile_size.y});
        cellSprite.setPosition(0, ((int) cell.first + 2) * tile_size.y * scale);
        cellSprite.setScale(scale, scale);
        objectsSprites[(int) cell.first] = cellSprite;

        sf::Text comment;
        comment.setFont(*text.getFont());
        comment.setCharacterSize(RPG::font_size + 4);
        comment.setPosition(2 * tile_size.x * scale, ((int) cell.first + 2) * tile_size.y * scale);
        comments[(int) cell.first] = comment;
//        ++i;
    }

    std::ostringstream describeFloor, describeWall, describeGlass, describePartition, describeStorage, describeHave;

    describeFloor << "\nThis is floor." << std::endl;
    comments[0].setString(describeFloor.str());
    describeWall << "\nWall - you can't see through it and can't destroy it." << std::endl;
    comments[1].setString(describeWall.str());
    describeGlass << "\nGlass - you can see through it and can destroy it,\nbut can't go through." << std::endl;
    comments[2].setString(describeGlass.str());
    describePartition << "\nPartition - you can't see through it,\nbut can destroy it and can't go through." << std::endl;
    comments[3].setString(describePartition.str());
    describeStorage << "\nChest - inside is a weapon, ammo and a medicine kit.\nDisappears if you take everything." << std::endl;
    comments[4].setString(describeStorage.str());
    describeHave << "\nMoney - a cell that contains a valuable item." << std::endl;
    comments[5].setString(describeHave.str());

    for (int i = 0; i < objectsSprites.size(); ++i) {
        window.draw(objectsSprites[i]);
        window.draw(comments[i]);
    }
}

int Game::saveMenu(std::string message, sf::Texture& texture, sf::Text& text) {
    coordinate size = std::make_pair(6, 10);
    int res = 0;
    sf::RenderWindow window(sf::VideoMode(size.second * RPG::tile_size.y * RPG::scale
                                    , size.first * RPG::tile_size.x * RPG::scale)
            , "Configuration level");

    for (int i = 0; i < size.first; ++i) {
        RPG::TileOnMap::drawFullRow(window, i, size.second, std::make_pair(0, 0), texture);
    }

    sf::Text mainText(text), yesText(text), noText(text), helpText(text);
    std::ostringstream status, yesStr, noStr, helpStr;
    status << message << std::endl;
    mainText.setString(status.str());
    mainText.setPosition((size.second / 4) * tile_size.y * scale,
                     (size.first / 4) * tile_size.x * scale);
    mainText.setCharacterSize(12);
    window.draw(mainText);

    yesStr << "Yes" << std::endl;
    yesText.setString(yesStr.str());
    yesText.setPosition(RPG::tile_size.x * RPG::scale
            , (size.first - 2) * RPG::tile_size.y * RPG::scale);
    noStr << "No" << std::endl;
    noText.setString(noStr.str());
    noText.setPosition((size.second - 1) * RPG::tile_size.x * RPG::scale
            , (size.first - 2) * RPG::tile_size.y * RPG::scale);
    helpStr << "Enter 'enter' to confirm" << std::endl;
    helpText.setString(helpStr.str());
    helpText.setPosition(((size.second - 3) / 2) * RPG::tile_size.x * RPG::scale
                         , (size.first - 1) * RPG::tile_size.y * RPG::scale);
    helpText.setFillColor(sf::Color::Yellow);
    window.draw(helpText);

    sf::Keyboard::Key choice;
    do {
        if (res == 0) {
            yesText.setFillColor(sf::Color::Green);
            noText.setFillColor(sf::Color::White);
        }
        else {
            yesText.setFillColor(sf::Color::White);
            noText.setFillColor(sf::Color::Green);
        }
        window.draw(yesText);
        window.draw(noText);
        window.display();
        choice = RPG::get_input(window);
        if (choice == sf::Keyboard::A || choice == sf::Keyboard::D) {
            res ^= 1;
        }
    } while(choice != sf::Keyboard::Enter);

    window.close();
    return res;
}

int Game::chooseLevel(sf::Texture& texture, sf::Text& text) {

    coordinate size = std::make_pair(RPG::levels.size() * 2 + 4, 14);
    sf::RenderWindow window(sf::VideoMode(size.second * RPG::tile_size.y * RPG::scale
                                    , size.first * RPG::tile_size.x * RPG::scale)
            , "Level menu");

    for (int i = 0; i < size.first; ++i) {
        RPG::TileOnMap::drawFullRow(window, i, size.second, std::make_pair(0, 0), texture);
    }

    RPG::TileOnMap::drawStringNumbers(window, texture, std::make_pair(0, 1), "choose level");

    for (int i = 0; i < levels.size(); ++i) {
        RPG::TileOnMap::drawStringNumbers(window
                                          , texture
                                          , std::make_pair((i + 1) * 2, 0)
                                          , RPG::levels[i]);
    }

    sf::Text comment;
    comment.setFont(*text.getFont());
    comment.setCharacterSize(RPG::font_size + 5);
    std::ostringstream status;

    status << "Enter level number" << std::endl;

    comment.setString(status.str());
    comment.setPosition(4 * tile_size.x * RPG::scale, (size.first - 1) * tile_size.y * RPG::scale);
    comment.setFillColor(sf::Color::Green);
    window.draw(comment);

    window.display();

    sf::Keyboard::Key choice;
    int res = -1;
    do {
        choice = RPG::get_input(window);

    } while ((sf::Keyboard::Num0 > choice || choice > (levels.size() + sf::Keyboard::Num0 - 1))
    && (sf::Keyboard::Numpad0 > choice || choice > (levels.size() + sf::Keyboard::Numpad0 - 1))
    && choice != sf::Keyboard::Escape
    && choice != sf::Keyboard::Tilde);

    window.close();
    if ((sf::Keyboard::Num0 <= choice && choice <= levels.size() + sf::Keyboard::Num0 - 1)
    || (sf::Keyboard::Numpad0 <= choice && choice <= levels.size() + sf::Keyboard::Numpad0 - 1)) {
        res = (choice >= sf::Keyboard::Numpad0) ? choice - sf::Keyboard::Numpad0 : choice - sf::Keyboard::Num0;
    }
    return res;
}

Game::~Game() {
    delete level;
}

} // RPG
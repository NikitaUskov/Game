#include <iostream>
#include <fstream>
#include "graphics/TileOnMap.h"

using namespace sf;
using namespace std;


int main() {
    sf::Keyboard::Key choice;
    int res_turn_oper = 0;
    int res;

    sf::Texture texture;
    texture.loadFromFile(RPG::tileset_path);
    sf::Text text;
    sf::Font font;
    font.loadFromFile(RPG::font_path);
    text.setFont(font);
    text.setCharacterSize(RPG::font_size);

    if (RPG::Game::startMenu(texture, text) != 1) return 0;
    if (RPG::Game::helpMenu(texture, text, false) != 1) return 0;

    std::string nums = "0123456789";
    do {
        res = 0;
        std::ifstream fin(RPG::save_path + "map" + ".txt");
        int save = 1;
        if (fin.is_open()) {
            fin.close();
            save = RPG::Game::saveMenu("You have saved level. Would you like load it?", texture, text);
        }
        auto *level = new RPG::Level();
        if (save == 0) {
            level->load(RPG::save_path);
        }
        else {
            int menuRes = RPG::Game::chooseLevel(texture, text);
            if (menuRes == -1) return 0;
            level->start(RPG::level_path + nums[menuRes] + ".txt");
        }
        RPG::Game game = RPG::Game(level);

        sf::RenderWindow window(sf::VideoMode(level->get_size().second * RPG::tile_size.y * RPG::scale,
                                              level->get_size().first * RPG::tile_size.x * RPG::scale),
                                RPG::window_title);
        int diff = 0, resInventory;

        do {
            try {
                //sf::RenderWindow window(sf::VideoMode(level.get_size().second * tile_size.y * scale, level.get_size().first * tile_size.x * scale), window_title);
                while (window.isOpen()) {
                    while (res_turn_oper != -100 && window.isOpen()) {
                        RPG::draw(window, texture, text, *level, diff);
                        choice = RPG::get_input(window);
                        //~ - exit to main menu
                        if (choice == sf::Keyboard::Tilde) {
                            int saveLevel;
                            saveLevel = RPG::Game::saveMenu("Save this level? (other save will be delete)", texture, text);
                            if (saveLevel == 0) level->save(RPG::save_path);
                            window.close();
                            break;
                        } else if (choice == sf::Keyboard::Escape) {
                            RPG::Game::helpMenu(texture, text);
                        }
                        // Game - turn
                        res_turn_oper = game.turn_operatives(choice, window, &diff);
                        if (res_turn_oper == 77) {
                            resInventory = 0;
                            while (resInventory == 0) {
                                resInventory = game.inventoryActions(level->operatives[diff], texture, text);
                            }
                        }
                    }
                    if (choice == sf::Keyboard::Tilde) break;
                    game.turn_enemies(window, texture, text);
                    if (res_turn_oper == -100) {
                        diff = 0;
                        for (auto &curr_oper: level->operatives) {
                            curr_oper->update_time();
                        }
                        res_turn_oper = 0;
                    }
                    if (level->check_flag()) {
                        window.close();
                        break;
                    }
                }
            } catch (exception &error) {
                std::cout << "\n\t\tUnexpected error... " << error.what() << std::endl;
                window.close();
                break;
            }
        } while (!level->check_flag() && choice != sf::Keyboard::Tilde);
        if (level->check_flag() > 0) {

            if (level->check_flag() == 1) {
                res = RPG::TileOnMap::drawMessage(texture, RPG::tile_size, "you win", text);
            } else if (level->check_flag() == 2) {
                res = RPG::TileOnMap::drawMessage(texture, RPG::tile_size, "you lose", text);
            }
        }
    } while (res == 1);
    return 0;
}
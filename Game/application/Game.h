#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Level.h"
#include "../graphics/TileOnMap.h"
#include "../graphics/TileCoordinates.h"
#include <cstring>
#include <thread>
#include <chrono>

namespace RPG {

/**
 * @class Game - manager of game
 */
class Game {
public:
    /**
     * @brief constructor of Game
     * @param number_of_level (int) - number of level
     * @param level0 (Level&) - start level
     */
    explicit Game(Level* level0); //initialisation of Game

    /**
     * @brief turn of operatives - reactions of clicks
     * @param choice what player is click
     * @param window sf::window, in which player can click second choice (in shoot case)
     * @param diff what operative has turn
     * @return flag after change (if we want to finish turn)
     */
    int turn_operatives(sf::Keyboard::Key choice, sf::RenderWindow& window, int* diff);

    /**
     * @brief turn of all enemies
     * @param window window for draw map
     * @param texture texture for draw map
     * @param text text for draw map
     * @return flag - result of turn
     */
    int turn_enemies(sf::RenderWindow& window, sf::Texture& texture, sf::Text& text);

    /**
     * @brief create inventory mode
     * @param currOperative ptr to active operative
     * @param texture textures of items
     * @param text parameters of text
     * @return flag of mode (finish flag of mode)
     */
    int inventoryActions(Operative* currOperative, sf::Texture& texture, sf::Text& text);

    /**
     * @brief create start menu
     * @param texture textures of items
     * @param text parameters of text
     * @return flag of result
     */
    static int startMenu(sf::Texture& texture, sf::Text& text);

    /**
     * @brief create help menu (with rules)
     * @param texture textures of items
     * @param text parameters of text
     * @return flag of result
     */
    static int helpMenu(sf::Texture& texture, sf::Text& text, bool pause = true);

    /**
     * @brief create menu with level choice
     * @param texture textures of items
     * @param text parameters of text
     * @return flag of result
     */
    static int chooseLevel(sf::Texture& texture, sf::Text& text);
    static int saveMenu(std::string message, sf::Texture& texture, sf::Text& text);
    /**
     * @brief free pointer of level
     */
    ~Game();

protected:
    /**
     * (Level) - start level
     */
    Level* level;

    /**
     * @brief calculate path to point with algorithm lee
     * @param path vector, which will have resulting directions
     * @param from point from we start
     * @param to point where we go
     * @return true or false - Is exist the path?
     */
    bool pathToPoint(std::vector<Direction>& path, coordinate from, coordinate to);

    /**
     * @brief check situation: can unit see to point
     * @param unitCoorFrom coordinate from unit see
     * @param CoorTo coordinate where unit try see
     * @return can unit see to point
     */
    bool isSeeUnit(coordinate unitCoorFrom, coordinate CoorTo);

    /**
     * @brief turn of all enemies with type == wild
     * @param currWild current wild
     * @param window window for draw map
     * @param texture texture for draw map
     * @param text text for draw map
     */
    void wildTurn(Wild* currWild, sf::RenderWindow& window, sf::Texture& texture, sf::Text& text);

    /**
     * @brief turn of all enemies with type == forager
     * @param currForager current forager
     * @param window window for draw map
     * @param texture texture for draw map
     * @param text text for draw map
     */
    void foragerTurn(Forager* currForager, sf::RenderWindow& window, sf::Texture& texture, sf::Text& text);

    /**
     * @brief turn of all enemies with type == rational
     * @param currRational current rational
     * @param window window for draw map
     * @param texture texture for draw map
     * @param text text for draw map
     */
    void rationalTurn(Rational* currRational, sf::RenderWindow& window, sf::Texture& texture, sf::Text& text);

    /**
     * @brief print all parameters of item
     * @param currItem ptr to needs item
     * @param window active window
     * @param texture texture of items
     * @param text parameters of text
     * @param shift shift of row for print
     * @param printOptions need to print options
     */
    void itemActions(Item* currItem
                     , sf::RenderWindow& window
                     , sf::Texture& texture
                     , sf::Text& text
                     , int shift
                     , bool printOptions = true);

    static void manageMenu(sf::RenderWindow& window, sf::Texture& texture, sf::Text& text, coordinate size);
    static void charactersMenu(sf::RenderWindow& window, sf::Texture& texture, sf::Text& text, coordinate size);
    static void objectsMenu(sf::RenderWindow& window, sf::Texture& texture, sf::Text& text, coordinate size);

};

/**
 * @brief get tap of keyboard
 * @param window active window
 * @return tapped key
 */
static sf::Keyboard::Key get_input(sf::RenderWindow& window) {
    sf::Event event{};
    do {
        window.pollEvent(event);
        if (event.type == sf::Event::Closed) {
            return sf::Keyboard::Tilde;
        }
        if (event.type == sf::Event::KeyPressed) {
            return event.key.code;
        }
    } while (true);
}

} // RPG

#endif //GAME_H

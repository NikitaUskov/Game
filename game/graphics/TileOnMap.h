#ifndef OOP_SEM5_TILEONMAP_H
#define OOP_SEM5_TILEONMAP_H

#include "TileCoordinates.h"
#include "../application/Game.h"
#include <sstream>
#include <set>

namespace RPG {

//static const std::string root_path = "/mnt/c/Users/danii";
static const std::string root_path = "/home/usik-dan";
//static const std::string level_path = "/ClionProjects/OOP_sem5/lab4/src/maps/level3.txt";
static const std::string level_path = "src/maps/level";
static const std::string save_path = "src/maps/savedLevel/";
//static const std::string tileset_path = "/ClionProjects/OOP_sem5/lab4/src/textures/tileset.png";
static const std::string tileset_path = "src/textures/tileset.png";
//static const std::string font_path = "/ClionProjects/OOP_sem5/lab4/src/textures/solo5.ttf";
static const std::string font_path = "src/textures/solo5.ttf";
static const std::string window_title = "Operation";
static const std::vector<std::string> levels = {"0:train", "1:easy", "2:normal", "3:hard"};
static const uint32_t font_size = 9;
static const float scale = 4.f;
static const sf::Vector2i tile_size(16, 16);

class TileOnMap {
public:
    TileOnMap(const sf::Texture &textures, const sf::Text &text, const sf::Vector2i &tile_size, float scale = 4.f);

    void load(RPG::Level &level, int currOperInd = -1);

    void drawCells(sf::RenderWindow &window) const;

    void drawUnits(sf::RenderWindow &window) const;

    void drawTexts(sf::RenderWindow &window) const;

    static int drawMessage(const sf::Texture &textures
                     , const sf::Vector2i &tile_size
                     , const std::string& message
                     , sf::Text& text
                     , float scale = 4.f);

    static void drawInventory(sf::RenderWindow &window, std::vector<sf::Sprite>& itemSprites);

    static void drawFullRow(sf::RenderWindow &window
                            , int shift
                            , int sizeTable
                            , coordinate tileCoord
                            , sf::Texture& texture);

    static void drawStringNumbers(sf::RenderWindow& window
                            , sf::Texture& texture
                            , coordinate startPoint
                            , std::string message);

    static std::set<RPG::coordinate>&
    LoS(RPG::Map &map_, std::set<RPG::coordinate> &visible_cells, int x1, int y1, int x2, int y2, int height);

private:
    float scale;

    const sf::Texture &textures;
    const sf::Text &text_;
    std::vector<sf::Sprite> sprite_map;
    std::vector<sf::Sprite> sprite_units;
    std::vector<sf::Text> texts;
    sf::Vector2i tile_size;
    std::vector<sf::Vertex> vertices;

};

static void draw(sf::RenderWindow& window, sf::Texture& texture, sf::Text& text, RPG::Level& level, int currOperInd = -1) {
    RPG::TileOnMap tileMap(texture, text, tile_size, scale);
    tileMap.load(level, currOperInd);
    window.clear(sf::Color::Black);
    tileMap.drawCells(window);
    tileMap.drawUnits(window);
    tileMap.drawTexts(window);
    window.display();
}

} // RPG
#endif //OOP_SEM5_TILEONMAP_H

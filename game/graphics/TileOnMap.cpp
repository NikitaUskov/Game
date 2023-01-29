#include "TileOnMap.h"
#include "help_func.h"


namespace RPG {

std::set<RPG::coordinate>& TileOnMap::LoS(RPG::Map &map_
                                          , std::set<RPG::coordinate> &visible_cells
                                          , int x1
                                          , int y1
                                          , int x2
                                          , int y2
                                          , int height) {
    int dx, dy, sdx, sdy, dx_abs, dy_abs, x, y, px, py;

    dx = x2 - x1;
    dy = y2 - y1;
    dx_abs = abs(dx);
    dy_abs = abs(dy);
    sdx = RPG::Sgn(dx);
    sdy = RPG::Sgn(dy);
    x = dy_abs / 2;
    y = dx_abs / 2;
    px = x1;
    py = y1;
    if (dx_abs >= dy_abs) {
        for (int i = 0; i < dx_abs; ++i) {
            y += dy_abs;
            if (y >= dx_abs) {
                y -= dx_abs;
                py += sdy;
            }
            px += sdx;
            visible_cells.insert(std::make_pair(height - py - 1, px)); // добавляем в список отрисовки
            //если здесь стена или плитка, остановка...
            if (!map_[std::make_pair(height - py - 1, px)]->is_visible()) {
                return visible_cells;
            }
        }
    } else {
        for (int i = 0; i < dy_abs; ++i) {
            x += dx_abs;
            if (x > dy_abs) {
                x -= dy_abs;
                px += sdx;
            }
            py += sdy;
            visible_cells.insert(std::make_pair(height - py - 1, px)); //рисуем тайл на экране
            //если здесь стена или плитка, остановка...
            if (!map_[std::make_pair(height - py - 1, px)]->is_visible()) {
                return visible_cells;
            }
        }
    }
    return visible_cells;
}

TileOnMap::TileOnMap(const sf::Texture &textures, const sf::Text &text, const sf::Vector2i &tile_size, float scale)
        :
        textures(textures),
        text_(text),
        tile_size(tile_size),
        scale(scale) {}

void TileOnMap::load(RPG::Level &level, int currOperInd) {
    RPG::GenerateTables();
    RPG::Map &map_ = level.map_;
    auto operatives = level.operatives;
    auto enemies = level.enemies;

    const int height = level.get_size().first;
    const int width = level.get_size().second;

    RPG::coordinate c;
    std::set<RPG::coordinate> draw_cell;
    for (auto &oper: operatives) {
        int x = oper->get_position().first;
        int y = oper->get_position().second;
        for (int i = 0; i < RPG::numOfRays; ++i) {
            c = RPG::CastRay(y, height - x - 1, oper->get_params().view_radius, i * RPG::graduate, height, width);
            draw_cell = LoS(map_, draw_cell, y, height - x - 1, c.first, c.second, height);
        }
    }
    for (auto &cell_coord: draw_cell) {
        sf::Sprite cell_sprite = sf::Sprite(textures);
        RPG::coordinate sprite_coord = cell_tile_coords.at(map_[cell_coord]->get_type());
        cell_sprite.setTextureRect({sprite_coord.first, sprite_coord.second, tile_size.x, tile_size.y});
        cell_sprite.setPosition(cell_coord.second * tile_size.y * scale, cell_coord.first * tile_size.x * scale);
        cell_sprite.setScale(scale, scale);
        sprite_map.push_back(cell_sprite);

        //vertices[cell_coord.first * width + cell_coord.second] = cell_sprite;
    }

    RPG::coordinate sprite_coord;
    if (currOperInd >= (int) operatives.size()) {
        currOperInd -= (currOperInd - (int) operatives.size() + 1);
    }
    int i = 1;
    for (auto &oper: operatives) {
        sf::Text text(text_);
        std::ostringstream status;
        status << "HP: " << oper->get_params().current_health << " / " << oper->get_params().max_health << "\n"
               << "Time: " << oper->get_params().current_time << " / " << oper->get_params().max_time << "\n"
               << "Ammo: " << oper->get_current_weapon()->get_ammo_num() << " / "
               << oper->get_current_weapon()->get_params().bas_params.max_ammo
               << "\n\n\t\t  " << i << std::endl;
        text.setString(status.str());
        text.setPosition(oper->get_position().second * tile_size.y * scale,
                         oper->get_position().first * tile_size.x * scale);
        text.setFillColor(sf::Color::White);

        if (currOperInd != -1 && operatives[currOperInd] == oper) {
            text.setFillColor(sf::Color::Red);
        }
        texts.push_back(text);
        sf::Sprite oper_sprite = sf::Sprite(textures);
        sprite_coord = unit_tile_coords.at(RPG::OPERATIVE);
        oper_sprite.setTextureRect({sprite_coord.first, sprite_coord.second, tile_size.x, tile_size.y});
        oper_sprite.setPosition(oper->get_position().second * tile_size.y * scale,
                                oper->get_position().first * tile_size.x * scale);
        oper_sprite.setScale(scale, scale);
        sprite_units.push_back(oper_sprite);
        ++i;
    }

    for (auto &enemy: enemies) {
        bool flag_see = false;
        for (auto &cell_coord: draw_cell) {
            if (cell_coord == enemy->get_position()) {
                flag_see = true;
                break;
            }
        }
        if (!flag_see) {
            continue;
        }

        sf::Sprite enemy_sprite(textures);
        sf::Text text(text_);
        std::ostringstream status;
        status << "HP: " << enemy->get_params().current_health << " / " << enemy->get_params().max_health;
        text.setString(status.str());
        text.setPosition(enemy->get_position().second * tile_size.y * scale,
                         enemy->get_position().first * tile_size.x * scale);
        texts.push_back(text);

        sprite_coord = unit_tile_coords.at(enemy->get_type());
        enemy_sprite.setTextureRect({sprite_coord.first,
                                     sprite_coord.second,
                                     tile_size.x,
                                     tile_size.y});
        enemy_sprite.setPosition(enemy->get_position().second * tile_size.y * scale,
                                 enemy->get_position().first * tile_size.x * scale);
        enemy_sprite.setScale(scale, scale);
        sprite_units.push_back(enemy_sprite);
    }

}

void TileOnMap::drawCells(sf::RenderWindow &window) const {
    for (auto &cell_sprite: sprite_map) {
        window.draw(cell_sprite);
    }
}

void TileOnMap::drawUnits(sf::RenderWindow &window) const {
    for (const auto &unit: sprite_units) {
        window.draw(unit);
    }
}

void TileOnMap::drawTexts(sf::RenderWindow &window) const {
    for (const auto &message: texts)
        window.draw(message);
}

int TileOnMap::drawMessage(const sf::Texture &textures
                            , const sf::Vector2i &tile_size
                            , const std::string& message
                            , sf::Text& text
                            , float scale) {

    std::vector<sf::Sprite> charSprites;

    coordinate size = std::make_pair(9, message.size() + 2);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 13; ++j) {
            int x = (tile_size.x + 1) * (19 + j);
            int y = (tile_size.x + 1) * (30 + i);
            sf::Sprite charSprite = sf::Sprite(textures);
            charSprite.setTextureRect({x, y, tile_size.x, tile_size.y});
            charSprite.setScale(scale, scale);

            charSprites.push_back(charSprite);
        }
    }


    sf::RenderWindow window(sf::VideoMode(size.second * (unsigned int) ((float) tile_size.y * scale)
                                          , size.first * (unsigned int) ((float) tile_size.x * scale))
                            , RPG::window_title);

    for (int i = 0; i < size.first; ++i) {
        for (int j = 0; j < size.second; ++j) {
            int x = i, y = j;
            sf::Sprite sprite = sf::Sprite(textures);
            sprite.setTextureRect({0, 0, tile_size.x, tile_size.y});
            sprite.setPosition(y * tile_size.y * scale, x * tile_size.x * scale);
            sprite.setScale(scale, scale);
            window.draw(sprite);
        }
    }

    int i = 0;
    for (auto char_ : message) {
        ++i;
        if (char_ != ' ') {
            sf::Sprite sprite = charSprites[(int) (char_ - 'a')];
            sprite.setPosition(i * tile_size.y * scale, (size.first / 2) * tile_size.x * scale);
            window.draw(sprite);
        }
    }
    sf::Text comment;
    comment.setFont(*text.getFont());
    comment.setCharacterSize(RPG::font_size + 5);
    std::ostringstream status;
    status << "Press enter to continue" << std::endl;
    comment.setString(status.str());
    comment.setPosition(((message.size() - 5) / 2 + 1.5) * tile_size.x * RPG::scale
                        , (size.first - 1) * tile_size.y * RPG::scale);
    window.draw(comment);

    window.display();
    sf::Keyboard::Key choice;
    int res = 0;
    do {
        choice = RPG::get_input(window);

    } while (choice != sf::Keyboard::Enter && choice != sf::Keyboard::Escape && choice != sf::Keyboard::Tilde);
    window.close();
    if (choice == sf::Keyboard::Enter) res = 1;
    return res;
}

void TileOnMap::drawInventory(sf::RenderWindow &window, std::vector<sf::Sprite>& itemSprites) {
    for (auto& currItem : itemSprites) {
        window.draw(currItem);
    }
}

void TileOnMap::drawStringNumbers(sf::RenderWindow &window, sf::Texture& texture
                                  , coordinate startPoint, std::string message) {

    std::vector<sf::Sprite> charSprites;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 13; ++j) {
            int x = (RPG::tile_size.x + 1) * (19 + j);
            int y = (RPG::tile_size.x + 1) * (30 + i);
            sf::Sprite charSprite = sf::Sprite(texture);
            charSprite.setTextureRect({x, y, RPG::tile_size.x, RPG::tile_size.y});
            charSprite.setScale(RPG::scale, RPG::scale);
            charSprites.push_back(charSprite);
        }
    }

    std::vector<sf::Sprite> numSprites;
    for (int j = 0; j < 11; ++j) {
        int x = (RPG::tile_size.x + 1) * (19 + j);
        int y = (RPG::tile_size.y + 1) * (29);
        sf::Sprite numSprite = sf::Sprite(texture);
        numSprite.setTextureRect({x, y, RPG::tile_size.x, RPG::tile_size.y});
        numSprite.setScale(RPG::scale, RPG::scale);
        numSprites.push_back(numSprite);
    }

    int i = startPoint.second;
    for (auto char_ : message) {
        if ('a' <= char_ and char_ <= 'z') {
            sf::Sprite sprite = charSprites[(int) (char_ - 'a')];
            sprite.setPosition(i * RPG::tile_size.y * RPG::scale, startPoint.first * RPG::tile_size.x * RPG::scale);
            window.draw(sprite);
        }
        else if ('0' <= char_ and char_ <= '9') {
            sf::Sprite sprite = numSprites[(int) (char_ - '0')];
            sprite.setPosition(i * RPG::tile_size.y * RPG::scale, startPoint.first * RPG::tile_size.x * RPG::scale);
            window.draw(sprite);
        }
        else if (char_ == ':') {
            sf::Sprite sprite = numSprites[10];
            sprite.setPosition(i * RPG::tile_size.y * RPG::scale, startPoint.first * RPG::tile_size.x * RPG::scale);
            window.draw(sprite);
        }
        ++i;
    }
}

void TileOnMap::drawFullRow(sf::RenderWindow &window, int shift, int sizeTable, coordinate tileCoord, sf::Texture& texture) {
    for (int i = 0; i < sizeTable; ++i) {
        sf::Sprite sprite = sf::Sprite(texture);
        sprite.setTextureRect({tileCoord.first, tileCoord.second
                               , RPG::tile_size.x, RPG::tile_size.y});
        sprite.setPosition(i * RPG::tile_size.x * RPG::scale, shift * RPG::tile_size.y * RPG::scale);
        sprite.setScale(RPG::scale, RPG::scale);
        window.draw(sprite);
    }
}

} // RPG
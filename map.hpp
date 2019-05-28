#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Map
{
    sf::Sprite sprite;
    std::string id;
    std::string name;
    sf::Vector2i screen_size;
    sf::Vector2u screen_center;
    sf::Uint8 p_per_tile;
    sf::Uint8 p_correction;
    sf::Uint8 hero_x;
    sf::Uint8 hero_y;
    sf::Uint8 size_x;
    sf::Uint8 size_y;

public:
    void set_id(const std::string& value);
    void set_name(const std::string& value);
    void set_screen_size(const sf::Vector2u size);
    void set_hero_xy(const sf::Uint8 x, const sf::Uint8 y);
    void set_map_size(const sf::Uint8 x, const sf::Uint8 y);
    void set_texture(const sf::Texture& texture);

    void draw(sf::RenderWindow& window);
    void center_map();
};

#endif // MAP_HPP_INCLUDED

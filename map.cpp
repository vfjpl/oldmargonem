#include "map.hpp"
#include <SFML/Graphics/Texture.hpp>

void Map::set_id(const std::string& value)
{
    id = value;
}

void Map::set_name(const std::string& value)
{
    name = value;
}

void Map::set_screen_size(const sf::Vector2u size)
{
    screen_size = sf::Vector2i(size.x, size.y);
    screen_center = size/2U;
}

void Map::set_hero_xy(const sf::Uint8 x, const sf::Uint8 y)
{
    hero_x = x;
    hero_y = y;
}

void Map::set_map_size(const sf::Uint8 x, const sf::Uint8 y)
{
    size_x = x;
    size_y = y;
}

void Map::set_texture(const sf::Texture& texture)
{
    sprite.setTexture(texture);
    sf::Vector2u texture_size = texture.getSize();
    p_per_tile = (texture_size.x + texture_size.y) / (size_x + size_y);
    p_correction = p_per_tile/2U;

    center_map();
}

void Map::center_map()
{
    sf::Vector2i where((hero_x*p_per_tile) + (p_correction) - (screen_center.x),
                       (hero_y*p_per_tile) + (p_correction) - (screen_center.y));
    sprite.setTextureRect(sf::IntRect(where, screen_size));
}

void Map::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

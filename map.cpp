#include "map.hpp"
#include <SFML/Graphics/Texture.hpp>

#define SPEED 4

void Map::set_id(const std::string& value)
{
    id = value;
}

void Map::set_name(const std::string& value)
{
    name = value;
}

void Map::set_screen_size(sf::Vector2u value)
{
    map_rect.width = value.x;
    map_rect.height = value.y;
    screen_size = sf::Vector2i(value.x, value.y);
    screen_center = sf::Vector2i(value.x/2, value.y/2);
}

void Map::set_hero_pos(sf::Vector2i value)
{
    hero_pos_diff = value - hero_pos;
    hero_old_pos = hero_pos;
    hero_pos = value;
    move_fraction = 0;
}

void Map::set_map_size(sf::Vector2i size)
{
    map_size = size;
}

void Map::set_texture(const sf::Texture& texture)
{
    sprite.setTexture(texture);
    sf::Vector2u texture_size = texture.getSize();
    p_per_tile = (texture_size.x + texture_size.y);
    p_per_tile /= (map_size.x + map_size.y);
    p_correction = p_per_tile/2;

    map_rect.left = (hero_pos.x*p_per_tile) + (p_correction) - (screen_center.x);
    map_rect.top = (hero_pos.y*p_per_tile) + (p_correction) - (screen_center.y);
    sprite.setTextureRect(map_rect);
}

void Map::draw(sf::RenderWindow& window, sf::Time time)
{
    move_fraction += time.asSeconds();
    if(move_fraction < ((float)1/SPEED))
    {
        map_rect.left = (hero_old_pos.x * p_per_tile) + (p_correction) - (screen_center.x)
                        + (hero_pos_diff.x * p_per_tile * move_fraction * SPEED);
        map_rect.top = (hero_old_pos.y * p_per_tile) + (p_correction) - (screen_center.y)
                       + (hero_pos_diff.y * p_per_tile * move_fraction * SPEED);
    }
    else
    {
        map_rect.left = (hero_pos.x * p_per_tile) + (p_correction) - (screen_center.x);
        map_rect.top = (hero_pos.y * p_per_tile) + (p_correction) - (screen_center.y);
    }

    sprite.setTextureRect(map_rect);
    window.draw(sprite);
}

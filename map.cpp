#include "map.hpp"
#include <SFML/Graphics/Texture.hpp>
#include "config.hpp"

void Map::set_map_id(const std::string& value)
{
    map_id = value;
}

void Map::set_map_name(const std::string& value)
{
    map_name = value;
}

void Map::set_map_pvp(const std::string& value)
{
    map_pvp = value;
}

void Map::set_map_size(sf::Vector2u value)
{
    map_size = value;
}

void Map::set_screen_size(sf::Vector2u value)
{
    map_rect.width = value.x;
    map_rect.height = value.y;
    screen_size = value;
    screen_center = value/2u;
}

void Map::center_to(sf::Vector2i value)
{
    center_pos_diff = value - center_pos;
    center_old_pos = center_pos;
    center_pos = value;
}

void Map::center_rel(sf::Vector2i value)
{
    center_old_pos = center_pos;
    center_pos += value;
    center_pos_diff = center_pos - center_old_pos;
}

void Map::set_texture(const sf::Texture& texture)
{
    map_sprite.setTexture(texture);
    sf::Vector2u texture_size = texture.getSize();
    p_per_tile = (texture_size.x + texture_size.y);
    p_per_tile /= (map_size.x + map_size.y);
    p_correction = p_per_tile/2;

    //position should be now set
    center_view();
}

void Map::draw(sf::RenderWindow& window, sf::Time move_fraction)
{
    if(move_fraction.asSeconds() < 1/MOVEMENT_SPEED)
        center_view_smooth(move_fraction);
    else
        center_view();
    window.draw(map_sprite);
}

void Map::center_view()
{
    map_rect.left = (center_pos.x * p_per_tile) + (p_correction) - (screen_center.x);
    map_rect.top = (center_pos.y * p_per_tile) + (p_correction) - (screen_center.y);
    map_sprite.setTextureRect(map_rect);
}

void Map::center_view_smooth(sf::Time move_fraction)
{
    map_rect.left = (center_old_pos.x * p_per_tile) + (p_correction) - (screen_center.x)
                    + (center_pos_diff.x * p_per_tile * move_fraction.asSeconds() * MOVEMENT_SPEED);
    map_rect.top = (center_old_pos.y * p_per_tile) + (p_correction) - (screen_center.y)
                   + (center_pos_diff.y * p_per_tile * move_fraction.asSeconds() * MOVEMENT_SPEED);
    map_sprite.setTextureRect(map_rect);
}

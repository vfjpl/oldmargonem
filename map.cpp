#include "map.hpp"
#include <SFML/Graphics/Texture.hpp>

#define TILES_PER_SECOND 4

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

void Map::set_center_pos(sf::Vector2i value)
{
    center_pos_diff = value - center_pos;
    center_old_pos = center_pos;
    center_pos = value;
    move_fraction = sf::Time::Zero;
}

void Map::set_texture(const sf::Texture& texture)
{
    map_sprite.setTexture(texture);
    sf::Vector2u texture_size = texture.getSize();
    p_per_tile = (texture_size.x + texture_size.y);
    p_per_tile /= (map_size.x + map_size.y);
    p_correction = p_per_tile/2;

    center_view();
}

void Map::draw(sf::RenderWindow& window, sf::Time time)
{
    move_fraction += time;

    center_view();
    window.draw(map_sprite);
}

void Map::center_view()
{
    map_rect.left = (center_pos.x * p_per_tile) + (p_correction) - (screen_center.x);
    map_rect.top = (center_pos.y * p_per_tile) + (p_correction) - (screen_center.y);
    map_sprite.setTextureRect(map_rect);
}

void Map::center_view_smooth()
{
    map_rect.left = (center_old_pos.x * p_per_tile) + (p_correction) - (screen_center.x)
                    + (center_pos_diff.x * p_per_tile * move_fraction.asSeconds() * TILES_PER_SECOND);
    map_rect.top = (center_old_pos.y * p_per_tile) + (p_correction) - (screen_center.y)
                   + (center_pos_diff.y * p_per_tile * move_fraction.asSeconds() * TILES_PER_SECOND);
    map_sprite.setTextureRect(map_rect);
}

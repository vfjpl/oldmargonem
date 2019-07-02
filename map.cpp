#include "map.hpp"
#include "config.hpp"
#include <SFML/Graphics/Texture.hpp>

void Map::set_map_size(sf::Vector2u value)
{
    map_size = value;
}

void Map::set_screen_size(sf::Vector2u value)
{
    sprite_rect.width = value.x;
    sprite_rect.height = value.y;
    screen_center = sf::Vector2f(value)/2.f;
}

void Map::set_texture(const sf::Texture& texture)
{
    map_sprite.setTexture(texture);
    sf::Vector2u texture_size = texture.getSize();
    p_per_tile = (texture_size.x + texture_size.y);
    p_per_tile /= (map_size.x + map_size.y);
    p_correction = p_per_tile/2;
}

void Map::center_to(sf::Vector2i value)
{
    center_old_pos = center_pos;
    center_pos = value;
    center_pos_diff = center_pos - center_old_pos;
}

void Map::draw(sf::RenderWindow& window, sf::Time move_fraction)
{
    sf::Vector2f temp;
    if(move_fraction.asSeconds() < 1/MOVEMENT_SPEED)
    {
        temp.x = (center_old_pos.x * p_per_tile) + (p_correction) - (screen_center.x)
                 + (center_pos_diff.x * p_per_tile * move_fraction.asSeconds() * MOVEMENT_SPEED);
        temp.y = (center_old_pos.y * p_per_tile) + (p_correction) - (screen_center.y)
                 + (center_pos_diff.y * p_per_tile * move_fraction.asSeconds() * MOVEMENT_SPEED);
    }
    else
    {
        temp.x = (center_pos.x * p_per_tile) + (p_correction) - (screen_center.x);
        temp.y = (center_pos.y * p_per_tile) + (p_correction) - (screen_center.y);
    }
    sprite_rect.left = temp.x;
    sprite_rect.top = temp.y;

    map_sprite.setTextureRect(sprite_rect);
    window.draw(map_sprite);
}

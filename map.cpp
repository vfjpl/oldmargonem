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

void Map::center(sf::Vector2i value)
{
    center_old_pos = center_pos;
    center_pos = value;
    center_pos_diff = center_pos - center_old_pos;
}

void Map::clear()
{
    mutex.lock();

    items.clear();
    NPCs.clear();
    players.clear();

    mutex.unlock();
}

void Map::draw(sf::RenderWindow& window, float move_fraction)
{
    mutex.lock();

    sf::Vector2f map_offset;
    if(move_fraction < 1/MOVEMENT_SPEED)
    {
        map_offset = (sf::Vector2f(center_old_pos) * p_per_tile) - screen_center +
                     (sf::Vector2f(center_pos_diff) * (p_per_tile * move_fraction * MOVEMENT_SPEED));
    }
    else
    {
        map_offset = (sf::Vector2f(center_pos) * p_per_tile) - screen_center;
    }

    sprite_rect.left = (map_offset.x + p_correction);
    sprite_rect.top = (map_offset.y + p_correction);

    map_sprite.setTextureRect(sprite_rect);
    window.draw(map_sprite);

    for(auto &i : items)
        i.second.draw(window, map_offset, p_per_tile);
    for(auto &i : NPCs)
        i.second.draw(window, map_offset, p_per_tile);
    for(auto &i : players)
        i.second.draw(window, map_offset, p_per_tile);

    mutex.unlock();
}

std::string Map::findclose() const
{
    for(auto &i : NPCs)
    {
        sf::Vector2i temp = center_pos - i.second.get_position();
        if(temp.x >= -1 && temp.x <= 1)
            if(temp.y >= -1 && temp.y <= 1)
                return i.first;
    }
    return std::string();
}

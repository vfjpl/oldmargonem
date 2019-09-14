#include "item.hpp"
#include <SFML/Graphics/Texture.hpp>

void Item::set_position(sf::Vector2i value)
{
    item_pos = value;
}

void Item::set_texture(const sf::Texture& texture)
{
    item_sprite.setTexture(texture);
    p_correction = sf::Vector2f(texture.getSize())/2.f;
}

void Item::draw(sf::RenderWindow& window, sf::Vector2f map_offset, float map_tile_size)
{
    item_sprite.setPosition((sf::Vector2f(item_pos) * map_tile_size) - map_offset - p_correction);
    window.draw(item_sprite);
}

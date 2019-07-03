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

void Item::draw(sf::RenderWindow& window, sf::Vector2f map_offset, float p_per_tile)
{
    item_sprite.setPosition((sf::Vector2f(item_pos) * p_per_tile) - p_correction - map_offset);
    window.draw(item_sprite);
}

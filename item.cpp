#include "item.hpp"

void Item::set_position(sf::Vector2i value)
{
    item_pos = value;
}

void Item::set_texture(const sf::Texture& texture)
{
    item_sprite.setTexture(texture);
}

void Item::draw(sf::RenderWindow& window)
{
    window.draw(item_sprite);
}

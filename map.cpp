#include "map.hpp"

void Map::set_texture(const sf::Texture& texture)
{
    sprite.setTexture(texture);
}

void Map::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

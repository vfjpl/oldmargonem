#include "other.hpp"
#include <SFML/Graphics/Texture.hpp>

void Other::set_position(sf::Vector2i value)
{
    other_pos = value;
}

void Other::set_texture(const sf::Texture& texture)
{
    other_sprite.setTexture(texture);
    sf::Vector2u texture_size = texture.getSize();
    sprite_rect.width = texture_size.x/4;
    sprite_rect.height = texture_size.y/4;
    p_per_tile = sf::Vector2f(texture_size)/4.f;
    p_correction = p_per_tile/2.f;
}

void Other::draw(sf::RenderWindow& window, sf::Vector2f map_offset, float map_tile_size)
{
    other_sprite.setPosition((sf::Vector2f(other_pos) * map_tile_size) - map_offset - p_correction);
    window.draw(other_sprite);
}

void Other::set_dir(char value)
{
    sprite_rect.top = (value - '0') * p_per_tile.y;
    other_sprite.setTextureRect(sprite_rect);
}

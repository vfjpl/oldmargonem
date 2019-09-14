#ifndef OTHER_HPP_INCLUDED
#define OTHER_HPP_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Other
{
    sf::Sprite other_sprite;
    sf::IntRect sprite_rect;
    sf::Vector2f p_per_tile;
    sf::Vector2f p_correction;
    sf::Vector2i other_pos;

public:
    void set_position(sf::Vector2i value);
    void set_texture(const sf::Texture& texture);
    void draw(sf::RenderWindow& window, sf::Vector2f map_offset, float map_tile_size);

    void set_dir(char value);
};

#endif // OTHER_HPP_INCLUDED

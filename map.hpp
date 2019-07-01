#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Map
{
    sf::Sprite map_sprite;
    sf::IntRect sprite_rect;
    sf::Vector2u map_size;

    sf::Vector2u screen_center;

    sf::Vector2i center_pos;
    sf::Vector2i center_old_pos;
    sf::Vector2i center_pos_diff;

    float p_per_tile;
    float p_correction;

public:
    void set_map_size(sf::Vector2u value);
    void set_screen_size(sf::Vector2u value);
    void set_texture(const sf::Texture& texture);

    //move maps view to given position
    void center_to(sf::Vector2i value);

    void draw(sf::RenderWindow& window, sf::Time move_fraction);

private:
    void center_view();
    void center_view_smooth(sf::Time move_fraction);
};

#endif // MAP_HPP_INCLUDED

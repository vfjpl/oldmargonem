#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Map
{
    std::string map_name;
    std::string map_id;
    std::string map_pvp;
    sf::Sprite map_sprite;
    sf::IntRect map_rect;
    sf::Vector2u map_size;

    sf::Vector2u screen_size;
    sf::Vector2u screen_center;

    sf::Vector2i center_pos;
    sf::Vector2i center_old_pos;
    sf::Vector2i center_pos_diff;

    sf::Time move_fraction;
    float p_per_tile;
    float p_correction;

public:
    void set_map_id(const std::string& value);
    void set_map_name(const std::string& value);
    void set_map_pvp(const std::string& value);
    void set_map_size(sf::Vector2u value);

    void set_screen_size(sf::Vector2u value);
    void set_center_pos(sf::Vector2i value);
    void set_texture(const sf::Texture& texture);

    void draw(sf::RenderWindow& window, sf::Time time);

private:
    void center_view();
    void center_view_smooth();
};

#endif // MAP_HPP_INCLUDED

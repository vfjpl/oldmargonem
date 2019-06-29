#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Map
{
    sf::Sprite map_sprite;
    sf::IntRect sprite_rect;
    std::string map_id;
    std::string map_name;
    std::string map_pvp;
    sf::Vector2u map_size;

    sf::Vector2u screen_center;

    sf::Vector2i center_pos;
    sf::Vector2i center_old_pos;
    sf::Vector2i center_pos_diff;

    float p_per_tile;
    float p_correction;

public:
    void set_map_id(const std::string& value);
    void set_map_name(const std::string& value);
    void set_map_pvp(const std::string& value);
    void set_map_size(sf::Vector2u value);

    void set_screen_size(sf::Vector2u value);
    void set_texture(const sf::Texture& texture);
    //move maps view to given position
    void center_to(sf::Vector2i value);
    //move maps view by value relative to current position
    void center_rel(sf::Vector2i value);

    void draw(sf::RenderWindow& window, sf::Time move_fraction);

private:
    void center_view();
    void center_view_smooth(sf::Time move_fraction);
};

#endif // MAP_HPP_INCLUDED

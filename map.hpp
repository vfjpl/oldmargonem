#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Map
{
    sf::Sprite sprite;
    std::string id;
    std::string name;

    sf::IntRect map_rect;
    sf::Vector2i screen_size;
    sf::Vector2i screen_center;
    sf::Vector2i map_size;
    sf::Vector2i hero_pos;
    sf::Vector2i hero_old_pos;
    sf::Vector2i hero_pos_diff;
    float p_per_tile;
    float p_correction;
    float move_fraction;

public:
    void set_id(const std::string& value);
    void set_name(const std::string& value);
    void set_screen_size(sf::Vector2u value);
    void set_hero_pos(sf::Vector2i value);
    void set_map_size(sf::Vector2i value);
    void set_texture(const sf::Texture& texture);

    void draw(sf::RenderWindow& window, sf::Time time);
};

#endif // MAP_HPP_INCLUDED

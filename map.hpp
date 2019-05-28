#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Map
{
    sf::Sprite sprite;

public:
    void set_texture(const sf::Texture& texture);
    void draw(sf::RenderWindow& window);
};

#endif // MAP_HPP_INCLUDED

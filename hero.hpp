#ifndef HERO_HPP_INCLUDED
#define HERO_HPP_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Hero
{
    sf::Sprite hero_sprite;
    sf::IntRect sprite_rect;
    sf::Vector2f screen_center;
    sf::Vector2f p_per_tile;
    sf::Vector2f p_correction;
    sf::Vector2i hero_pos;

public:
    void set_screen_size(sf::Vector2u value);
    void set_pos(sf::Vector2i value);
    void set_texture(const sf::Texture& texture);
    void set_dir(char value);

    //move in specified direction
    void move(char dir);
    sf::Vector2i getPosition() const;
    void draw(sf::RenderWindow& window);
};

#endif // HERO_HPP_INCLUDED

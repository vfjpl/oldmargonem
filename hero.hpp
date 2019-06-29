#ifndef HERO_HPP_INCLUDED
#define HERO_HPP_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Hero
{
    sf::Sprite hero_sprite;
    sf::IntRect sprite_rect;
    std::string hero_nick;
    std::string hero_dir;

    sf::Vector2u screen_center;

    sf::Vector2i hero_pos;

    sf::Vector2f p_per_tile;

public:
    void set_screen_size(sf::Vector2u value);
    void set_nick(const std::string& value);
    void set_dir(const std::string& value);
    void set_texture(const sf::Texture& texture);
    void set_pos(sf::Vector2i value);
    void move(sf::Vector2i value);

    sf::Vector2i getPosition() const;

    void draw(sf::RenderWindow& window);
};

#endif // HERO_HPP_INCLUDED

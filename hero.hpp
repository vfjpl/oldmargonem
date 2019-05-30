#ifndef HERO_HPP_INCLUDED
#define HERO_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <string>

class Hero
{
    std::string hero_nick;
    std::string hero_dir;
    sf::Sprite hero_sprite;
    sf::Vector2i hero_pos;

public:
    void set_hero_nick(const std::string& value);
    void set_texture(const sf::Texture& texture);
    void move_to(sf::Vector2i value);
    void move_rel(sf::Vector2i value);

    sf::Vector2i get_position() const;
};

#endif // HERO_HPP_INCLUDED

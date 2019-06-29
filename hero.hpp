#ifndef HERO_HPP_INCLUDED
#define HERO_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <string>

class Hero
{
    sf::Sprite hero_sprite;
    std::string hero_nick;
    std::string hero_dir;
    sf::Vector2i hero_pos;

public:
    void set_nick(const std::string& value);
    void set_dir(const std::string& value);
    void set_texture(const sf::Texture& texture);
    void set_pos(sf::Vector2i value);
    void move(sf::Vector2i value);

    sf::Vector2i getPosition() const;
    std::string getDir() const;
};

#endif // HERO_HPP_INCLUDED

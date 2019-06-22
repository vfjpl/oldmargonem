#include "hero.hpp"

void Hero::set_nick(const std::string& value)
{
    hero_nick = value;
}

void Hero::set_dir(const std::string& value)
{
    hero_dir = value;
}

void Hero::set_texture(const sf::Texture& texture)
{
    hero_sprite.setTexture(texture);
}

void Hero::set_pos(sf::Vector2i value)
{
    hero_pos = value;
}

void Hero::move(sf::Vector2i value)
{
    hero_pos += value;
}

sf::Vector2i Hero::getPosition() const
{
    return hero_pos;
}

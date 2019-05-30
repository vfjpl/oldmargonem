#include "hero.hpp"

void Hero::set_hero_nick(const std::string& value)
{
    hero_nick = value;
}

void Hero::set_texture(const sf::Texture& texture)
{
    hero_sprite.setTexture(texture);
}

void Hero::move_to(sf::Vector2i value)
{
    hero_pos = value;
}

void Hero::move_rel(sf::Vector2i value)
{
    hero_pos += value;
}

sf::Vector2i Hero::get_position() const
{
    return hero_pos;
}

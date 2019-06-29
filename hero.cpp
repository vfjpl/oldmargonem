#include "hero.hpp"
#include <SFML/Graphics/Texture.hpp>

void Hero::set_screen_size(sf::Vector2u value)
{
    sf::Vector2f center(value/2u);
    hero_sprite.setPosition(center);
}

void Hero::set_nick(const std::string& value)
{
    hero_nick = value;
}

void Hero::set_dir(const std::string& value)
{
    hero_dir = value;
    sprite_rect.top = std::stoul(hero_dir) * p_per_tile.y;

    hero_sprite.setTextureRect(sprite_rect);
}

void Hero::set_texture(const sf::Texture& texture)
{
    hero_sprite.setTexture(texture);
    sf::Vector2u texture_size = texture.getSize();
    sprite_rect.width = texture_size.x/4;
    sprite_rect.height = texture_size.y/4;
    p_per_tile.x = texture_size.x/4;
    p_per_tile.y = texture_size.y/4;
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

std::string Hero::getDir() const
{
    return hero_dir;
}

void Hero::draw(sf::RenderWindow& window)
{
    window.draw(hero_sprite);
}

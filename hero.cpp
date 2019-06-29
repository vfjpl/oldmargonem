#include "hero.hpp"
#include <SFML/Graphics/Texture.hpp>

void Hero::set_screen_size(sf::Vector2u value)
{
    screen_center = value/2u;
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

    sf::Vector2f correction(screen_center);
    p_per_tile = sf::Vector2f(texture_size)/4.0f;
    correction -= p_per_tile/2.0f;

    hero_sprite.setPosition(correction);
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

void Hero::draw(sf::RenderWindow& window)
{
    window.draw(hero_sprite);
}

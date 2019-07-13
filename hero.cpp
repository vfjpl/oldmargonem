#include "hero.hpp"
#include <SFML/Graphics/Texture.hpp>

void Hero::set_screen_size(sf::Vector2u value)
{
    screen_center = sf::Vector2f(value)/2.f;
}

void Hero::set_pos(sf::Vector2i value)
{
    hero_pos = value;
}

void Hero::set_texture(const sf::Texture& texture)
{
    hero_sprite.setTexture(texture);
    sf::Vector2u texture_size = texture.getSize();
    sprite_rect.width = texture_size.x/4;
    sprite_rect.height = texture_size.y/4;
    p_per_tile = sf::Vector2f(texture_size)/4.f;
    p_correction = p_per_tile/2.f;

    hero_sprite.setPosition(screen_center - p_correction);
}

void Hero::set_dir(char value)
{
    sprite_rect.top = (value - '0') * p_per_tile.y;
    hero_sprite.setTextureRect(sprite_rect);
}

void Hero::move(char dir)
{
    switch(dir)
    {
    case '0':
        ++hero_pos.y;
        break;
    case '1':
        --hero_pos.x;
        break;
    case '2':
        ++hero_pos.x;
        break;
    case '3':
        --hero_pos.y;
        break;
    }// end switch
    sprite_rect.top = (dir - '0') * p_per_tile.y;
    hero_sprite.setTextureRect(sprite_rect);
}

sf::Vector2i Hero::getPosition() const
{
    return hero_pos;
}

void Hero::draw(sf::RenderWindow& window)
{
    window.draw(hero_sprite);
}

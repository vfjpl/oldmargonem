#ifndef NPC_HPP_INCLUDED
#define NPC_HPP_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

class NPC
{
    sf::Sprite npc_sprite;
    sf::Vector2i npc_pos;

public:
    void set_position(sf::Vector2i value);
    void set_texture(const sf::Texture& texture);

    void draw(sf::RenderWindow& window);
};

#endif // NPC_HPP_INCLUDED

#include "npc.hpp"

void NPC::set_position(sf::Vector2i value)
{
    npc_pos = value;
}

void NPC::set_texture(const sf::Texture& texture)
{
    npc_sprite.setTexture(texture);
}

void NPC::draw(sf::RenderWindow& window)
{
    window.draw(npc_sprite);
}

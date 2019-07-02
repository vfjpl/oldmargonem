#include "npc.hpp"
#include <SFML/Graphics/Texture.hpp>

void NPC::set_position(sf::Vector2i value)
{
    npc_pos = value;
}

void NPC::set_texture(const sf::Texture& texture)
{
    npc_sprite.setTexture(texture);
    p_correction = sf::Vector2f(texture.getSize())/2.f;
}

void NPC::draw(sf::RenderWindow& window, sf::Vector2f map_offset, float p_per_tile)
{
    npc_sprite.setPosition((sf::Vector2f(npc_pos) * p_per_tile) - map_offset - p_correction);
    window.draw(npc_sprite);
}

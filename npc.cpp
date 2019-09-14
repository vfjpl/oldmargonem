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

void NPC::draw(sf::RenderWindow& window, sf::Vector2f map_offset, float map_tile_size)
{
    npc_sprite.setPosition((sf::Vector2f(npc_pos) * map_tile_size) - map_offset - p_correction);
    window.draw(npc_sprite);
}

sf::Vector2i NPC::get_position() const
{
    return npc_pos;
}

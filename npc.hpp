#ifndef NPC_HPP_INCLUDED
#define NPC_HPP_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

class NPC
{
    sf::Sprite npc_sprite;
    sf::Vector2f p_correction;
    sf::Vector2i npc_pos;

public:
    void set_position(sf::Vector2i value);
    void set_texture(const sf::Texture& texture);

    sf::Vector2i get_position() const;
    void draw(sf::RenderWindow& window, sf::Vector2f map_offset, float map_tile_size);
};

#endif // NPC_HPP_INCLUDED

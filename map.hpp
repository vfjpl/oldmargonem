#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "npc.hpp"
#include "item.hpp"
#include <SFML/System/Mutex.hpp>
#include <deque>

class Map
{
public:
    std::deque<NPC> NPCs;
    std::deque<Item> items;

private:
    sf::Sprite map_sprite;
    sf::IntRect sprite_rect;
    sf::Vector2u map_size;

    sf::Vector2f screen_center;
    float p_per_tile;
    float p_correction;

    sf::Vector2i center_pos;
    sf::Vector2i center_old_pos;
    sf::Vector2i center_pos_diff;

    sf::Mutex mutex;
public:
    void set_map_size(sf::Vector2u value);
    void set_screen_size(sf::Vector2u value);
    void set_texture(const sf::Texture& texture);

    //move maps view to given position
    void center_to(sf::Vector2i value);

    void clear();
    void draw(sf::RenderWindow& window, float move_fraction);
};

#endif // MAP_HPP_INCLUDED

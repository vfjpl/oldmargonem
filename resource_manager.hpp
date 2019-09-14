#ifndef RESOURCE_MANAGER_HPP_INCLUDED
#define RESOURCE_MANAGER_HPP_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <map>

enum Graphic
{
    GAME,
    INTERFACE,
    MAP,
    BATTLEPLACE,
    HERO,
    ITEM,
    NPC,
};

class Resource_Manager
{
    std::map<std::string, sf::Texture> storage;
    std::string mpath;
    unsigned long size_in_bytes = 0;

public:
    //set download server address for graphics
    void set_mpath(const std::string& value);

    //give you texture that you can use with sprites
    const sf::Texture& get_texture(const std::string& name) const;

    //download graphic with given name and type
    void load_graphic(const std::string& name, Graphic type);
};

#endif // RESOURCE_MANAGER_HPP_INCLUDED

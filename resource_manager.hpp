#ifndef RESOURCE_MANAGER_HPP_INCLUDED
#define RESOURCE_MANAGER_HPP_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <map>

enum Graphic
{
    HERO,
    MAP,
};

class Resource_Manager
{
    std::map<std::string, sf::Texture> storage;
    std::string mpath;
    long size_in_bytes = 0;

public:
    void set_mpath(const std::string& value);
    const sf::Texture& get_texture(const std::string& name) const;
    void load_graphic(const std::string& name, const Graphic what);
};

#endif // RESOURCE_MANAGER_HPP_INCLUDED

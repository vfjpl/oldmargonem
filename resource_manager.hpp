#ifndef RESOURCE_MANAGER_HPP_INCLUDED
#define RESOURCE_MANAGER_HPP_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <map>

class Resource_Manager
{
    std::map<std::string, sf::Texture> storage;
    std::string mpath;

public:
    void set_mpath(const std::string& value);
    void load_character(const std::string& name);
    void load_map(const std::string& name);
};

#endif // RESOURCE_MANAGER_HPP_INCLUDED

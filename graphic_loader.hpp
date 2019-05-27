#ifndef GRAPHIC_LOADER_HPP_INCLUDED
#define GRAPHIC_LOADER_HPP_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <map>

class Graphic_Loader
{
    std::map<std::string, sf::Texture> storage;
    std::string mpath;

public:
    void set_mpath(const std::string& path);
    void load_image(const std::string& name);
};

#endif // GRAPHIC_LOADER_HPP_INCLUDED

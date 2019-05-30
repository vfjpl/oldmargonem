#include "resource_manager.hpp"
#include <SFML/Network/Http.hpp>
#include <iostream>

void Resource_Manager::set_mpath(const std::string& value)
{
    mpath = value;
}

const sf::Texture& Resource_Manager::get_texture(const std::string& name) const
{
    return storage.at(name);
}

void Resource_Manager::load_graphic(const std::string& name, Graphic what)
{
    if(storage.count(name))
        return;

    std::string path;
    switch(what)
    {
    case MAP:
        path = "obrazki/miasta/";
        break;
    case CHARACTER:
        path = "obrazki/postacie";
        break;
    case ITEM:
        path = "obrazki/itemy";
        break;
    case NPC:
        path = "obrazki/npc";
        break;
    }

    sf::Http http(mpath);
    sf::Http::Request req(path + name);
    sf::Http::Response resp = http.sendRequest(req);


    long lenght = std::stol(resp.getField("Content-Length"));
    size_in_bytes += lenght;
    std::cout << size_in_bytes/1024 << " KiB\n";

    if(!storage[name].loadFromMemory(resp.getBody().data(), lenght))
    {
        //RESIZING
        sf::Image orginal;
        sf::Image resized;
        orginal.loadFromMemory(resp.getBody().data(), lenght);

        float scale = sf::Texture::getMaximumSize();
        sf::Vector2u orginal_size = orginal.getSize();
        sf::Vector2f scale_xy(orginal_size);
        scale_xy /= scale;
        if(scale_xy.x > scale_xy.y)
            scale = scale_xy.x;
        else
            scale = scale_xy.y;

        resized.create(orginal_size.x / scale, orginal_size.y / scale);
        sf::Vector2u resized_size = resized.getSize();

        std::cout << "Resizing by: " << 1/scale << ", New Size: " << resized_size.x << 'x' << resized_size.y << '\n';

        for(unsigned int y = 0; y < resized_size.y; ++y)
        {
            for(unsigned int x = 0; x < resized_size.x; ++x)
            {
                resized.setPixel(x, y, orginal.getPixel(x*scale, y*scale));
            }
        }
        storage[name].loadFromImage(resized);
    }
}

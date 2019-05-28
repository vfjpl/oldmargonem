#include "resource_manager.hpp"
#include <SFML/Network/Http.hpp>
#include <iostream>

Resource_Manager::Resource_Manager()
{
    size_in_bytes = 0;
}

void Resource_Manager::set_mpath(const std::string& value)
{
    mpath = value;
}

const sf::Texture& Resource_Manager::get_texture(const std::string& name) const
{
    return storage.at(name);
}

void Resource_Manager::load_graphic(const std::string& name, const Graphic what)
{
    if(storage.count(name))
        return;

    std::string path;
    switch(what)
    {
    case HERO:
        path = "obrazki/postacie";
        break;
    case MAP:
        path = "obrazki/miasta/";
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
        float scale_x = scale / orginal_size.x;
        float scale_y = scale / orginal_size.y;
        if(scale_x < scale_y)
            scale = scale_x;
        else
            scale = scale_y;

        resized.create(orginal_size.x * scale, orginal_size.y * scale);
        sf::Vector2u resized_size = resized.getSize();

        std::cout << "Resizing by: " << scale << ", New Size: " << resized_size.x << 'x' << resized_size.y << '\n';

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

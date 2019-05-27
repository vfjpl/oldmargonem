#include "graphic_loader.hpp"
#include <SFML/Network/Http.hpp>

void Graphic_Loader::set_mpath(const std::string& path)
{
    mpath = path;
}

void Graphic_Loader::load_image(const std::string& name)
{
    if(storage.count(name))
        return;

    sf::Http http(mpath);
    sf::Http::Request req("obrazki/miasta/" + name);
    sf::Http::Response resp = http.sendRequest(req);

    sf::Texture temp;
    temp.loadFromMemory(resp.getBody().data(), std::stol(resp.getField("Content-Length")));
    storage[name] = temp;
}

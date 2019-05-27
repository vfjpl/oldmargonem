#include "resource_manager.hpp"
#include <SFML/Network/Http.hpp>

void Resource_Manager::set_mpath(const std::string& value)
{
    mpath = value;
}

void Resource_Manager::load_character(const std::string& name)
{
    if(storage.count(name))
        return;

    sf::Http http(mpath);
    sf::Http::Request req("obrazki/postacie" + name);
    sf::Http::Response resp = http.sendRequest(req);

    sf::Texture temp;
    temp.loadFromMemory(resp.getBody().data(), std::stol(resp.getField("Content-Length")));
    storage[name] = temp;
}

void Resource_Manager::load_map(const std::string& name)
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

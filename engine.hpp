#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include "map.hpp"
#include "network.hpp"
#include "resource_manager.hpp"
#include "myclock.hpp"

class Engine
{
    sf::RenderWindow window;
    Map map;
    Network network;
    Resource_Manager resource_manager;
    MyClock clock;
    bool loop = true;

public:
    void main();

private:
    void setup_window(const bool fullscreen);
    void load_game();
    void process_response(const std::string& body);
    void input_handle();
};

#endif // ENGINE_HPP_INCLUDED

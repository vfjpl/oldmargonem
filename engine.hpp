#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include "map.hpp"
#include "network.hpp"
#include "resource_manager.hpp"
#include "myclock.hpp"
#include "mykeyboard.hpp"

class Engine
{
    sf::RenderWindow window;
    Map map;
    Network network;
    Resource_Manager resource_manager;
    MyClock clock;
    MyKeyboard keyboard;
    bool quit = false;

public:
    void main();

private:
    void setup_window(bool fullscreen);
    void load_game();
    void process_response(const std::string& body);
    void input_handle();
    void game_logic();
};

#endif // ENGINE_HPP_INCLUDED

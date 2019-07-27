#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include "gui.hpp"
#include "map.hpp"
#include "network.hpp"
#include "hero.hpp"
#include "resource_manager.hpp"
#include "myclock.hpp"
#include "mykeyboard.hpp"

class Engine
{
    sf::RenderWindow window;
    Gui gui;
    Map map;
    Network network;
    Hero hero;
    Resource_Manager resource_manager;
    MyClock clock;
    MyKeyboard keyboard;

public:
    Engine();
    bool run_game();
    bool run_network();

private:
    void setup_window(bool fullscreen);
    void resize_window(sf::Vector2u screen_size);

    void process_input();
    void game_logic();
    void draw_frame();

    void process_network();
};

#endif // ENGINE_HPP_INCLUDED

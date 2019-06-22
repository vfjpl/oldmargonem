#include "engine.hpp"
#include <SFML/Window/Event.hpp>

namespace
{
int str2int(const std::string& str)
{
    int res = 0;
    for(sf::Uint8 i = 0; i < str.size(); ++i)
        res += str[i];
    return res;
}
constexpr int char2int(const char* str)
{
    int res = 0;
    for(sf::Uint8 i = 0; i < strlen(str); ++i)
        res += str[i];
    return res;
}
}

Engine::Engine()
{
    setup_window(false);
    map.set_screen_size(window.getSize());
    network.login("login", "sha1password");
    network.queueLoadSequence();
}

void Engine::gameThreadFunc()
{
    while(run_game());
}

void Engine::networkThreadFunc()
{
    while(run_network());
}

bool Engine::run_game()
{
    process_input();
    game_logic();
    draw_frame();
    window.display();

    return window.isOpen();
}

bool Engine::run_network()
{
    process_network();

    return window.isOpen();
}

void Engine::setup_window(bool fullscreen)
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    if(fullscreen)
    {
        window.create(mode, "oldmargonem", sf::Style::Fullscreen);
    }
    else
    {
        mode.width = (mode.width*2)/3;
        mode.height = (mode.height*2)/3;
        window.create(mode, "oldmargonem", sf::Style::Close);
    }
    window.setFramerateLimit(30);
    window.setKeyRepeatEnabled(false);
}

void Engine::process_input()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::KeyPressed:
        {
            switch(event.key.code)
            {
            case sf::Keyboard::W:
                keyboard.up = true;
                break;
            case sf::Keyboard::A:
                keyboard.left = true;
                break;
            case sf::Keyboard::S:
                keyboard.down = true;
                break;
            case sf::Keyboard::D:
                keyboard.right = true;
                break;
            case sf::Keyboard::Escape:
                window.close();
                break;
            default:
                break;
            }// end switch
            break;
        }
        case sf::Event::KeyReleased:
        {
            switch(event.key.code)
            {
            case sf::Keyboard::W:
                keyboard.up = false;
                break;
            case sf::Keyboard::A:
                keyboard.left = false;
                break;
            case sf::Keyboard::S:
                keyboard.down = false;
                break;
            case sf::Keyboard::D:
                keyboard.right = false;
                break;
            default:
                break;
            }// end switch
            break;
        }
        case sf::Event::Closed:
        {
            window.close();
            break;
        }
        default:
        {
            break;
        }
        }//end switch
    }
}

void Engine::game_logic()
{
    if(keyboard.anyKey() && clock.moveInterrupt())
    {
        map.center_rel(keyboard.getPosChange());
    }
    clock.update();
}

void Engine::draw_frame()
{
    map.draw(window, clock.getMoveTime());
}

void Engine::process_network()
{

}

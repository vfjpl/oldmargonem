#include "engine.hpp"
#include <SFML/Window/Event.hpp>
#include <iostream>

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
    network.login();
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
    //window.setVerticalSyncEnabled(true);
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
    std::string body = network.sendRequest();
    bool alldata = false;

    for(size_t old_pos = 0;;)
    {
        size_t new_pos = body.find("<eol>\n<eol>", old_pos);
        size_t colon_pos = body.find(':', old_pos);

        std::string cmd = body.substr(old_pos, colon_pos++ - old_pos);

        switch(str2int(cmd))
        {
        case char2int("hero"):
        {
            break;
        }
        case char2int("xy"):
        {
            break;
        }
        case char2int("town"):
        {
            break;
        }
        case char2int("battlemsg"):
        {
            break;
        }
        case char2int("lastevent")://FINISHED
        {
            std::string parm = body.substr(colon_pos, new_pos - colon_pos);
            network.set_ev(parm);
            break;
        }
        case char2int("maxchat")://FINISHED
        {
            std::string parm = body.substr(colon_pos, new_pos - colon_pos);
            network.set_lastch(parm);
            break;
        }
        case char2int("maxcchat")://FINISHED
        {
            std::string parm = body.substr(colon_pos, new_pos - colon_pos);
            network.set_lastcch(parm);
            break;
        }
        case char2int("sqltime")://FINISHED
        {
            break;
        }
        case char2int("end")://FINISHED
        {
            alldata = true;
            break;
        }
        default:
        {
            std::cout << cmd << " NOT IMPLEMENTED\n";
            break;
        }
        }// end switch

        if(new_pos == std::string::npos)
            break;

        old_pos = new_pos + 11;
    }// end for

    if(!alldata)
        std::cout << "INVALID PACKET\n";
}

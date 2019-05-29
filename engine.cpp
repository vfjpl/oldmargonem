#include "engine.hpp"
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <cstring>

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
std::vector<std::string> split(const std::string& parm)
{
    std::vector<std::string> temp;
    for(size_t old_pos = 0;;)
    {
        size_t new_pos = parm.find(';', old_pos);
        temp.push_back(parm.substr(old_pos, new_pos - old_pos));
        if(new_pos == std::string::npos)
            break;
        old_pos = new_pos + 1;
    }
    return temp;
}
std::vector<std::string> splitv(const std::string& parm)
{
    std::vector<std::string> temp;
    for(size_t old_pos = 0;;)
    {
        size_t new_pos = parm.find(';', old_pos);
        std::string key = parm.substr(old_pos, new_pos - old_pos);
        temp.push_back(key.substr(key.find('=') + 1));
        if(new_pos == std::string::npos)
            break;
        old_pos = new_pos + 1;
    }
    return temp;
}
}

void Engine::main()
{
    setup_window(true);
    map.set_screen_size(window.getSize());

    network.login("","");
    load_game();

    while(loop)
    {
        time = clock.restart();
        map.draw(window, time);
        window.display();
        input_handle();
    }

    network.logout();
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
    window.clear();
    window.display();
}

void Engine::load_game()
{
    network.queue_load_sequence();
    for(sf::Uint8 i = 0; i < 4U; ++i)
    {
        network.send_command(clock.getElapsedTime());
        process_response(network.get_response());
    }
}

void Engine::process_response(const std::string& body)
{
    bool alldata = false;
    for(size_t old_pos = 0;;)
    {
        size_t new_pos = body.find("<eol>\n<eol>", old_pos);
        std::string line = body.substr(old_pos, new_pos - old_pos);
        size_t colon = line.find(':');
        std::string cmd = line.substr(0, colon);

        switch(str2int(cmd))
        {
        case char2int("hero"):
        {
            std::vector<std::string> val = splitv(line.substr(colon+1));
            map.set_hero_pos(sf::Vector2i(std::stol(val[0]), std::stol(val[1])));
            network.set_pdir(val[2]);
            resource_manager.set_mpath(val[11]);
            resource_manager.load_graphic(val[10], Graphic::HERO);
            std::cout << cmd << " PARTIALLY IMPLEMENTED\n";
            break;
        }
        case char2int("town"):
        {
            std::vector<std::string> val = split(line.substr(colon+1));
            map.set_map_size(sf::Vector2i(std::stol(val[0]), std::stol(val[1])));
            resource_manager.load_graphic(val[2], Graphic::MAP);
            map.set_texture(resource_manager.get_texture(val[2]));
            map.set_name(val[3]);
            map.set_id(val[6]);
            std::cout << cmd << " PARTIALLY IMPLEMENTED\n";
            break;
        }
        case char2int("lastevent"):
        {
            network.set_ev(line.substr(colon+1));
            break;
        }
        case char2int("maxchat"):
        {
            network.set_lastch(line.substr(colon+1));
            break;
        }
        case char2int("maxcchat"):
        {
            network.set_lastcch(line.substr(colon+1));
            break;
        }
        case char2int("sqltime"):
        {
            sf::sleep(sf::microseconds(std::stof(line.substr(colon+1)) * 1000));
            break;
        }
        case char2int("end"):
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
    {
        std::cout << "INVALID PACKET\n";
    }
}

void Engine::input_handle()
{
    static int x = 1;
    static int y = 1;
    sf::Event event;
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::KeyPressed:
        {
            switch(event.key.code)
            {
            case sf::Keyboard::Up:
            {
                map.set_hero_pos(sf::Vector2i(x, --y));
                break;
            }
            case sf::Keyboard::Down:
            {
                map.set_hero_pos(sf::Vector2i(x, ++y));
                break;
            }
            case sf::Keyboard::Left:
            {
                map.set_hero_pos(sf::Vector2i(--x, y));
                break;
            }
            case sf::Keyboard::Right:
            {
                map.set_hero_pos(sf::Vector2i(++x, y));
                break;
            }
            default:
            {
                loop = false;
                break;
            }
            }
            break;
        }
        case sf::Event::Closed:
        {
            loop = false;
            break;
        }
        case sf::Event::Resized:
        {
            window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            break;
        }
        default:
        {
            break;
        }
        }//end switch
    }
}

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
std::vector<std::string> split(const std::string& parm, char delim)
{
    std::vector<std::string> temp;
    for(size_t old_pos = 0;;)
    {
        size_t new_pos = parm.find(delim, old_pos);
        temp.push_back(parm.substr(old_pos, new_pos - old_pos));
        if(new_pos == std::string::npos)
            break;
        old_pos = new_pos + 1;
    }
    return temp;
}
std::vector<std::string> split2(const std::string& parm, char first, char second)
{
    std::vector<std::string> temp;
    for(size_t old_pos = 0;;)
    {
        old_pos = parm.find(first, old_pos) + 1;
        size_t new_pos = parm.find(second, old_pos);
        temp.push_back(parm.substr(old_pos, new_pos - old_pos));
        if(new_pos == std::string::npos)
            break;
        old_pos = new_pos + 1;
    }
    return temp;
}
}

Engine::Engine()
{
    setup_window(false);
    sf::Vector2u screen_size = window.getSize();
    map.set_screen_size(screen_size);
    hero.set_screen_size(screen_size);
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
    window.setVerticalSyncEnabled(true);
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
                keyboard.dir = '3';
                keyboard.up = true;
                break;
            case sf::Keyboard::A:
                keyboard.dir = '1';
                keyboard.left = true;
                break;
            case sf::Keyboard::S:
                keyboard.dir = '0';
                keyboard.down = true;
                break;
            case sf::Keyboard::D:
                keyboard.dir = '2';
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
        hero.set_dir(keyboard.dir);
        network.set_pdir(keyboard.dir);

        sf::Vector2i pos_dif = keyboard.getPosChange();
        hero.move(pos_dif);
        map.center_rel(pos_dif);
        network.queueMove(hero.getPosition());
    }
    clock.update();
}

void Engine::draw_frame()
{
    map.draw(window, clock.getMoveTime());
    hero.draw(window);
}

void Engine::process_network()
{
    std::string body = network.sendRequest();
    bool alldata = false;

    for(size_t old_pos = 0;;)
    {
        size_t new_pos = body.find("<eol>\n<eol>", old_pos);
        std::string line = body.substr(old_pos, new_pos - old_pos);
        size_t colon = line.find(':');
        std::string cmd = line.substr(0, colon);

        switch(str2int(cmd))
        {
        case char2int("element"):
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split2(parm, '=', ';');

            //p[0] element type
            switch(str2int(p[0]))
            {
            default:
            {
                std::cout << p[0] << " NOT IMPLEMENTED\n";
                break;
            }
            }//end switch
            break;
        }
        case char2int("hero"):
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split2(parm, '=', ';');

            //p[0-1] is hero pos
            sf::Vector2i pos(std::stoi(p[0]), std::stoi(p[1]));
            hero.set_pos(pos);
            map.center_to(pos);
            //p[2] is hero direction

            //p[3] is hero nick
            //p[4] is
            //p[5] is hero level
            //p[6] is hero exp
            //p[7] is
            //p[8] is hero gold
            //p[9] is
            //p[10] is hero graphic

            //p[11] is mpath

            //p[12] is
            //p[13] is
            //p[14] is
            //p[15] is hero profession shortcut
            //p[16] is hero base strenght
            //p[17] is hero base agility
            //p[18] is hero base inteligence
            //p[19] is hero profession name

            resource_manager.set_mpath(p[11]);
            resource_manager.load_graphic(p[10], Graphic::CHARACTER);
            hero.set_texture(resource_manager.get_texture(p[10]));
            hero.set_dir(p[2]);
            network.set_pdir(p[2]);
            break;
        }
        case char2int("town")://FINISHED
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split(parm, ';');

            //p[0-1] is map size
            map.set_map_size(sf::Vector2u(std::stoul(p[0]), std::stoul(p[1])));
            //p[2] is map graphic
            resource_manager.load_graphic(p[2], Graphic::MAP);
            map.set_texture(resource_manager.get_texture(p[2]));
            //p[3] is map name
            map.set_map_name(p[3]);
            //p[4] is map pvp
            map.set_map_pvp(p[4]);
            //p[5] is battle background
            //p[6] is map id
            map.set_map_id(p[6]);
            break;
        }
        case char2int("move")://FINISHED
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split(parm, ',');

            //p[0-1] is hero pos
            sf::Vector2i pos(std::stoi(p[0]), std::stoi(p[1]));
            hero.set_pos(pos);
            map.center_to(pos);
            //p[2] is hero dir
            hero.set_dir(p[2]);
            network.set_pdir(p[2]);
            break;
        }
        case char2int("maxchat")://FINISHED
        {
            std::string parm = line.substr(colon + 1);
            network.set_lastch(parm);
            break;
        }
        case char2int("maxcchat")://FINISHED
        {
            std::string parm = line.substr(colon + 1);
            network.set_lastcch(parm);
            break;
        }
        case char2int("lastevent")://FINISHED
        {
            std::string parm = line.substr(colon + 1);
            network.set_ev(parm);
            break;
        }
        case char2int("xy")://FINISHED
        {
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

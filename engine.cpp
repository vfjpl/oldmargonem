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

    sf::Vector2u screen_size = window.getSize();
    map.set_screen_size(screen_size);
    hero.set_screen_size(screen_size);
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
            case sf::Keyboard::E:
                network.queueEnter();
                break;
            case sf::Keyboard::F:
                network.queueFight(map.findclose());
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
        }// end switch
    }
}

void Engine::game_logic()
{
    if(keyboard.anyKey() && clock.moveInterrupt())
    {
        network.set_pdir(keyboard.dir);
        hero.set_dir(keyboard.dir);
        hero.move(keyboard.getPosChange());

        sf::Vector2i pos = hero.getPosition();
        map.center(pos);
        network.queueMove(pos);
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

    for(size_t old_pos = 0;;)
    {
        size_t new_pos = body.find("<eol>\n<eol>", old_pos);
        std::string line = body.substr(old_pos, new_pos - old_pos);
        size_t colon = line.find(':');
        std::string cmd = line.substr(0, colon);

        switch(str2int(cmd))
        {
        case char2int("msg"):
        {
            std::string parm = line.substr(colon + 1);
            std::cout << parm << '\n';
            break;
        }
        case char2int("chat"):
        {
            std::string parm = line.substr(colon + 1);
            std::cout << parm << '\n';
            break;
        }
        case char2int("element"):
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split2(parm, '=', ';');

            //p[0] element type
            switch(str2int(p[0]))
            {
            case char2int("item"):
                //p[1] is item id

                //p[2] is item name
                //p[3-4] is item position
                map.items[p[1]].set_position(sf::Vector2i(std::stoi(p[3]), std::stoi(p[4])));
                //p[5] is item graphic
                resource_manager.load_graphic(p[5], Graphic::ITEM);
                map.items[p[1]].set_texture(resource_manager.get_texture(p[5]));
                break;
            case char2int("npc"):
                //p[1] is npc id

                //p[2] is npc nick
                //p[3-4] is npc position
                map.NPCs[p[1]].set_position(sf::Vector2i(std::stoi(p[3]), std::stoi(p[4])));
                //p[5] is npc graphic
                resource_manager.load_graphic(p[5], Graphic::NPC);
                map.NPCs[p[1]].set_texture(resource_manager.get_texture(p[5]));
                //p[6] is npc level
                //p[7] is
                //p[8] is
                //p[9] is npc group fight
                //p[10] is npc questmark
                break;
            case char2int("other"):
                //p[1] is other id

                //p[2] is other nick
                //p[3-4] is other position
                map.players[p[1]].set_position(sf::Vector2i(std::stoi(p[3]), std::stoi(p[4])));
                //p[5] is other profesion
                //p[6] is other direction

                //p[7] is other graphic

                //p[8] is
                //p[9] is other level
                //p[10] is other clan tag

                resource_manager.load_graphic(p[7], Graphic::CHARACTER);
                map.players[p[1]].set_texture(resource_manager.get_texture(p[7]));
                map.players[p[1]].set_dir(p[6]);
                break;
            default:
                std::cout << cmd << ' ' << p[0] << " NOT IMPLEMENTED\n";
                break;
            }// end switch
            break;
        }
        case char2int("hero"):
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split2(parm, '=', ';');

            //p[0-1] is hero pos
            sf::Vector2i pos(std::stoi(p[0]), std::stoi(p[1]));
            hero.set_pos(pos);
            map.center(pos);
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
            //p[4] is map pvp
            //p[5] is battle background
            //p[6] is map id
            break;
        }
        case char2int("move")://FINISHED
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split(parm, ',');

            //p[0-1] is hero pos
            sf::Vector2i pos(std::stoi(p[0]), std::stoi(p[1]));
            hero.set_pos(pos);
            map.center(pos);
            //p[2] is hero dir
            hero.set_dir(p[2]);
            network.set_pdir(p[2]);
            break;
        }
        case char2int("reload"):
        {
            map.clear();
            network.queueLoadSequence();
            break;
        }
        case char2int("othermove")://FINISHED
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p1 = split(parm, ',');
            std::vector<std::string> p2 = split2(p1[1], '=', ';');

            //p1[0] is id
            //p2[0] is event
            switch(str2int(p2[0]))
            {
            case char2int("move"):
                //p2[1-2] is id position
                map.players[p1[0]].set_position(sf::Vector2i(std::stoi(p2[1]), std::stoi(p2[2])));
                //p2[3] is id direction
                map.players[p1[0]].set_dir(p2[3]);
                break;
            default:
                std::cout << cmd << ' ' << p2[0] << " NOT IMPLEMENTED\n";
                break;
            }// end switch
            break;
        }
        case char2int("delete")://FINISHED
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split(parm, ',');

            //p[0] is id
            //p[1] is type
            switch(str2int(p[1]))
            {
            case char2int("npc"):
                map.NPCs.erase(p[0]);
                break;
            case char2int("item"):
                map.items.erase(p[0]);
                break;
            case char2int("other"):
                map.players.erase(p[0]);
                break;
            default:
                std::cout << cmd << ' ' << p[1] << " NOT IMPLEMENTED\n";
                break;
            }// end switch
            break;
        }
        case char2int("battleref")://FINISHED
        {
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
}

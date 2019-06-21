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

Engine::Engine()
{
    setup_window(false);
    map.set_screen_size(window.getSize());
    network.login("", "");
    load_game();
}

bool Engine::run()
{
    network_handle();
    input_handle();
    game_logic();
    draw_window();
    window.display();

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
    window.clear();
    window.display();
}

void Engine::load_game()
{
    network.queue_load_sequence();
    network_handle();
    network_handle();
    network_handle();
    network_handle();
}

void Engine::network_handle()
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
            map.center_to(sf::Vector2i(std::stoi(val[0]), std::stoi(val[1])));
            hero.move_to(sf::Vector2i(std::stoi(val[0]), std::stoi(val[1])));
            network.set_pdir(val[2]);//direction the player is facing
            resource_manager.set_mpath(val[11]);
            resource_manager.load_graphic(val[10], Graphic::CHARACTER);
            std::cout << cmd << " PARTIALLY IMPLEMENTED\n";
            break;
        }
        case char2int("town"):
        {
            //FINISHED
            std::vector<std::string> val = split(line.substr(colon+1));
            map.set_map_size(sf::Vector2u(std::stoul(val[0]), std::stoul(val[1])));
            resource_manager.load_graphic(val[2], Graphic::MAP);
            map.set_texture(resource_manager.get_texture(val[2]));
            map.set_map_name(val[3]);
            map.set_map_pvp(val[4]);
            //val[5] is battle background(load elsewhere)
            map.set_map_id(val[6]);
            break;
        }
        case char2int("lastevent"):
        {
            //FINISHED
            network.set_ev(line.substr(colon+1));
            break;
        }
        case char2int("maxchat"):
        {
            //FINISHED
            network.set_lastch(line.substr(colon+1));
            break;
        }
        case char2int("maxcchat"):
        {
            //FINISHED
            network.set_lastcch(line.substr(colon+1));
            break;
        }
        case char2int("battlemsg"):
        {
            std::vector<std::string> val = split(line.substr(colon+1));
            network.set_bseq(val[0]);
            std::cout << cmd << " PARTIALLY IMPLEMENTED\n";
            break;
        }
        case char2int("sqltime"):
        {
            //FINISHED
            break;
        }
        case char2int("end"):
        {
            //FINISHED
            alldata = true;
            break;
        }
        case char2int(""):
        {
            //FINISHED
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

void Engine::game_logic()
{
    if(keyboard.anyKey())
        if(clock.move_interrupt())
        {
            sf::Vector2i temp = keyboard.getPosChange();
            map.center_rel(temp);
            hero.move_rel(temp);
            network.queue_move(hero.get_position());
        }
    clock.update();
}

void Engine::draw_window()
{
    map.draw(window, clock.getMoveTime());
}

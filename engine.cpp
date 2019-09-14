#include "engine.hpp"
#include <TGUI/SignalImpl.hpp>
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
    for(size_t old_pos = parm.find(first);;)
    {
        if(old_pos == std::string::npos)
            break;
        ++old_pos;
        size_t new_pos = parm.find(second, old_pos);
        temp.push_back(parm.substr(old_pos, new_pos - old_pos));
        if(new_pos == std::string::npos)
            break;
        old_pos = parm.find(first, new_pos + 1);
    }
    return temp;
}
}

Engine::Engine()
{
    gui.editbox->connect("Focused", [&] {keyboard.block = true;});
    gui.editbox->connect("Unfocused", [&] {keyboard.block = false;});
    gui.editbox->connect("ReturnKeyPressed", [&](const sf::String& msg)
    {
        network.queueMessage((char*)msg.toUtf8().data());
        gui.editbox->setText(sf::String());
    });

    //temp
    setup_window(true);
    network.login();
    network.queueLoadSequence();
}

bool Engine::run_game()
{
    process_input();
    game_logic();
    draw_frame();

    return window.isOpen();
}

bool Engine::run_network()
{
    network.sendRequest();
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
    window.setKeyRepeatEnabled(false);
    window.clear();
    gui.tgui.setTarget(window);
}

void Engine::resize_window(sf::Vector2u screen_size)
{
    gui.set_screen_size(screen_size);
    sf::Vector2u usable_size = gui.usableScreenSize();
    hero.set_screen_size(usable_size);
    map.set_screen_size(usable_size);
}

void Engine::process_input()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        gui.tgui.handleEvent(event);
        switch(event.type)
        {
        case sf::Event::KeyPressed:
        {
            if(!keyboard.block)
                switch(event.key.code)
                {
                case sf::Keyboard::W:
                    keyboard.dir = '3';
                    keyboard.keys[3] = true;
                    break;
                case sf::Keyboard::A:
                    keyboard.dir = '1';
                    keyboard.keys[1] = true;
                    break;
                case sf::Keyboard::S:
                    keyboard.dir = '0';
                    keyboard.keys[0] = true;
                    break;
                case sf::Keyboard::D:
                    keyboard.dir = '2';
                    keyboard.keys[2] = true;
                    break;
                case sf::Keyboard::F:
                    network.queueFight(map.findclose());
                    break;
                case sf::Keyboard::E:
                    network.queueEnter();
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
                keyboard.keys[3] = false;
                break;
            case sf::Keyboard::A:
                keyboard.keys[1] = false;
                break;
            case sf::Keyboard::S:
                keyboard.keys[0] = false;
                break;
            case sf::Keyboard::D:
                keyboard.keys[2] = false;
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
    }// end while
}

void Engine::game_logic()
{
    if(keyboard.anyKey() && clock.moveInterrupt())
    {
        network.set_pdir(keyboard.dir);
        hero.move(keyboard.dir);

        sf::Vector2i pos = hero.getPosition();
        map.center(pos);
        network.queueMove(pos);
    }
    clock.update();
}

void Engine::draw_frame()
{
    float mt = clock.getMoveTime();
    map.draw(window, mt);
    hero.draw(window, mt);
    gui.draw(window);
    window.display();
}

void Engine::process_network()
{
    std::string body = network.receiveResponse();

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
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split2(parm, '=', ';');

            //p[0-1] is hero pos
            //p[2] is hero direction
            //p[3] is hero nick
            //p[4] is group
            //p[5] is hero level
            //p[6] is hero exp
            //p[7] is abilitypoints
            //p[8] is hero gold
            //p[9] is draconite
            //p[10] is hero graphic
            //p[11] is mpath
            //p[12] is guild
            //p[13] is pvp
            //p[14] is options
            //p[15] is hero profession shortcut
            //p[16] is hero base strength
            //p[17] is hero base agility
            //p[18] is hero base intelligence
            //p[19] is hero profession name
            resource_manager.set_mpath(p[11]);

            //load gui
            resource_manager.load_graphic("panel.png", Graphic::INTERFACE);
            resource_manager.load_graphic(p[10], Graphic::HERO);
            gui.set_right_pannel_texture(resource_manager.get_texture("panel.png"));
            hero.set_texture(resource_manager.get_texture(p[10]));
            resize_window(window.getSize());//we need textures for size

            sf::Vector2i pos(std::stoi(p[0]), std::stoi(p[1]));
            char dir = p[2].front();
            hero.set_pos(pos);
            hero.set_dir(dir);
            map.center(pos);
            network.set_pdir(dir);
            break;
        }
        case char2int("town")://FINISHED
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split(parm, ';');

            //p[0-1] is map size
            //p[2] is map graphic
            //p[3] is map name
            //p[4] is map pvp
            //p[5] is battle background
            //p[6] is map id
            resource_manager.load_graphic(p[2], Graphic::MAP);
            map.set_map_size(sf::Vector2u(std::stoul(p[0]), std::stoul(p[1])));
            map.set_texture(resource_manager.get_texture(p[2]));
            break;
        }
        case char2int("move")://FINISHED
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split(parm, ',');

            //p[0-1] is hero pos
            //p[2] is hero dir
            sf::Vector2i pos(std::stoi(p[0]), std::stoi(p[1]));
            char dir = p[2].front();
            hero.set_pos(pos);
            hero.set_dir(dir);
            map.center(pos);
            network.set_pdir(dir);
            break;
        }
        case char2int("element"):
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split2(parm, '=', ';');

            //p[1] is element id
            unsigned long id = std::stoul(p[1]);
            //p[0] element type
            switch(str2int(p[0]))
            {
            case char2int("item"):
                //p[2] is item name
                //p[3-4] is item position
                //p[5] is item graphic
                resource_manager.load_graphic(p[5], Graphic::ITEM);
                map.items[id].set_position(sf::Vector2i(std::stoi(p[3]), std::stoi(p[4])));
                map.items[id].set_texture(resource_manager.get_texture(p[5]));
                break;
            case char2int("npc"):
                //p[2] is npc nick
                //p[3-4] is npc position
                //p[5] is npc graphic
                //p[6] is npc level
                //p[7] is
                //p[8] is
                //p[9] is npc group fight
                //p[10] is npc questmark
                resource_manager.load_graphic(p[5], Graphic::NPC);
                map.NPCs[id].set_position(sf::Vector2i(std::stoi(p[3]), std::stoi(p[4])));
                map.NPCs[id].set_texture(resource_manager.get_texture(p[5]));
                break;
            case char2int("other"):
                //p[2] is other nick
                //p[3-4] is other position
                //p[5] is other profession
                //p[6] is other direction
                //p[7] is other graphic
                //p[8] is
                //p[9] is other level
                //p[10] is other clan tag
                resource_manager.load_graphic(p[7], Graphic::HERO);
                map.players[id].set_position(sf::Vector2i(std::stoi(p[3]), std::stoi(p[4])));
                map.players[id].set_texture(resource_manager.get_texture(p[7]));
                map.players[id].set_dir(p[6].front());
                break;
            default:
                std::cout << cmd << ' ' << p[0] << " NOT IMPLEMENTED\n";
                break;
            }// end switch
            break;
        }
        case char2int("othermove")://FINISHED
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p1 = split(parm, ',');
            std::vector<std::string> p2 = split2(p1[1], '=', ';');

            //p1[0] is id
            unsigned long id = std::stoul(p1[0]);
            //p2[0] is event
            switch(str2int(p2[0]))
            {
            case char2int("move"):
                //p2[1-2] is id position
                //p2[3] is id direction
                map.players[id].set_position(sf::Vector2i(std::stoi(p2[1]), std::stoi(p2[2])));
                map.players[id].set_dir(p2[3].front());
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
            unsigned long id = std::stoul(p[0]);
            //p[1] is type
            switch(str2int(p[1]))
            {
            case char2int("item"):
                map.items.erase(id);
                break;
            case char2int("npc"):
                map.NPCs.erase(id);
                break;
            case char2int("other"):
                map.players.erase(id);
                break;
            default:
                std::cout << cmd << ' ' << p[1] << " NOT IMPLEMENTED\n";
                break;
            }// end switch
            break;
        }
        case char2int("battlemsg"):
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split(parm, ';');

            network.set_bseq(p[0]);
            break;
        }
        case char2int("battleref"):
        {
            std::string parm = line.substr(colon + 1);
            resource_manager.load_graphic(parm, Graphic::BATTLEPLACE);
            break;
        }
        case char2int("chat")://FINISHED
        {
            std::string parm = line.substr(colon + 1);
            gui.chatbox->addLine(parm);
            break;
        }
        case char2int("alert")://FINISHED
        {
            std::string parm = line.substr(colon +1);
            gui.chatbox->addLine(parm);
            break;
        }
        case char2int("msg")://FINISHED
        {
            std::string parm = line.substr(colon +1);
            gui.chatbox->addLine(parm);
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
        case char2int("reload")://FINISHED
        {
            map.clear();
            gui.chatbox->removeAllLines();
            network.queueLoadSequence();
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

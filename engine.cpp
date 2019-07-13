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
    setup_window(true);
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

void Engine::setup_gui()
{
    if(gui.isSetupCompleted())
        return;

    gui.set_screen_size(window.getSize());

    resource_manager.load_graphic("loading.png", Graphic::GAME);
    gui.set_loading_texture(resource_manager.get_texture("loading.png"));
    resource_manager.load_graphic("panel.png", Graphic::INTERFACE);
    gui.set_right_pannel_texture(resource_manager.get_texture("panel.png"));
    resource_manager.load_graphic("equip.png", Graphic::INTERFACE);

    gui.editbox->connect("Focused", [&]{keyboard.block = true;});
    gui.editbox->connect("Unfocused", [&]{keyboard.block = false;});
    gui.editbox->connect("ReturnKeyPressed", &Engine::sendMessage, this);

    sf::Vector2u screen_size = gui.leftoverScreenSize();
    map.set_screen_size(screen_size);
    hero.set_screen_size(screen_size);

    gui.setupCompleted();
}

void Engine::sendMessage(const std::string& value)
{
    gui.editbox->setText(sf::String());
    network.queueMessage(value);
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
        hero.move(keyboard.dir.front());

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
            //p[16] is hero base strength
            //p[17] is hero base agility
            //p[18] is hero base intelligence
            //p[19] is hero profession name
            resource_manager.set_mpath(p[11]);
            setup_gui();//we need mpath

            resource_manager.load_graphic(p[10], Graphic::HERO);
            hero.set_texture(resource_manager.get_texture(p[10]));
            sf::Vector2i pos(std::stoi(p[0]), std::stoi(p[1]));
            hero.set_pos(pos);
            hero.set_dir(p[2]);
            map.center(pos);
            network.set_pdir(p[2]);
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
            hero.set_pos(pos);
            hero.set_dir(p[2]);
            map.center(pos);
            network.set_pdir(p[2]);
            break;
        }
        case char2int("element"):
        {
            std::string parm = line.substr(colon + 1);
            std::vector<std::string> p = split2(parm, '=', ';');

            //p[0] element type
            //p[1] is item id
            switch(str2int(p[0]))
            {
            case char2int("item"):
                //p[2] is item name
                //p[3-4] is item position
                //p[5] is item graphic
                resource_manager.load_graphic(p[5], Graphic::ITEM);
                map.items[p[1]].set_texture(resource_manager.get_texture(p[5]));
                map.items[p[1]].set_position(sf::Vector2i(std::stoi(p[3]), std::stoi(p[4])));
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
                map.NPCs[p[1]].set_texture(resource_manager.get_texture(p[5]));
                map.NPCs[p[1]].set_position(sf::Vector2i(std::stoi(p[3]), std::stoi(p[4])));
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
                map.players[p[1]].set_texture(resource_manager.get_texture(p[7]));
                map.players[p[1]].set_position(sf::Vector2i(std::stoi(p[3]), std::stoi(p[4])));
                map.players[p[1]].set_dir(p[6]);
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
            //p2[0] is event
            switch(str2int(p2[0]))
            {
            case char2int("move"):
                //p2[1-2] is id position
                //p2[3] is id direction
                map.players[p1[0]].set_position(sf::Vector2i(std::stoi(p2[1]), std::stoi(p2[2])));
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
            case char2int("item"):
                map.items.erase(p[0]);
                break;
            case char2int("npc"):
                map.NPCs.erase(p[0]);
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
        case char2int("chat"):
        {
            std::string parm = line.substr(colon + 1);
            gui.addChatMessage(parm);
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
        case char2int("reload"):
        {
            map.clear();
            gui.clear();
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

#include "engine.hpp"
#include <SFML/System/Sleep.hpp>
#include <iostream>
#include <cstring>

Engine::Engine()
{
    http.setHost("http://game.oldmargonem.pl/");
}

sf::Time Engine::clock_restart()
{
    sf::Time now = clock.getElapsedTime();
    sf::Time diff = now - last_clock;
    last_clock = now;
    return diff;
}

std::string sha1(const std::string& password)
{
    //TODO
    return password;
}
std::string get_cookie_value(const std::string& field)
{
    return field.substr(0, field.find(' ') + 1);
}
std::string get_pid_value(const std::string& body)
{
    size_t size = body.find("value") + 7;
    return body.substr(size, body.find('"', size) - size);
}

void Engine::login(const std::string& login, const std::string& password)
{
    sf::Http http_login("http://www.oldmargonem.pl/");
    sf::Http::Request request_login("/ajax/logon.php?t=login",
                                    sf::Http::Request::Post,
                                    "l=" + login + "&ph=" + sha1(password));
    sf::Http::Response response_login = http_login.sendRequest(request_login);

    //INIT
    pdir = '0';
    bseq = '0';
    lastcch = '0';
    lastch = '0';
    ev = '0';
    pid = get_pid_value(response_login.getBody());
    cookie = get_cookie_value(response_login.getField("Set-Cookie"));
    cookie += get_cookie_value(response_login.getField("Set-Cookie0"));
    cookie += get_cookie_value(response_login.getField("Set-Cookie1"));
    cookie += "mchar_id" + pid;
    request.setField("Cookie", cookie);
}

void Engine::logout()
{
    request = sf::Http::Request();
    cookie.clear();
    pid.clear();
    ev.clear();
    lastch.clear();
    lastcch.clear();
    bseq.clear();
    pdir.clear();
}

void Engine::load_game()
{
    send_command("initlvl=1&build=1007&task=init");
    process_response(response.getBody());
    send_command("initlvl=2&task=init");
    process_response(response.getBody());
    send_command("initlvl=3&task=init");
    process_response(response.getBody());
    send_command("initlvl=4&task=init");
    process_response(response.getBody());
}

void Engine::send_command(const std::string& command)
{
    request.setUri("/db.php?"+
                   command+
                   "&pid="+pid+
                   "&ev="+ev+
                   "&lastch="+lastch+
                   "&lastcch="+lastcch+
                   "&bseq="+bseq+
                   "&pdir="+pdir+
                   "&a="+std::to_string(clock.getElapsedTime().asMicroseconds()));
    response = http.sendRequest(request);
}

void Engine::process_response(const std::string& body)
{
    size_t old_pos = 0;
    for(;;)
    {
        size_t new_pos = body.find("<eol>\n<eol>", old_pos);
        process_line(body.substr(old_pos, new_pos - old_pos));
        if(new_pos == std::string::npos)
            break;
        else
            old_pos = new_pos + 11;
    }
}

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

void Engine::process_line(const std::string& line)
{
    size_t pos = line.find(':');
    std::string cmd = line.substr(0, pos++);

    switch(str2int(cmd))
    {
    case char2int("lastevent"):
    {
        ev = line.substr(pos);
        break;
    }
    case char2int("maxchat"):
    {
        lastch = line.substr(pos);
        break;
    }
    case char2int("maxcchat"):
    {
        lastcch = line.substr(pos);
        break;
    }
    case char2int("sqltime"):
    {
        sf::sleep(sf::microseconds(std::stof(line.substr(pos)) * 1000));
        break;
    }
    case char2int("end"):
    {
        break;
    }
    default:
    {
        std::cout << cmd << " NOT IMPLEMENTED\n";
        break;
    }
    }// end switch
}

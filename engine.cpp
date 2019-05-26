#include "engine.hpp"

std::string sha1(const std::string& password)
{
    //TODO
    return password;
}

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

void Engine::login(const std::string& login, const std::string& password)
{
    sf::Http http_login("http://www.oldmargonem.pl/");
    sf::Http::Request request_login("/ajax/logon.php?t=login",
                              sf::Http::Request::Post,
                              "l=" + login + "&ph=" + sha1(password));
    sf::Http::Response response_login = http_login.sendRequest(request_login);

    //TODO
    std::string temp;
    temp = response_login.getField("Set-Cookie");
    cookie = temp.substr(0, temp.find(' ') + 1);
    temp = response_login.getField("Set-Cookie0");
    cookie += temp.substr(0, temp.find(' ') + 1);
    temp = response_login.getField("Set-Cookie1");
    cookie += temp.substr(0, temp.find(' ') + 1);

    temp = response_login.getBody();
    size_t size = temp.find("value") + 7;
    pid = temp.substr(size, temp.find('"', size) - size);
    lastch = '0';
    lastcch = '0';
    bseq = '0';
    pdir = '0';

    cookie += "mchar_id=" + pid;
    request.setField("Cookie", cookie);
}

void Engine::logout()
{
    cookie.clear();
    pid.clear();
    lastch.clear();
    lastcch.clear();
    bseq.clear();
    pdir.clear();
    request = sf::Http::Request();
}

void Engine::load_game()
{
    send_command("initlvl=1&build=1007&task=init");
    send_command("initlvl=2&task=init");
    send_command("initlvl=3&task=init");
    send_command("initlvl=4&task=init");
}

void Engine::send_command(const std::string& command)
{
    request.setUri("/db.php?"+
                   command+
                   "&pid="+pid+
                   "&ev="+std::to_string(ev)+
                   "&lastch="+lastch+
                   "&lastcch="+lastcch+
                   "&bseq="+bseq+
                   "&pdir="+pdir+
                   "&a="+std::to_string(clock.getElapsedTime().asMicroseconds()));
    response = http.sendRequest(request);
    ++ev;
}

void Engine::process_response()
{
}

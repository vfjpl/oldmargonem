#include "engine.hpp"

std::string sha1(const std::string& password)
{
    //TODO
    return password;
}

std::string get_cookie_value(const std::string& cookie)
{
    return cookie.substr(0, cookie.find(' ') + 1);
}

std::string get_pid_value(const std::string& body)
{
    size_t size = body.find("value") + 7;
    return body.substr(size, body.find('"', size) - size);
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

    //INIT
    pdir = '0';
    bseq = '0';
    lastcch = '0';
    lastch = '0';
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
    lastch.clear();
    lastcch.clear();
    bseq.clear();
    pdir.clear();
}

void Engine::send_command(const std::string& command)
{
    request.setUri("/db.php?"+
                   command+
                   "&pid="+pid+
                   "&ev="+std::to_string(ev++)+
                   "&lastch="+lastch+
                   "&lastcch="+lastcch+
                   "&bseq="+bseq+
                   "&pdir="+pdir+
                   "&a="+std::to_string(clock.getElapsedTime().asMicroseconds()));
    response = http.sendRequest(request);
}

void Engine::process_response()
{

}

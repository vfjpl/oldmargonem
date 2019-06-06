#include "network.hpp"
#include "config.hpp"
#include <sys/time.h>

namespace
{
std::string sha1(const std::string& password)
{
    //TODO SHA1
    return password;
}
std::string get_cookie(const std::string& field)
{
    return field.substr(0, field.find(' ') + 1);
}
std::string get_pid_value(const std::string& body)
{
    size_t pos = body.find("value") + 7;
    return body.substr(pos, body.find('"', pos) - pos);
}
std::string get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long rev = (tv.tv_sec*10000) + (tv.tv_usec/100);
    return std::to_string(rev).substr(5);
}
}

Network::Network()
{
    http.setHost(GAME_ADDRES);
}

void Network::set_ev(const std::string& value)
{
    ev = value;
}

void Network::set_lastch(const std::string& value)
{
    lastch = value;
}

void Network::set_lastcch(const std::string& value)
{
    lastcch = value;
}

void Network::set_bseq(const std::string& value)
{
    bseq = value;
}

void Network::set_pdir(const std::string& value)
{
    pdir = value;
}

const std::string& Network::get_response() const
{
    return response.getBody();
}

void Network::login(const std::string& login, const std::string& password)
{
    sf::Http http_login(LOGIN_ADDRES);
    sf::Http::Request request_login("ajax/logon.php?t=login",
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
    cookie = get_cookie(response_login.getField("Set-Cookie"));
    cookie += get_cookie(response_login.getField("Set-Cookie0"));
    cookie += get_cookie(response_login.getField("Set-Cookie1"));
    cookie += "mchar_id=" + pid;
    request.setField("Cookie", cookie);
}

void Network::queue_move(sf::Vector2i value)
{
    if(ml.empty())
        ml = std::to_string(value.x) + ',' + std::to_string(value.y);
    else
        ml += ';' + std::to_string(value.x) + ',' + std::to_string(value.y);
}

void Network::queue_command(const std::string& command)
{
    fifo.push(command);
}

void Network::send_command()
{
    //TODO BATTLE
    std::string cmd;
    if(!fifo.empty())
    {
        cmd = fifo.front();
        fifo.pop();
    }
    else if(!ml.empty())
    {
        cmd = "dir=" + pdir + "&ml=" + ml;
        ml.clear();
    }
    else
    {
        cmd = "task=_";
    }

    request.setUri("db.php?"+
                   cmd+
                   "&pid="+pid+
                   "&ev="+ev+
                   "&lastch="+lastch+
                   "&lastcch="+lastcch+
                   "&bseq="+bseq+
                   "&pdir="+pdir+
                   "&a="+get_time());
    response = http.sendRequest(request);
}

void Network::load_sequence()
{
    fifo.emplace("initlvl=1&build=1007&task=init");
    fifo.emplace("initlvl=2&task=init");
    fifo.emplace("initlvl=3&task=init");
    fifo.emplace("initlvl=4&task=init");
}

#include "network.hpp"

namespace
{
std::string sha1(const std::string& password)
{
    //TODO
    return password;
}
std::string get_cookie(const std::string& field)
{
    return field.substr(0, field.find(' ') + 1);
}
std::string get_pid_value(const std::string& body)
{
    size_t size = body.find("value") + 7;
    return body.substr(size, body.find('"', size) - size);
}
}

Network::Network()
{
    http.setHost("http://game.oldmargonem.pl/");
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
    sf::Http http_login("http://www.oldmargonem.pl/");
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

void Network::logout()
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

void Network::send_command(const std::string& command, sf::Time a)
{
    request.setUri("db.php?"+
                   command+
                   "&pid="+pid+
                   "&ev="+ev+
                   "&lastch="+lastch+
                   "&lastcch="+lastcch+
                   "&bseq="+bseq+
                   "&pdir="+pdir+
                   "&a="+std::to_string(a.asMicroseconds()));
    response = http.sendRequest(request);
}

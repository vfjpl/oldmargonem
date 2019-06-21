#include "network.hpp"
#include "config.hpp"
#include <Poco/Net/NameValueCollection.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPCookie.h>
#include <sys/time.h>
#include <sstream>

namespace
{
std::string sha1(const std::string& password)
{
    //TODO SHA1
    return password;
}
std::string get_pid_value(const std::stringstream& ss)
{
    const std::string &temp = ss.str();
    size_t pos = temp.find("value") + 7;
    return temp.substr(pos, temp.find('"', pos) - pos);
}
std::string get_time_string()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long rev = tv.tv_usec;
    return std::to_string(rev);
}
}

Network::Network()
{
    session.setHost(GAME_ADDRES);
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

void Network::login(const std::string& login, const std::string& password)
{
    //send
    Poco::Net::HTMLForm login_form;
    login_form.add("l", login);
    login_form.add("ph", sha1(password));
    Poco::Net::HTTPClientSession login_session(LOGIN_ADDRES);
    Poco::Net::HTTPRequest login_request(Poco::Net::HTTPRequest::HTTP_POST, "/ajax/logon.php?t=login");
    login_form.prepareSubmit(login_request);
    login_form.write(login_session.sendRequest(login_request));

    //receive
    Poco::Net::HTTPResponse response;
    std::stringstream body;
    login_session.receiveResponse(response) >> body.rdbuf();

    //INIT
    pdir = '0';
    bseq = '0';
    lastcch = '0';
    lastch = '0';
    ev = '0';
    pid = get_pid_value(body);

    //Cookies
    Poco::Net::NameValueCollection cookies;
    std::vector<Poco::Net::HTTPCookie> cookies_vec;
    response.getCookies(cookies_vec);
    for(auto &i : cookies_vec)
        cookies.add(i.getName(), i.getValue());
    cookies.add("mchar_id", pid);
    request.setCookies(cookies);
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

void Network::queue_load_sequence()
{
    fifo.emplace("initlvl=1&build=1007&task=init");
    fifo.emplace("initlvl=2&task=init");
    fifo.emplace("initlvl=3&task=init");
    fifo.emplace("initlvl=4&task=init");
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
        cmd = "dir=" + pdir + "&ml=" + ml + "&task=go";
        ml.clear();
    }
    else
    {
        cmd = "task=_";
    }

    request.setURI("/db.php?"+
                   cmd+
                   "&pid="+pid+
                   "&ev="+ev+
                   "&lastch="+lastch+
                   "&lastcch="+lastcch+
                   "&bseq="+bseq+
                   "&pdir="+pdir+
                   "&a="+get_time_string());

    session.sendRequest(request);
    Poco::Net::HTTPResponse response;

    //std::cout << var.toString();
}

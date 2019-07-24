#include "network.hpp"
#include "config.hpp"
#include <Poco/SHA1Engine.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/NameValueCollection.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPCookie.h>
#include <sys/time.h>
#include <sstream>

namespace
{
std::string sha1(const std::string& password)
{
    Poco::SHA1Engine sha1engine;
    sha1engine.update(PASSHASH + password);
    return sha1engine.digestToHex(sha1engine.digest());
}
std::string getPidValue(const std::string& body)
{
    size_t pos = body.find("value") + 7;
    return body.substr(pos, body.find('"', pos) - pos);
}
std::string getTimeString()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    //9 digits
    unsigned long rev = (tv.tv_sec*10000) + (tv.tv_usec/100);
    return std::to_string(rev).substr(5);
}
std::string toString(std::istream& stream)
{
    std::stringstream ss;
    stream >> ss.rdbuf();
    return ss.str();
}
}

Network::Network()
{
    session.setHost(GAME_ADDRES);
    session.setKeepAlive(true);
    request.setVersion(Poco::Net::HTTPRequest::HTTP_1_1);
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

void Network::set_pdir(char value)
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
    std::string body = toString(login_session.receiveResponse(response));

    //INIT
    pdir = '0';
    bseq = '0';
    lastcch = '0';
    lastch = '0';
    ev = '0';
    pid = getPidValue(body);

    //Cookies
    std::vector<Poco::Net::HTTPCookie> cookies_vec;
    response.getCookies(cookies_vec);
    Poco::Net::NameValueCollection cookies;
    for(auto &i : cookies_vec)
        cookies.add(i.getName(), i.getValue());
    cookies.add("mchar_id", pid);
    request.setCookies(cookies);
}

void Network::queueMove(sf::Vector2i value)
{
    if(ml.empty())
        ml = std::to_string(value.x) + ',' + std::to_string(value.y);
    else
        ml += ';' + std::to_string(value.x) + ',' + std::to_string(value.y);
}

void Network::queueEnter()
{
    fifo.emplace("task=walk");
}

void Network::queueFight(unsigned long id)
{
    if(id == ULONG_MAX)
        return;
    fifo.emplace("auto=1&attack=" + std::to_string(id)  + "&task=fight");
}

void Network::queueMessage(const std::string& value)
{
    //TODO: proper handling
    fifo.emplace("tekst=" + value + "&task=chat");
}

void Network::queueLoadSequence()
{
    fifo.emplace("initlvl=1&build=1007&task=init");
    fifo.emplace("initlvl=2&task=init");
    fifo.emplace("initlvl=3&task=init");
    fifo.emplace("initlvl=4&task=init");
}

void Network::sendRequest()
{
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
                   "&a="+getTimeString());

    session.sendRequest(request);
}

std::string Network::receiveResponse()
{
    Poco::Net::HTTPResponse response;
    return toString(session.receiveResponse(response));
}

#include "network.hpp"
#include <Poco/SHA1Engine.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/NameValueCollection.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPCookie.h>
#include <sys/time.h>
#include <sstream>

namespace
{
std::string Query::percentEncode(const std::string& value)
{
    std::string msg;
    Poco::URI::encode(value, Poco::URI::RESERVED_QUERY_PARAM, msg);
    return msg;
}
std::string sha1(const std::string& password)
{
    Poco::SHA1Engine sha1engine;
    sha1engine.update("mleczko" + password);
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
    //1s == 1000ms == 1000000us
    unsigned long rev = (tv.tv_sec*10000) + (tv.tv_usec/100);
    //9 digits == 00000.0000s
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
    session.setKeepAlive(true);
    session.setHost("game.oldmargonem.pl");
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
    Poco::Net::HTTPClientSession login_session("www.oldmargonem.pl");
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

    //cookies
    std::vector<Poco::Net::HTTPCookie> cookies_vec;
    Poco::Net::NameValueCollection cookies;

    response.getCookies(cookies_vec);
    for(auto &i : cookies_vec)
        cookies.add(i.getName(), i.getValue());
    cookies.add("mchar_id", pid);
    request.setCookies(cookies);
}

void Network::queueMove(sf::Vector2i value)
{
    fifo.emplace("dir=" + pdir + "&ml=" + std::to_string(value.x) + ',' + std::to_string(value.y) + "&task=go");
}

void Network::queueEnter()
{
    fifo.emplace("task=walk");
}

void Network::queueFight(unsigned long id)
{
    fifo.emplace("auto=1&attack=" + std::to_string(id)  + "&task=fight");
}

void Network::queueMessage(const char* value)
{
    fifo.emplace("tekst=" + Query::percentEncode(value) + "&task=chat");
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
    if(fifo.empty())
        fifo.emplace("task=_");

    request.setURI("/db.php?"+
                   fifo.front()+
                   "&pid="+pid+
                   "&ev="+ev+
                   "&lastch="+lastch+
                   "&lastcch="+lastcch+
                   "&bseq="+bseq+
                   "&pdir="+pdir+
                   "&a="+getTimeString());

    session.sendRequest(request);
    fifo.pop();
}

std::string Network::receiveResponse()
{
    Poco::Net::HTTPResponse response;
    return toString(session.receiveResponse(response));
}

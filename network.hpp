#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <SFML/System/Vector2.hpp>
#include <queue>

class Network
{
    Poco::Net::HTTPClientSession session;
    Poco::Net::HTTPRequest request;
    std::queue<std::string> fifo;
    std::string ml;

    std::string pid;
    std::string ev;
    std::string lastch;
    std::string lastcch;
    std::string bseq;
    std::string pdir;

public:
    Network();
    void set_ev(const std::string& value);
    void set_lastch(const std::string& value);
    void set_lastcch(const std::string& value);
    void set_bseq(const std::string& value);
    void set_pdir(const std::string& value);

    //logs in and sets necessary cookies(currently you have to give it sha1 hashed password)
    void login(const std::string& login, const std::string& password);

    void queue_move(sf::Vector2i value);
    void queue_command(const std::string& command);
    void queue_load_sequence();

    std::istream& send_command();
};

#endif // NETWORK_HPP_INCLUDED

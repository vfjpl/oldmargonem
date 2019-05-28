#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <SFML/Network/Http.hpp>
#include <queue>

class Network
{
    sf::Http http;
    sf::Http::Request request;
    sf::Http::Response response;
    std::queue<std::string> fifo;
    std::string cookie;
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
    const std::string& get_response() const;

    void login(const std::string& login, const std::string& password);
    void logout();

    void queue_command(const std::string& command);
    void send_command(sf::Time a);
};

#endif // NETWORK_HPP_INCLUDED

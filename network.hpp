#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <SFML/Network/Http.hpp>

class Network
{
    sf::Http http;
    sf::Http::Request request;
    sf::Http::Response response;
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
    const std::string& get_response() const;

    void login(const std::string& login, const std::string& password);
    void logout();

    void send_command(const std::string& command, const sf::Time a);
};

#endif // NETWORK_HPP_INCLUDED

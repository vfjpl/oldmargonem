#ifndef MYCLOCK_HPP_INCLUDED
#define MYCLOCK_HPP_INCLUDED

#include <SFML/System/Clock.hpp>

class MyClock
{
    sf::Clock clock;
    sf::Time last_clock;

public:
    sf::Time getElapsedTime() const;
    sf::Time restart();
};

#endif // MYCLOCK_HPP_INCLUDED

#ifndef MYCLOCK_HPP_INCLUDED
#define MYCLOCK_HPP_INCLUDED

#include <SFML/System/Clock.hpp>

class MyClock
{
    sf::Clock clock;
    sf::Time last_clock;
    sf::Time walk_time;

public:
    void update();
    bool walkInterrupt();
    float getWalkTime() const;
};

#endif // MYCLOCK_HPP_INCLUDED

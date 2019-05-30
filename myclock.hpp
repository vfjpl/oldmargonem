#ifndef MYCLOCK_HPP_INCLUDED
#define MYCLOCK_HPP_INCLUDED

#include <SFML/System/Clock.hpp>

class MyClock
{
    sf::Clock clock;
    sf::Time last_clock;
    sf::Time interrupt_time;

public:
    sf::Time getElapsedTime() const;
    sf::Time getInterruptTime() const;

    void update();
    bool interrupt();
};

#endif // MYCLOCK_HPP_INCLUDED

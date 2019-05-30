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

    //updates interrupt timer
    void update();
    //true only when certain amount of time have passed
    bool interrupt();
};

#endif // MYCLOCK_HPP_INCLUDED

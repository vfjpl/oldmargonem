#ifndef MYCLOCK_HPP_INCLUDED
#define MYCLOCK_HPP_INCLUDED

#include <SFML/System/Clock.hpp>

class MyClock
{
    sf::Clock clock;
    sf::Time last_clock;
    sf::Time move_time;

public:
    float getMoveTime() const;

    //updates interrupt times
    void update();
    //true only when certain amount of time have passed
    bool moveInterrupt();
};

#endif // MYCLOCK_HPP_INCLUDED

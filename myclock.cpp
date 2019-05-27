#include "myclock.hpp"

sf::Time MyClock::getElapsedTime()
{
    return clock.getElapsedTime();
}

sf::Time MyClock::restart()
{
    sf::Time now = clock.getElapsedTime();
    sf::Time diff = now - last_clock;
    last_clock = now;
    return diff;
}

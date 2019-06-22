#include "myclock.hpp"
#include "config.hpp"

sf::Time MyClock::getElapsedTime() const
{
    return clock.getElapsedTime();
}

sf::Time MyClock::getMoveTime() const
{
    return move_time;
}

void MyClock::update()
{
    sf::Time now = clock.getElapsedTime();
    move_time += now - last_clock;
    last_clock = now;
}

bool MyClock::moveInterrupt()
{
    if(move_time.asSeconds() < 1/MOVEMENT_SPEED)
        return false;

    move_time = sf::Time::Zero;
    return true;
}

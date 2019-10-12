#include "myclock.hpp"
#include "config.hpp"

void MyClock::update()
{
    sf::Time now = clock.getElapsedTime();
    walk_time += now - last_clock;
    last_clock = now;
}

bool MyClock::walkInterrupt()
{
    if(walk_time < sf::seconds(1/MOVEMENT_SPEED))
        return false;

    walk_time = sf::Time::Zero;
    return true;
}

float MyClock::getWalkTime() const
{
    return walk_time.asSeconds();
}

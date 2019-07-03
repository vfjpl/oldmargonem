#include "myclock.hpp"
#include "config.hpp"

float MyClock::getMoveTime() const
{
    return move_time.asSeconds();
}

void MyClock::update()
{
    sf::Time now = clock.getElapsedTime();
    move_time += now - last_clock;
    last_clock = now;
}

bool MyClock::moveInterrupt()
{
    if(move_time < sf::seconds(1/MOVEMENT_SPEED))
        return false;

    move_time = sf::Time::Zero;
    return true;
}

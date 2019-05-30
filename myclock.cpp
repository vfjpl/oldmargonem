#include "myclock.hpp"
#include "config.hpp"

sf::Time MyClock::getElapsedTime() const
{
    return clock.getElapsedTime();
}

sf::Time MyClock::getInterruptTime() const
{
    return interrupt_time;
}

void MyClock::update()
{
    sf::Time now = clock.getElapsedTime();
    interrupt_time += now - last_clock;
    last_clock = now;
}

bool MyClock::interrupt()
{
    if(interrupt_time.asSeconds() < INTERRUPT_TIME)
        return false;

    interrupt_time = sf::Time::Zero;
    return true;
}

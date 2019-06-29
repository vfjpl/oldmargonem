#include "mykeyboard.hpp"

bool MyKeyboard::anyKey() const
{
    return (left != right) || (up != down);
}

sf::Vector2i MyKeyboard::getPosChange() const
{
    return sf::Vector2i(right - left, down - up);
}

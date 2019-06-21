#include "mykeyboard.hpp"

bool MyKeyboard::anyKey() const
{
    return up || left || down || right;
}

sf::Vector2i MyKeyboard::getPosChange() const
{
    return sf::Vector2i(right - left,
                        down - up);
}

#include "mykeyboard.hpp"

bool MyKeyboard::anyKey() const
{
    switch(dir.front())
    {
    case '3':
        return up;
    case '1':
        return left;
    case '0':
        return down;
    case '2':
        return right;
    }// end switch
    return false;
}

sf::Vector2i MyKeyboard::getPosChange() const
{
    switch(dir.front())
    {
    case '3':
        return sf::Vector2i(0, -1);
    case '1':
        return sf::Vector2i(-1, 0);
    case '0':
        return sf::Vector2i(0, 1);
    case '2':
        return sf::Vector2i(1, 0);
    }// end switch
    return sf::Vector2i(0, 0);
}

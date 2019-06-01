#include "mykeyboard.hpp"

bool MyKeyboard::anyKey() const
{
    return up || left || down || right;
}

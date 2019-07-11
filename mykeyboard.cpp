#include "mykeyboard.hpp"

bool MyKeyboard::anyKey() const
{
    if(block)
        return false;

    switch(dir.front())
    {
    case '0':
        return keys[0];
    case '1':
        return keys[1];
    case '2':
        return keys[2];
    case '3':
        return keys[3];
    }// end switch
    return false;
}

#ifndef MYKEYBOARD_HPP_INCLUDED
#define MYKEYBOARD_HPP_INCLUDED

#include <string>

class MyKeyboard
{
public:
    std::string dir;
    bool keys[4];
    bool block = false;

    bool anyKey() const;
};

#endif // MYKEYBOARD_HPP_INCLUDED

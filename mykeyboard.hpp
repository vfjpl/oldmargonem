#ifndef MYKEYBOARD_HPP_INCLUDED
#define MYKEYBOARD_HPP_INCLUDED

class MyKeyboard
{
public:
    char dir;
    bool keys[4];
    bool block = false;

    bool anyKey() const;
};

#endif // MYKEYBOARD_HPP_INCLUDED

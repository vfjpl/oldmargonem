#ifndef MYKEYBOARD_HPP_INCLUDED
#define MYKEYBOARD_HPP_INCLUDED

class MyKeyboard
{
public:
    bool up = false;
    bool left = false;
    bool down = false;
    bool right = false;

    bool anyKey() const;
};

#endif // MYKEYBOARzD_HPP_INCLUDED

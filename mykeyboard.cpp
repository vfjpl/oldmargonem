#include "mykeyboard.hpp"

void MyKeyboard::update()
{
    prev_up = up;
    prev_left = left;
    prev_down = down;
    prev_right = right;
}

void MyKeyboard::set_up(bool value)
{
    up = value;
}
void MyKeyboard::set_left(bool value)
{
    left = value;
}
void MyKeyboard::set_down(bool value)
{
    down = value;
}
void MyKeyboard::set_right(bool value)
{
    right = value;
}

bool MyKeyboard::any_key() const
{
    return up_key() || left_key() || down_key() || right_key();
}

bool MyKeyboard::held_any_key() const
{
    return held_up() || held_left() || held_down() || held_right();
}

bool MyKeyboard::up_key() const
{
    return up && !prev_up;
}
bool MyKeyboard::left_key() const
{
    return left && !prev_left;
}
bool MyKeyboard::down_key() const
{
    return down && !prev_down;
}
bool MyKeyboard::right_key() const
{
    return right && !prev_right;
}

bool MyKeyboard::held_up() const
{
    return up && prev_up;
}
bool MyKeyboard::held_left() const
{
    return left && prev_left;
}
bool MyKeyboard::held_down() const
{
    return down && prev_down;
}
bool MyKeyboard::held_right() const
{
    return right && prev_right;
}

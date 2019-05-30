#include "mykeyboard.hpp"

void MyKeyboard::update()
{
    pr_up = up;
    pr_left = left;
    pr_down = down;
    pr_right = right;
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
    return up && !pr_up;
}
bool MyKeyboard::left_key() const
{
    return left && !pr_left;
}
bool MyKeyboard::down_key() const
{
    return down && !pr_down;
}
bool MyKeyboard::right_key() const
{
    return right && !pr_right;
}

bool MyKeyboard::held_up() const
{
    return up && pr_up;
}
bool MyKeyboard::held_left() const
{
    return left && pr_left;
}
bool MyKeyboard::held_down() const
{
    return down && pr_down;
}
bool MyKeyboard::held_right() const
{
    return right && pr_right;
}

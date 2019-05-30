#include "mykeyboard.hpp"

void MyKeyboard::update()
{
    prev_up = up;
    prev_left = left;
    prev_down = down;
    prev_right = right;
}

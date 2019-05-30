#ifndef MYKEYBOARD_HPP_INCLUDED
#define MYKEYBOARD_HPP_INCLUDED

class MyKeyboard
{
    bool up = false;
    bool prev_up = false;
    bool left = false;
    bool prev_left = false;
    bool down = false;
    bool prev_down = false;
    bool right = false;
    bool prev_right = false;

public:
    void update();

    void set_up(bool value);
    void set_left(bool value);
    void set_down(bool value);
    void set_right(bool value);

    bool any_key() const;
    bool held_any_key() const;

    bool up_key() const;
    bool left_key() const;
    bool down_key() const;
    bool right_key() const;

    bool held_up() const;
    bool held_left() const;
    bool held_down() const;
    bool held_right() const;
};

#endif // MYKEYBOARzD_HPP_INCLUDED

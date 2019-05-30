#ifndef MYKEYBOARD_HPP_INCLUDED
#define MYKEYBOARD_HPP_INCLUDED

class MyKeyboard
{
public:
    bool up = false;
    bool pr_up = false;
    bool left = false;
    bool pr_left = false;
    bool down = false;
    bool pr_down = false;
    bool right = false;
    bool pr_right = false;


    void update();
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

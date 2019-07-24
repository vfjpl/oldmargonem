#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/ChatBox.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Gui
{
public:
    tgui::Gui tgui;
    tgui::ChatBox::Ptr chatbox;
    tgui::EditBox::Ptr editbox;

private:
    sf::Sprite right_panel_sprite;
    sf::Vector2u usable_screen_size;
    sf::Vector2u interface_size;

public:
    Gui();
    void set_screen_size(sf::Vector2u value);
    sf::Vector2u usableScreenSize() const;

    void set_right_pannel_texture(const sf::Texture& texture);

    void draw(sf::RenderWindow& window);
};

#endif // GUI_HPP_INCLUDED

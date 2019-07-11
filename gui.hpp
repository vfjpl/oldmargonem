#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

#include <TGUI/Gui.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Gui
{
public:
    tgui::Gui tgui;

private:
    sf::Sprite loading_sprite;

public:
    void set_texture(const sf::Texture& texture);
    void draw(sf::RenderWindow& window);
};

#endif // GUI_HPP_INCLUDED

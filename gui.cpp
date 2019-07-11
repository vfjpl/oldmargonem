#include "gui.hpp"

void Gui::set_texture(const sf::Texture& texture)
{
    loading_sprite.setTexture(texture);
}

void Gui::draw(sf::RenderWindow& window)
{
    //tgui.draw();
    window.draw(loading_sprite);
}

#include "gui.hpp"

void Gui::set_screen_size(sf::Vector2u value)
{
    screen_size = value;
}

void Gui::set_texture(const sf::Texture& texture)
{
    loading_sprite.setTexture(texture);

    sf::IntRect rect;
    rect.width = screen_size.x;
    rect.height = screen_size.y;
    sf::Vector2f offset = (sf::Vector2f(texture.getSize())/2.f) - (sf::Vector2f(screen_size)/2.f);
    rect.left = offset.x;
    rect.top = offset.y;
    loading_sprite.setTextureRect(rect);
}

void Gui::draw(sf::RenderWindow& window)
{
    //tgui.draw();
    window.draw(loading_sprite);
}

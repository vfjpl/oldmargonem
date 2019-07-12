#include "gui.hpp"

bool Gui::isDataLoaded() const
{
    return dataLoaded;
}

void Gui::dataLoadCompleted()
{
    dataLoaded = true;
}

void Gui::showLoadScreen()
{
    show_load_screen = true;
}

void Gui::hideLoadScreen()
{
    show_load_screen = false;
}

void Gui::set_screen_size(sf::Vector2u value)
{
    screen_size = value;
}

sf::Vector2u Gui::leftoverScreenSize() const
{
    return leftover_size;
}

void Gui::set_right_pannel_texture(const sf::Texture& texture)
{
    right_panel_sprite.setTexture(texture);
    sf::Vector2u texture_size = texture.getSize();
    leftover_size = screen_size;
    leftover_size.x -= texture_size.x;

    sf::Vector2f offset;
    offset.x = leftover_size.x;
    right_panel_sprite.setPosition(offset);
}

void Gui::set_loading_texture(const sf::Texture& texture)
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
    window.draw(right_panel_sprite);
    //tgui.draw();
    if(show_load_screen)
        window.draw(loading_sprite);
}

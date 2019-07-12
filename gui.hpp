#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/ChatBox.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Gui
{
public:
    tgui::Gui tgui;

private:
    sf::Sprite right_panel_sprite;
    sf::Sprite loading_sprite;
    tgui::ChatBox::Ptr chatbox;
    sf::Vector2u screen_size;
    sf::Vector2u leftover_size;
    bool show_load_screen = false;
    bool dataLoaded = false;

public:
    void setupTGUI();
    bool isDataLoaded() const;
    void dataLoadCompleted();
    void showLoadScreen();
    void hideLoadScreen();

    void set_screen_size(sf::Vector2u value);
    sf::Vector2u leftoverScreenSize() const;

    void set_loading_texture(const sf::Texture& texture);
    void set_right_pannel_texture(const sf::Texture& texture);

    void addChatMessage(const std::string& msg);
    void clear();

    void draw(sf::RenderWindow& window);
};

#endif // GUI_HPP_INCLUDED

#include "gui.hpp"

//792x544 oldmargonem window size
Gui::Gui()
{
    //228x190 oldmargonem chatbox size
    chatbox = tgui::ChatBox::create();
    chatbox->setTextSize(10);
    chatbox->setSize(sf::Vector2f(228, 190));
    chatbox->setTextColor(tgui::Color::White);
    tgui::ChatBoxRenderer* cbr = chatbox->getSharedRenderer();
    cbr->setBorders(tgui::Borders());
    cbr->setBackgroundColor(tgui::Color::Transparent);
    tgui.add(chatbox);

    //235x18 oldmargonem textbox size
    editbox = tgui::EditBox::create();
    editbox->setTextSize(10);
    editbox->setSize(sf::Vector2f(235, 18));
    tgui::EditBoxRenderer* ebr = editbox->getSharedRenderer();
    ebr->setTextColor(tgui::Color::White);
    ebr->setCaretColor(tgui::Color::White);
    ebr->setBorders(tgui::Borders());
    ebr->setBackgroundColor(tgui::Color::Transparent);
    ebr->setBackgroundColorHover(tgui::Color::Transparent);
    tgui.add(editbox);
}

bool Gui::isSetupCompleted() const
{
    return setup_state;
}

void Gui::setupCompleted()
{
    setup_state = true;
}

void Gui::set_screen_size(sf::Vector2u value)
{
    //540x270 oldmargonem chatbox position
    chatbox->setPosition(sf::Vector2f(value.x-(792-540), 270));
    //535x490 oldmargonem textbox position
    editbox->setPosition(sf::Vector2f(value.x-(792-535), 490));

    screen_size = value;
    leftover_size = value;
}

sf::Vector2u Gui::leftoverScreenSize() const
{
    return leftover_size;
}

void Gui::set_right_pannel_texture(const sf::Texture& texture)
{
    right_panel_sprite.setTexture(texture);
    leftover_size.x -= texture.getSize().x;

    sf::Vector2f offset;
    offset.x = leftover_size.x;
    right_panel_sprite.setPosition(offset);
}

void Gui::addChatMessage(const std::string& msg)
{
    chatbox->addLine(msg);
}

void Gui::draw(sf::RenderWindow& window)
{
    window.draw(right_panel_sprite);
    tgui.draw();
}

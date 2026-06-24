#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include <optional>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Platformer");
    window.setFramerateLimit(60);

    // init tgui
    tgui::Gui gui(window);

    // tgui stuff

    // title
    auto titleLabel = tgui::Label::create();
    titleLabel->setText("Label text");
    titleLabel->setTextSize(24);
    titleLabel->setPosition(220, 40);
    titleLabel->getRenderer()->setTextColor(tgui::Color::White);
    gui.add(titleLabel);

    // text box
    auto editBox = tgui::EditBox::create();
    editBox->setSize(200, 40);
    editBox->setPosition(300, 320);
    editBox->setDefaultText("type something");
    gui.add(editBox);

    // button
    auto actionButton = tgui::Button::create("Apply Color");
    actionButton->setSize(120, 40);
    actionButton->setPosition(340, 380);
    gui.add(actionButton);

    // add actions to elements
    actionButton->onPress([&]() {
        // get text input
        std::string text = editBox->getText().toStdString();
        });

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {

            // update ui events
            gui.handleEvent(*event);

            // close window
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                window.close();
            }
        }

        // drawing
        window.clear(sf::Color(255, 255, 255));

        // draw sfml before tgui

        // draw tgui
        gui.draw();

        window.display();
    }

    return 0;
}

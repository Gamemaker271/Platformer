#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include <optional>

int main() {
    // 1. Create the SFML window (SFML 3 uses initialization lists for sizing)
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML 3.1 + TGUI Workspace Test");
    window.setFramerateLimit(60);

    // 2. Initialize TGUI and link it to the window
    tgui::Gui gui(window);

    // 3. Create a decorative background SFML shape to prove SFML works
    sf::CircleShape backgroundCircle(100.f);
    backgroundCircle.setFillColor(sf::Color::Cyan);
    backgroundCircle.setOrigin({ 100.f, 100.f });
    backgroundCircle.setPosition({ 400.f, 180.f });

    // 4. Create TGUI UI Elements

    // Create a Label (Text display)
    auto titleLabel = tgui::Label::create();
    titleLabel->setText("Welcome to your SFML + TGUI Project!");
    titleLabel->setTextSize(24);
    titleLabel->setPosition(220, 40);
    titleLabel->getRenderer()->setTextColor(tgui::Color::White);
    gui.add(titleLabel);

    // Create an Input Text Box
    auto editBox = tgui::EditBox::create();
    editBox->setSize(200, 40);
    editBox->setPosition(300, 320);
    editBox->setDefaultText("Type a color (e.g. Red, Green)...");
    gui.add(editBox);

    // Create an Interactive Button
    auto actionButton = tgui::Button::create("Apply Color");
    actionButton->setSize(120, 40);
    actionButton->setPosition(340, 380);
    gui.add(actionButton);

    // 5. Connect UI events to actions (Callbacks)
    actionButton->onPress([&]() {
        // Read what the user typed into the text box
        std::string text = editBox->getText().toStdString();

        // Change the SFML circle color based on input
        if (text == "Red" || text == "red") {
            backgroundCircle.setFillColor(sf::Color::Red);
        }
        else if (text == "Green" || text == "green") {
            backgroundCircle.setFillColor(sf::Color::Green);
        }
        else if (text == "Blue" || text == "blue") {
            backgroundCircle.setFillColor(sf::Color::Blue);
        }
        else if (text == "Yellow" || text == "yellow") {
            backgroundCircle.setFillColor(sf::Color::Yellow);
        }
        else {
            backgroundCircle.setFillColor(sf::Color::Magenta); // Surprise color
        }
        });

    // 6. Main Application Loop (Updated for SFML 3.1)
    while (window.isOpen()) {
        // pollEvent() now returns a std::optional in SFML 3.1
        while (const std::optional event = window.pollEvent()) {

            // Forward the underlying event to TGUI using the pointer star
            gui.handleEvent(*event);

            // Check if the event is a window close request using the arrow operator
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // 7. Rendering Phase
        window.clear(sf::Color(30, 30, 35)); // Dark slate background color

        // Draw standard SFML graphics first (underneath the UI)
        window.draw(backgroundCircle);

        // Draw TGUI elements last (on top of everything else)
        gui.draw();

        window.display();
    }

    return 0;
}

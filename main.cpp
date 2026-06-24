#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>

const int screenw = 800;
const int screenh = 600;

bool menu = true;


float playerx = 0;
float playery = 0;

sf::RectangleShape player;

bool upkey = false;
bool downkey = false;
bool leftkey = false;
bool rightkey = false;

void Reset()
{

}

void Logic() {
    if (menu) {
        
    }
    else {
        if (upkey)
        {
			playery -= 5;
        }
        if (downkey)
        {
            playery += 5;
        }
        if (leftkey)
        {
            playerx -= 5;
        }
        if (rightkey)
        {
            playerx += 5;
        }

		player.setPosition(sf::Vector2f(playerx, playery));
    }
}
int main() {
    sf::RenderWindow window(sf::VideoMode({ screenw, screenh }), "Platformer");
    window.setFramerateLimit(60);
    // init TGUI
    tgui::Gui menuGui(window);
    tgui::Gui playGui(window);

    // SFML objects
    // player
    player.setFillColor(sf::Color(150, 0, 0));
    player.setSize(sf::Vector2f(50, 50));
    player.setPosition(sf::Vector2f( playerx, playery ));

    // TGUI stuff
    // title
    auto titleLabel = tgui::Label::create();
    titleLabel->setText("C++ Platformer");
    titleLabel->setTextSize(30);
    titleLabel->setPosition(280, 50);
    titleLabel->getRenderer()->setTextColor(tgui::Color::White);
    menuGui.add(titleLabel);

    // button
    auto actionButton = tgui::Button::create("Play");
    actionButton->setSize(120, 40);
    actionButton->setPosition(340, 100);
    menuGui.add(actionButton);

    // add actions to elements
    actionButton->onPress([&]() {
		menu = false;
    });

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            // update ui events
            menuGui.handleEvent(*event);
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                upkey = true;
            else 
                upkey = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                leftkey = true;
            else
                leftkey = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                downkey = true;
            else 
                downkey = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                rightkey = true;
            else 
                rightkey = false;

            // close window
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                window.close();
            }
        }

        Logic();

        // drawing
        window.clear(sf::Color(0, 0, 0));

        // draw sfml before tgui
        if (menu)
        {
            
        }
        else
        {
            window.draw(player);
        }

        // draw tgui
        if (menu)
        {
            menuGui.draw();
        }
        else
        {
            playGui.draw();
        }

        window.display();
        std::cout << "W: " << upkey << " A: " << leftkey << " S: " << downkey << " D:" << rightkey << std::endl;
    }

    return 0;
}

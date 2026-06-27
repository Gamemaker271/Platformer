#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include <vector>

const int screenw = 800;
const int screenh = 600;

bool menu = true;
int level = 1;

float playervely = 0;

bool grounded = false;

float speed = 12;
float gravity = 2;
float jumpForce = 23;

sf::RectangleShape player;
sf::RectangleShape lefteye;
sf::RectangleShape righteye;

sf::RectangleShape portal;

std::vector<sf::RectangleShape> Walls;

bool upkey = false;
bool downkey = false;
bool leftkey = false;
bool rightkey = false;

void Reset()
{
    player.setPosition(sf::Vector2f(100, screenh - player.getSize().y));
    Walls.clear();
    if (level == 1) {
        sf::RectangleShape a;
        // wall 1
        a.setPosition(sf::Vector2f(300, 500));
        a.setSize(sf::Vector2f(150, 100));
        a.setFillColor(sf::Color(50, 50, 50));
        Walls.push_back(a);
        // wall 2
        a.setPosition(sf::Vector2f(550, 400));
        a.setSize(sf::Vector2f(150, 50));
        a.setFillColor(sf::Color(50, 50, 50));
        Walls.push_back(a);
        // wall 3
        a.setPosition(sf::Vector2f(250, 300));
        a.setSize(sf::Vector2f(150, 50));
        a.setFillColor(sf::Color(50, 50, 50));
        Walls.push_back(a);
        // wall 4
        a.setPosition(sf::Vector2f(0, 200));
        a.setSize(sf::Vector2f(200, 50));
        a.setFillColor(sf::Color(50, 50, 50));
        Walls.push_back(a);
        // wall 5
        a.setPosition(sf::Vector2f(350, 150));
        a.setSize(sf::Vector2f(150, 50));
        a.setFillColor(sf::Color(50, 50, 50));
        Walls.push_back(a);
        // wall 6
        a.setPosition(sf::Vector2f(600, 100));
        a.setSize(sf::Vector2f(200, 50));
        a.setFillColor(sf::Color(50, 50, 50));
        Walls.push_back(a);
    }
}

bool checkBoxCollision(sf::RectangleShape a, sf::RectangleShape b)
{
    // collision
    if (
        (a.getPosition().x < b.getPosition().x + b.getSize().x) &&
        (a.getPosition().x + a.getSize().x > b.getPosition().x) &&
        (a.getPosition().y < b.getPosition().y + b.getSize().y) &&
        (a.getPosition().y + a.getSize().y > b.getPosition().y)
        )
    {
        return true;
    }
    else
        return false;
}

void Physics() // this function made with the help of AI
{
    /*----- do x axis collisions and movement -----*/
    if (leftkey)  player.setPosition(sf::Vector2f(player.getPosition().x - speed, player.getPosition().y));
    if (rightkey) player.setPosition(sf::Vector2f(player.getPosition().x + speed, player.getPosition().y));

    // keep inside screen boundries
    if (player.getPosition().x < 0) player.setPosition(sf::Vector2f(0, player.getPosition().y));
    if (player.getPosition().x > screenw - player.getSize().x) player.setPosition(sf::Vector2f(screenw - player.getSize().x, player.getPosition().y));

    // loop through walls to check and push player back horizontally
    for (const auto& wall : Walls) {
        if (checkBoxCollision(player, wall)) {
            if (leftkey) {
                // move left into a wall, push right to the wall's right edge
                player.setPosition(sf::Vector2f(wall.getPosition().x + wall.getSize().x, player.getPosition().y));
            }
            if (rightkey) {
                // move right into a wall, push left to the wall's left edge
                player.setPosition(sf::Vector2f(wall.getPosition().x - player.getSize().x, player.getPosition().y));
            }
        }
    }
    /*----- resolve y axis collisions -----*/
    // do gravity if not grounded
    if (!grounded) {
        playervely += gravity;
    }
    // act on velocity
    player.setPosition(sf::Vector2f(player.getPosition().x, player.getPosition().y + playervely));

    // Apply strict screen floor boundary
    if (player.getPosition().y > screenh - player.getSize().y) {
        player.setPosition(sf::Vector2f(player.getPosition().x, screenh - player.getSize().y));
        playervely = 0;
        grounded = true; // if on or below bottom of screen, will be grounded
    }
    if (player.getPosition().y < 0) {
        player.setPosition(sf::Vector2f(player.getPosition().x, 0));
        playervely = 0;
    }

    // set grounded to true if player is touching bottom of screen, else it is false
    grounded = (player.getPosition().y == screenh - player.getSize().y);

    // loop through Walls to check collisions vertically
    for (const auto& wall : Walls) {
        if (checkBoxCollision(player, wall)) {
            if (playervely > 0.0f) {
                // falling down onto a platform
                player.setPosition(sf::Vector2f(player.getPosition().x, wall.getPosition().y - player.getSize().y)); // Snap to top of tile
                playervely = 0.0f;                                  // reset velocity
                grounded = true;                                    // set grounded
            }
            else if (playervely < 0.0f) {
                // moving up (hitting head on ceiling)
                player.setPosition(sf::Vector2f(player.getPosition().x, wall.getPosition().y + wall.getSize().y)); // Snap underneath tile
                playervely = 0.0f;                                  // Stop upward momentum
            }
        }
    }
    /*----- do y movement -----*/
    if (grounded && upkey) {
        playervely = -jumpForce;
        grounded = false;
    }
}
void DrawPlayer()
{
    // update player sprite
    lefteye.setPosition(sf::Vector2f(player.getPosition().x + 2.5, player.getPosition().y + 10));
    righteye.setPosition(sf::Vector2f(player.getPosition().x + (50 - 15 - 2.5), player.getPosition().y + 10));
}
void Logic()
{
    if (menu) {
        
    }
    else {
        Physics();
        DrawPlayer();
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
    player.setFillColor(sf::Color(255, 50, 50));
    player.setSize(sf::Vector2f(50, 50));

	// left eye
    lefteye.setFillColor(sf::Color(0, 0, 0));
    lefteye.setSize(sf::Vector2f(15, 20));
    //lefteye.setPosition(sf::Vector2f(playerx - 10, playery));

    // right eye
    righteye.setFillColor(sf::Color(0, 0, 0));
    righteye.setSize(sf::Vector2f(15, 20));
    //righteye.setPosition(sf::Vector2f(playerx + 10, playery));

    portal.setFillColor(sf::Color::Green);
    portal.setSize(sf::Vector2f(50, 100));
    portal.setPosition(sf::Vector2f(750, 0));

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
		Reset();
    });

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            // update ui events
            menuGui.handleEvent(*event);
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                upkey = true;
            else 
                upkey = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                leftkey = true;
            else
                leftkey = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                downkey = true;
            else 
                downkey = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
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
        if (menu)
        {
            window.clear(sf::Color(0, 0, 0));
        }
        else
        {
            window.clear(sf::Color(100, 190, 230));
        }

        // draw sfml before tgui
        if (menu)
        {
            
        }
        else
        {
            // scene
            for (int i = 0; i < Walls.size(); i++)
            {
                window.draw(Walls[i]);
            }
            window.draw(portal);

            // player
            window.draw(player);
            window.draw(lefteye);
            window.draw(righteye);
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
        //std::cout << "Grounded: " << grounded << std::endl;
        //std::cout << "Input Keys. W: " << upkey << " A: " << leftkey << " S: " << downkey << " D:" << rightkey << std::endl;
    }

    return 0;
}

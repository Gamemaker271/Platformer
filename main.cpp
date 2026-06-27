#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include <vector>

const int screenw = 800;
const int screenh = 600;

bool menu = true;


float playerx = 0;
float playery = 0;
float playervely = 0;

bool grounded = false;

float speed = 15;
float gravity = 2;
float jumpForce = 25;

sf::RectangleShape player;
sf::RectangleShape lefteye;
sf::RectangleShape righteye;

std::vector<sf::RectangleShape> Walls;

bool upkey = false;
bool downkey = false;
bool leftkey = false;
bool rightkey = false;

void Reset()
{
    playerx = 100;
	playery = screenh - player.getSize().y;
    Walls.clear();
    if (true) {
        sf::RectangleShape a;
        // wall 1
        a.setSize(sf::Vector2f(100, 100));
        a.setPosition(sf::Vector2f(300, 500));
        a.setFillColor(sf::Color(50, 50, 50));
        Walls.push_back(a);

        // wall 2
        a.setSize(sf::Vector2f(100, 100));
        a.setPosition(sf::Vector2f(600, 500));
        a.setFillColor(sf::Color(50, 50, 50));
        Walls.push_back(a);
    }
}

bool CheckGrounded()
{
    // collision
    if (
        (playerx < Walls[0].getPosition().x + Walls[0].getSize().x) && (playerx + player.getSize().x > Walls[0].getPosition().x) &&
        (playery < Walls[0].getPosition().y + Walls[0].getSize().y) && (playery + player.getSize().y > Walls[0].getPosition().y)
        )
    {
        return true;
    }
    else
        return false;
}
bool CheckCollide()
{
    // collision
    if (
        (playerx < Walls[0].getPosition().x + Walls[0].getSize().x) && (playerx + player.getSize().x > Walls[0].getPosition().x) &&
        (playery < Walls[0].getPosition().y + Walls[0].getSize().y) && (playery + player.getSize().y > Walls[0].getPosition().y)
        )
    {
        return true;
    }
    else
        return false;
}

void Physics()
{
    // calculate grounded
    if (playery == screenh - player.getSize().y)
        grounded = true;
    else
        grounded = false;

    // jumping
    if (grounded)
    {
        playervely = 0;
        if (upkey)
        {
            playervely = -jumpForce;
        }
    }
    else
        playervely += gravity;

    // left and right
    if (leftkey)
    {
        playerx -= speed;
        if (CheckCollide()) {
            playerx += speed;
        }
    }
    if (rightkey)
    {
        playerx += speed;
        if (CheckCollide()) {
            playerx -= speed;
        }
    }

    // add velocity
    playery += playervely;
    if (CheckCollide()) {
        playery -= playervely;
    }

    // keep player in bounds
    if (playerx < 0)
        playerx = 0;
    if (playerx > screenw - player.getSize().x)
        playerx = screenw - player.getSize().x;
    if (playery < 0)
        playery = 0;
    if (playery > screenh - player.getSize().y)
        playery = screenh - player.getSize().y;
}
void DrawPlayer()
{
    // update player sprite
    player.setPosition(sf::Vector2f(playerx, playery));
    lefteye.setPosition(sf::Vector2f(playerx + 2.5, playery + 10));
    righteye.setPosition(sf::Vector2f(playerx + (50 - 15 - 2.5), playery + 10));
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
    player.setPosition(sf::Vector2f( playerx, playery ));

	// left eye
    lefteye.setFillColor(sf::Color(0, 0, 0));
    lefteye.setSize(sf::Vector2f(15, 20));
    lefteye.setPosition(sf::Vector2f(playerx - 10, playery));

    // right eye
    righteye.setFillColor(sf::Color(0, 0, 0));
    righteye.setSize(sf::Vector2f(15, 20));
    righteye.setPosition(sf::Vector2f(playerx + 10, playery));


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
            for (int i = 0; i < Walls.size(); i++)
            {
                window.draw(Walls[i]);
            }
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
        //std::cout << "Input Keys. W: " << upkey << " A: " << leftkey << " S: " << downkey << " D:" << rightkey << std::endl;
    }

    return 0;
}

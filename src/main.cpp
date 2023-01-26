#include <SFML/Graphics.hpp>
#include <polygon.h>
#include <SAT.h>

const int N_POINTS_RED = 5, N_POINTS_BLUE = 6;
const float MOVE_SPEED = 250.0f;
const float ROTATE_SPEED = 80.0f;


int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Separating Axis Theorem");    

    sf::ConvexShape red(N_POINTS_RED), blue(N_POINTS_BLUE);

    CreateRegularPolygon(red, 100.0f);
    CreateRegularPolygon(blue, 100.0f);

    red.setOutlineColor(sf::Color::Red);
    red.setOutlineThickness(5.0f);
    red.setFillColor(sf::Color::Transparent);
    red.setPosition(sf::Vector2f(300.0f, 300.0f));
    
    blue.setOutlineColor(sf::Color::Blue);
    blue.setOutlineThickness(5.0f);
    blue.setFillColor(sf::Color::Transparent);
    blue.setPosition(sf::Vector2f(800.0f, 300.0f));

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        float dt = deltaClock.restart().asSeconds();

        sf::Vector2f redPosition = red.getPosition();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            redPosition.y -= MOVE_SPEED * dt;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            redPosition.y += MOVE_SPEED * dt;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            redPosition.x -= MOVE_SPEED * dt;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            redPosition.x += MOVE_SPEED * dt;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            red.rotate(-ROTATE_SPEED * dt);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            red.rotate(ROTATE_SPEED * dt);
        }

        red.setPosition(redPosition);

        if(SATPolygonCollision(red, blue, window))
            printf("Colliding");

        window.draw(red);
        window.draw(blue);
        window.display();
    }

    return 0;
}
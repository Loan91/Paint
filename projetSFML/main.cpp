#include <SFML/Graphics.hpp>
#include <vector>

int main() 
{
    const int windowWidth = 800;
    const int windowHeight = 600;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Paint");
    window.setFramerateLimit(60);

    sf::RenderTexture canvas;
    canvas.create(windowWidth, windowHeight);
    canvas.clear(sf::Color::White);

    sf::Sprite canvasSprite(canvas.getTexture());

    sf::Color brushColor = sf::Color::Black;
    float brushRadius = 5.0f;

    bool isDrawing = false;

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
            {
                isDrawing = true;
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) 
            {
                isDrawing = false;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
            {
                brushRadius = 50.0f;
            }

            if (event.type == sf::Event::KeyPressed) 
            {
                if (event.key.code == sf::Keyboard::R) brushColor = sf::Color::Red;
                if (event.key.code == sf::Keyboard::G) brushColor = sf::Color::Green;
                if (event.key.code == sf::Keyboard::B) brushColor = sf::Color::Blue;
                if (event.key.code == sf::Keyboard::Y) brushColor = sf::Color::Yellow;
                if (event.key.code == sf::Keyboard::M) brushColor = sf::Color::Magenta;
                if (event.key.code == sf::Keyboard::C) brushColor = sf::Color::Cyan;
                if (event.key.code == sf::Keyboard::W) brushColor = sf::Color::White;
                if (event.key.code == sf::Keyboard::N) brushColor = sf::Color::Black;
                if (event.key.code == sf::Keyboard::P) brushColor = sf::Color::Color(187, 224, 230);
                if (event.key.code == sf::Keyboard::V) brushColor = sf::Color::Color(199, 21, 133);
                if (event.key.code == sf::Keyboard::E) canvas.clear(sf::Color::White);
                if (event.key.code == sf::Keyboard::Up) brushRadius += 1.0f;
                if (event.key.code == sf::Keyboard::Down && brushRadius > 1.0f) brushRadius -= 1.0f;
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
        }

        if (isDrawing) 
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::CircleShape brush(brushRadius);
            brush.setFillColor(brushColor);
            brush.setPosition(mousePos.x - brushRadius, mousePos.y - brushRadius);
            canvas.draw(brush);
        }

        canvas.display();
        window.clear(sf::Color::White);
        window.draw(canvasSprite);
        window.display();
    }

    return 0;
}
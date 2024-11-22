#include <SFML/Graphics.hpp>
#include <vector>

int main()
{
    const int windowWidth = 800;
    const int windowHeight = 600;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Paint");
    window.setFramerateLimit(60);

    sf::RenderTexture canvas;
    canvas.create(windowWidth, windowHeight);
    canvas.clear(sf::Color::White);

    sf::Sprite canvasSprite(canvas.getTexture());

    sf::Color brushColor = sf::Color::Black;
    float brushRadius = 10.0f;

    bool isDrawing = false;

    const int buttonSize = 50;
    std::vector<std::pair<sf::RectangleShape, sf::Color>> colorButtons;

    std::vector<sf::Color> colors
    {
        sf::Color::Black, sf::Color::Red, sf::Color::Green, sf::Color::Blue,
        sf::Color::Yellow, sf::Color::Cyan, sf::Color::Magenta,
        sf::Color::Color(176, 224, 230), sf::Color::Color(199, 21, 133)
    };

    for (size_t i = 0; i < colors.size(); ++i)
    {
        sf::RectangleShape button(sf::Vector2f(buttonSize, buttonSize));
        button.setFillColor(colors[i]);
        button.setPosition(10 + i * (buttonSize + 5), 10);
        colorButtons.push_back(std::pair<sf::RectangleShape, sf::Color>(button, colors[i]));
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    for (auto& pair : colorButtons)
                    {
                        sf::RectangleShape& button = pair.first;
                        sf::Color& color = pair.second;

                        if (button.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        {
                            brushColor = color;
                        }
                    }

                    isDrawing = true;
                }
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                isDrawing = false;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
            {
                brushRadius = 70.0f;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::C) canvas.clear(sf::Color::White);
                if (event.key.code == sf::Keyboard::W) brushColor = sf::Color::White;
                if (event.key.code == sf::Keyboard::R) brushRadius = 10.0f;
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

        for (const auto& pair : colorButtons)
        {
            const sf::RectangleShape& button = pair.first;
            const sf::Color& color = pair.second;
            window.draw(button);
        }

        window.display();
    }

    return 0;
}
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

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
    bool isTyping = false;
    std::string inputText;
    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf"))
        return -1;
    sf::Text text(inputText, font, 20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(50, 100);

    sf::Vector2f textPosition = text.getPosition();

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

    sf::Texture imageTexture;
    sf::Sprite imageSprite;
    bool imageLoaded = false;

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

                    isDrawing = !isTyping;
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
                if (event.key.code == sf::Keyboard::A) brushRadius = 10.0f;
                if (event.key.code == sf::Keyboard::Up) brushRadius += 1.0f;
                if (event.key.code == sf::Keyboard::Down && brushRadius > 1.0f) brushRadius -= 1.0f;
                if (event.key.code == sf::Keyboard::F && isTyping) isTyping = false;
                if (event.key.code == sf::Keyboard::Escape) window.close();
                if (event.key.code == sf::Keyboard::T) isTyping = true;

                if (isTyping)
                {
                    if (event.key.code == sf::Keyboard::Left) textPosition.x -= 5.0f;
                    if (event.key.code == sf::Keyboard::Right) textPosition.x += 5.0f;
                    if (event.key.code == sf::Keyboard::Up) textPosition.y -= 5.0f;
                    if (event.key.code == sf::Keyboard::Down) textPosition.y += 5.0f;
                    text.setPosition(textPosition);
                }

                if (event.key.code == sf::Keyboard::I)
                {
                    if (imageTexture.loadFromFile("assets/Arcane.png"))
                    {
                        imageSprite.setTexture(imageTexture);
                        imageSprite.setPosition(100, 100);
                        imageSprite.setScale(1.0f, 1.0f);
                        imageLoaded = true;
                    }
                }
                if (imageLoaded)
                {
                    if (event.key.code == sf::Keyboard::R) imageSprite.rotate(-5);
                    if (event.key.code == sf::Keyboard::L) imageSprite.rotate(5);
                    if (event.key.code == sf::Keyboard::Q) imageSprite.move(-5, 0);
                    if (event.key.code == sf::Keyboard::D) imageSprite.move(5, 0);
                    if (event.key.code == sf::Keyboard::Z) imageSprite.move(0, -5);
                    if (event.key.code == sf::Keyboard::S) imageSprite.move(0, 5);
                    if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) imageSprite.scale(1.1f, 1.1f);
                    if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Dash) imageSprite.scale(0.9f, 0.9f);
                    if (event.key.code == sf::Keyboard::Delete) imageLoaded = false;
                }
            }

            if (event.type == sf::Event::TextEntered && isTyping)
            {
                if (event.text.unicode == '\b' && !inputText.empty())
                {
                    inputText.pop_back();
                }
                else if (event.text.unicode == '\r')
                {
                    isTyping = false;
                    sf::Text newText(inputText, font, 20);
                    newText.setFillColor(sf::Color::Black);
                    newText.setPosition(textPosition);
                    canvas.draw(newText);
                    inputText.clear();
                }
                else if (event.text.unicode < 128)
                {
                    inputText += static_cast<char>(event.text.unicode);
                }
                text.setString(inputText);
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

        if (imageLoaded)
        {
            window.draw(imageSprite);
        }

        if (isTyping)
        {
            window.draw(text);
        }

        window.display();
    }

    return 0;
}

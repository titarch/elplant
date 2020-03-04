#include <SFML/Graphics.hpp>
#include "grammar/Grammar.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

     /* Grammar test
      *
    Grammar g("FX", 30);
    g.add_rule('Y', "-FX-Y");
    g.add_rule('X', "X+YF+");
    std::cout << g.generate(3) << std::endl;
      */

    return 0;
}

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "tlkGL.hh"

int
main(int argc, char* argv[])
{
    std::locale locale;
    sf::String title(L"Trillek m1 test");
    sf::Window mainWin(sf::VideoMode(800,600), title);

    mainWin.setFramerateLimit(24);

    while (mainWin.isOpen())
    {
        sf::Event event;
        while (mainWin.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                mainWin.close();
            }
        }
        mainWin.setActive();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainWin.display();
    }
}



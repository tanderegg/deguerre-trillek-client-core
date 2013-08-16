#include "graphics_driver.hh"
#include "input_driver.hh"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>
#include <iostream>

namespace trillek { namespace detail {

struct sfml_device {
    sf::Window mMainWin;
    sf::Vector2u mWinSize;
    sf::Vector2i mWinCentre;

    bool mInputActive;
    bool mMouseActive;
    bool mMouseInWindow;

    int mMouseX;
    int mMouseY;

    void open_window()
    {
        mMainWin.create(sf::VideoMode(800,600),
            L"Trillek m1 test", sf::Style::Titlebar);
        mWinSize = mMainWin.getSize();
        mWinCentre.x = mWinSize.x / 2;
        mWinCentre.y = mWinSize.y / 2;
        dispatch_events();
    }

    void close_window()
    {
        if (mMainWin.isOpen())
        {
            mMainWin.close();
        }
    }

    void force_mouse_location()
    {
        sf::Mouse::setPosition(mWinCentre, mMainWin);
        mMouseX = mWinCentre.x;
        mMouseY = mWinCentre.y;
    }

    void dispatch_events();

    void activate_for_gl()
    {
        mMainWin.setActive();
    }

    void swap_buffers()
    {
        mMainWin.display();
    }

    void activate_mouse()
    {
        mMouseActive = true;
        mMainWin.setMouseCursorVisible(false);

        sf::Vector2i pos = sf::Mouse::getPosition(mMainWin);
        mMouseInWindow
            = pos.x >= 0 && pos.x < mWinSize.x
            && pos.y >= 0 && pos.y < mWinSize.y;
        if (mMouseInWindow)
        {
            mMouseX = pos.x;
            mMouseY = pos.y;
        }
        force_mouse_location();
    }

    void deactivate_mouse()
    {
        if (mMouseActive)
        {
            mMainWin.setMouseCursorVisible(true);
            mMouseActive = false;
        }
    }

    void activate_input()
    {
        mInputActive = true;
    }
    
    void deactivate_input()
    {
        if (mInputActive)
        {
            mInputActive = false;
        }
    }
    
    sfml_device()
    {
        mInputActive = false;
        mMouseActive = false;
        mMouseInWindow = false;

        mMouseX = 0;
        mMouseY = 0;
    }
};


void
sfml_device::dispatch_events()
{
    sf::Event event;
    while (mMainWin.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed:
            {
                std::cerr << "Key down: " << (unsigned)event.key.code << '\n';
                if (event.key.code == sf::Keyboard::Escape)
                {
                    std::exit(0);
                }
                break;
            }

            case sf::Event::KeyReleased:
            {
                std::cerr << "Key up: " << (unsigned)event.key.code << '\n';
                break;
            }

            case sf::Event::MouseMoved:
            {
                if (!mMouseActive)
                {
                    break;
	        }

                int dx = event.mouseMove.x - mMouseX;
                int dy = event.mouseMove.y - mMouseY;

                if (dx || dy)
                {
                    mMouseX = event.mouseMove.x;
                    mMouseY = event.mouseMove.y;
                    std::cerr << "Mouse move (" << dx << ", " << dy << ", 0)\n";
                    force_mouse_location();
                }
                break;
            }

            case sf::Event::MouseWheelMoved:
            {
                if (!mMouseActive)
                {
                    break;
	        }
                int dz = event.mouseWheel.delta;
                std::cerr << "Mouse move (0, 0, " << dz << ")\n";
                // Post mouse event here
                break;
            }

            case sf::Event::MouseEntered:
            {
                if (!mMouseActive)
                {
                    break;
	        }
                sf::Vector2i pos = sf::Mouse::getPosition(mMainWin);
                mMouseX = pos.x;
                mMouseY = pos.y;
                mMouseInWindow = true;
                force_mouse_location();
                break;
            }

            case sf::Event::MouseLeft:
            {
                if (!mMouseActive)
                {
                    break;
	        }
                mMouseInWindow = false;
                force_mouse_location();
                break;
            }

            default:
            {
                break;
            }
        }
    }
}


static sfml_device sSfmlDevice;


struct sfml_graphics_driver : public graphics_driver {
    // From subsystem
    void init()
    {
        sSfmlDevice.open_window();
    }

    void pre_shutdown()
    {
    }

    void shutdown()
    {
        sSfmlDevice.close_window();
    }


    void begin_rendering()
    {
        sSfmlDevice.dispatch_events();
        sSfmlDevice.activate_for_gl();
    }

    void end_rendering()
    {
    }

    void swap_buffers()
    {
        sSfmlDevice.swap_buffers();
    }

};

static sfml_graphics_driver sGraphicsDriver;


struct sfml_input_driver : public input_driver {

    void init()
    {
         sSfmlDevice.activate_mouse();
         sSfmlDevice.activate_input();
    }

    void pre_shutdown()
    {
         sSfmlDevice.deactivate_input();
         sSfmlDevice.deactivate_mouse();
    }

    void shutdown()
    {
    }

    void capture_mouse(bool pCapture)
    {
        if (pCapture)
        {
            sSfmlDevice.activate_mouse();
        }
        else
        {
            sSfmlDevice.deactivate_mouse();
        }
    }
};

static sfml_input_driver sInputDriver;

} }


namespace trillek {
    graphics_driver& get_graphics_driver()
    {
        return detail::sGraphicsDriver;
    }

    input_driver& get_input_driver()
    {
        return detail::sInputDriver;
    }
}



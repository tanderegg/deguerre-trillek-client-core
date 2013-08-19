#include "graphics_driver.hh"
#include "input_driver.hh"
#include "keycodes.hh"

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
        mMainWin.setKeyRepeatEnabled(false);
        mWinCentre.x = mWinSize.x / 2;
        mWinCentre.y = mWinSize.y / 2;
        dispatch_events();
    }

    void close_window()
    {
        if (mMainWin.isOpen()) {
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
        if (mMouseInWindow) {
            mMouseX = pos.x;
            mMouseY = pos.y;
        }
        force_mouse_location();
    }

    void deactivate_mouse()
    {
        if (mMouseActive) {
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
        if (mInputActive) {
            mInputActive = false;
        }
    }

    trillek::keycode_t
    translate_key(sf::Keyboard::Key pKey)
    {
        switch (pKey)
        {
        case sf::Keyboard::A:           return trillek::k('a');
        case sf::Keyboard::B:           return trillek::k('b');
        case sf::Keyboard::C:           return trillek::k('c');
        case sf::Keyboard::D:           return trillek::k('d');
        case sf::Keyboard::E:           return trillek::k('e');
        case sf::Keyboard::F:           return trillek::k('f');
        case sf::Keyboard::G:           return trillek::k('g');
        case sf::Keyboard::H:           return trillek::k('h');
        case sf::Keyboard::I:           return trillek::k('i');
        case sf::Keyboard::J:           return trillek::k('j');
        case sf::Keyboard::K:           return trillek::k('k');
        case sf::Keyboard::L:           return trillek::k('l');
        case sf::Keyboard::M:           return trillek::k('m');
        case sf::Keyboard::N:           return trillek::k('n');
        case sf::Keyboard::O:           return trillek::k('o');
        case sf::Keyboard::P:           return trillek::k('p');
        case sf::Keyboard::Q:           return trillek::k('q');
        case sf::Keyboard::R:           return trillek::k('r');
        case sf::Keyboard::S:           return trillek::k('s');
        case sf::Keyboard::T:           return trillek::k('t');
        case sf::Keyboard::U:           return trillek::k('u');
        case sf::Keyboard::V:           return trillek::k('v');
        case sf::Keyboard::W:           return trillek::k('w');
        case sf::Keyboard::X:           return trillek::k('x');
        case sf::Keyboard::Y:           return trillek::k('y');
        case sf::Keyboard::Z:           return trillek::k('z');
        case sf::Keyboard::Num0:        return trillek::k('0');
        case sf::Keyboard::Num1:        return trillek::k('1');
        case sf::Keyboard::Num2:        return trillek::k('2');
        case sf::Keyboard::Num3:        return trillek::k('3');
        case sf::Keyboard::Num4:        return trillek::k('4');
        case sf::Keyboard::Num5:        return trillek::k('5');
        case sf::Keyboard::Num6:        return trillek::k('6');
        case sf::Keyboard::Num7:        return trillek::k('7');
        case sf::Keyboard::Num8:        return trillek::k('8');
        case sf::Keyboard::Num9:        return trillek::k('9');
        case sf::Keyboard::Escape:      return K_ESCAPE;
        case sf::Keyboard::LControl:    return K_LCTRL;
        case sf::Keyboard::LShift:      return K_LSHIFT;
        case sf::Keyboard::LAlt:        return K_LALT;
        case sf::Keyboard::LSystem:     return K_LSYSTEM;
        case sf::Keyboard::RControl:    return K_RCTRL;
        case sf::Keyboard::RShift:      return K_RSHIFT;
        case sf::Keyboard::RAlt:        return K_RALT;
        case sf::Keyboard::RSystem:     return K_RSYSTEM;
        case sf::Keyboard::Menu:        return K_MENU;
        case sf::Keyboard::LBracket:    return trillek::k('[');
        case sf::Keyboard::RBracket:    return trillek::k(']');
        case sf::Keyboard::SemiColon:   return trillek::k(';');
        case sf::Keyboard::Comma:       return trillek::k(',');
        case sf::Keyboard::Period:      return trillek::k('.');
        case sf::Keyboard::Quote:       return trillek::k('\'');
        case sf::Keyboard::Slash:       return trillek::k('/');
        case sf::Keyboard::BackSlash:   return trillek::k('\\');
        case sf::Keyboard::Tilde:       return trillek::k('~');
        case sf::Keyboard::Equal:       return trillek::k('=');
        case sf::Keyboard::Dash:        return trillek::k('-');
        case sf::Keyboard::Space:       return K_SPACE;
        case sf::Keyboard::Return:      return K_ENTER;
        case sf::Keyboard::BackSpace:   return K_BACKSPACE;
        case sf::Keyboard::Tab:         return K_TAB;
        case sf::Keyboard::PageUp:      return K_PAGEUP;
        case sf::Keyboard::PageDown:    return K_PAGEDOWN;
        case sf::Keyboard::End:         return K_END;
        case sf::Keyboard::Home:        return K_HOME;
        case sf::Keyboard::Insert:      return K_INS;
        case sf::Keyboard::Delete:      return K_DEL;
        case sf::Keyboard::Add:         return trillek::k('+');
        case sf::Keyboard::Subtract:    return trillek::k('-');
        case sf::Keyboard::Multiply:    return trillek::k('*');
        case sf::Keyboard::Divide:      return trillek::k('/');
        case sf::Keyboard::Left:        return K_LEFT;
        case sf::Keyboard::Right:       return K_RIGHT;
        case sf::Keyboard::Up:          return K_UP;
        case sf::Keyboard::Down:        return K_DOWN;
        case sf::Keyboard::Numpad0:     return K_PAD0;
        case sf::Keyboard::Numpad1:     return K_PAD1;
        case sf::Keyboard::Numpad2:     return K_PAD2;
        case sf::Keyboard::Numpad3:     return K_PAD3;
        case sf::Keyboard::Numpad4:     return K_PAD4;
        case sf::Keyboard::Numpad5:     return K_PAD5;
        case sf::Keyboard::Numpad6:     return K_PAD6;
        case sf::Keyboard::Numpad7:     return K_PAD7;
        case sf::Keyboard::Numpad8:     return K_PAD8;
        case sf::Keyboard::Numpad9:     return K_PAD9;
        case sf::Keyboard::F1:          return K_F1;
        case sf::Keyboard::F2:          return K_F2;
        case sf::Keyboard::F3:          return K_F3;
        case sf::Keyboard::F4:          return K_F4;
        case sf::Keyboard::F5:          return K_F5;
        case sf::Keyboard::F6:          return K_F6;
        case sf::Keyboard::F7:          return K_F7;
        case sf::Keyboard::F8:          return K_F8;
        case sf::Keyboard::F9:          return K_F9;
        case sf::Keyboard::F10:         return K_F10;
        case sf::Keyboard::F11:         return K_F11;
        case sf::Keyboard::F12:         return K_F12;
        case sf::Keyboard::F13:         return K_F13;
        case sf::Keyboard::F14:         return K_F14;
        case sf::Keyboard::F15:         return K_F15;
        case sf::Keyboard::Pause:       return K_PAUSE;
        default:                        return K_UNKNOWN;
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
    while (mMainWin.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::KeyPressed:
            {
                trillek::keycode_t key = translate_key(event.key.code);
                std::cerr << "Key down: " << key << '\n';
                if (key == K_ESCAPE)
                {
                    std::exit(0);
                }
                break;
            }

            case sf::Event::KeyReleased:
            {
                trillek::keycode_t key = translate_key(event.key.code);
                std::cerr << "Key up: " << key << '\n';
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



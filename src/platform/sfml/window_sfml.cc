#include <window_sfml.hh>
#include <window_target_sfml.hh>
#include <graphics_device_gl.hh>

namespace {
    static constexpr uint32_t s_width = 800;
    static constexpr uint32_t s_height = 600;
}

namespace trillek {

window_sfml::window_sfml(system_event_queue& pQueue)
        : mQueue(pQueue) {
    mInputActive = false;
    mMouseActive = false;
    mMouseInWindow = false;

    mMouseX = 0;
    mMouseY = 0;
}


window_sfml::~window_sfml() {
}

std::shared_ptr<window_target>
window_sfml::make_window_target(
        const std::shared_ptr<graphics_device>& pDevice) {
    auto device = std::static_pointer_cast<graphics_device_gl>(pDevice);
    return std::make_shared<window_target_sfml>(
        shared_from_this(), device
    );
}

void
window_sfml::get_dimensions(uint32_t& pWidth, uint32_t& pHeight) const
{
    pWidth = mWinSize.x; 
    pHeight = mWinSize.y; 
}

void
window_sfml::get_config(graphics_config_t& pConfig) const
{
    pConfig.mMajorVersion = mContextSettings.majorVersion;
    pConfig.mMinorVersion = mContextSettings.minorVersion;
    pConfig.mDepthBits = mContextSettings.depthBits;
    pConfig.mStencilBits = mContextSettings.stencilBits;
    pConfig.mAntialiasingLevel = mContextSettings.antialiasingLevel;
}

void
window_sfml::open_window() {
    sf::ContextSettings ctx(32, 8, 0, 3, 0);
    mMainWin.create(sf::VideoMode(s_width,s_height),
        L"Trillek m1 test", sf::Style::Titlebar, ctx);
    mWinSize = mMainWin.getSize();
    mContextSettings = mMainWin.getSettings();
    mMainWin.setFramerateLimit(30);
    mMainWin.setKeyRepeatEnabled(false);
    mWinCentre.x = mWinSize.x / 2;
    mWinCentre.y = mWinSize.y / 2;
    dispatch_events();
}

void
window_sfml::close_window() {
    if (mMainWin.isOpen()) {
        mMainWin.close();
    }
}

void
window_sfml::force_mouse_location() {
    sf::Mouse::setPosition(mWinCentre, mMainWin);
    mMouseX = mWinCentre.x;
    mMouseY = mWinCentre.y;
}


void
window_sfml::activate_for_gl() {
    mMainWin.setActive();
}

void
window_sfml::swap_buffers() {
    mMainWin.display();
}

void
window_sfml::activate_mouse() {
    mMouseActive = true;
    mMainWin.setMouseCursorVisible(false);

    sf::Vector2i pos = sf::Mouse::getPosition(mMainWin);
    mMouseInWindow
        = pos.x >= 0 && pos.x < (int)mWinSize.x
        && pos.y >= 0 && pos.y < (int)mWinSize.y;
    if (mMouseInWindow) {
        mMouseX = pos.x;
        mMouseY = pos.y;
    }
    force_mouse_location();
}

void
window_sfml::deactivate_mouse() {
    if (mMouseActive) {
        mMainWin.setMouseCursorVisible(true);
        mMouseActive = false;
    }
}

void
window_sfml::activate_input() {
    mInputActive = true;
}

void
window_sfml::deactivate_input() {
    if (mInputActive) {
        mInputActive = false;
    }
}

trillek::keycode_t
window_sfml::translate_key(sf::Keyboard::Key pKey) {
    switch (pKey) {
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


void
window_sfml::dispatch_events()
{
    sf::Event event;
    while (mMainWin.pollEvent(event)) {
        if (!mInputActive) {
            continue;
        }

        switch (event.type) {
            case sf::Event::KeyPressed:
            {
                trillek::keycode_t key = translate_key(event.key.code);
                mQueue.push(system_event_t(EV_KEY, EV_K_DOWN, key));
                break;
            }

            case sf::Event::KeyReleased:
            {
                trillek::keycode_t key = translate_key(event.key.code);
                mQueue.push(system_event_t(EV_KEY, EV_K_UP, key));
                break;
            }

            case sf::Event::MouseMoved:
            {
                if (!mMouseActive) {
                    break;
                }

                int dx = event.mouseMove.x - mMouseX;
                int dy = event.mouseMove.y - mMouseY;

                if (!dx && !dy) {
                    break;
                }
                mMouseX = event.mouseMove.x;
                mMouseY = event.mouseMove.y;
                force_mouse_location();
                if (dx) {
                    mQueue.push(system_event_t(EV_MOUSE, EV_M_DX, dx));
                }
                if (dy) {
                    mQueue.push(system_event_t(EV_MOUSE, EV_M_DY, dy));
                }
                break;
            }

            case sf::Event::MouseWheelMoved: {
                if (!mMouseActive) {
                    break;
                }
                int dz = event.mouseWheel.delta;
                if (dz) {
                    mQueue.push(system_event_t(EV_MOUSE, EV_M_DZ, dz));
                }
                break;
            }

            case sf::Event::MouseEntered: {
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

            case sf::Event::MouseLeft: {
                if (!mMouseActive) {
                    break;
                }
                mMouseInWindow = false;
                force_mouse_location();
                break;
            }

            default: {
                break;
            }
        }
    }
}



}

#ifndef KEYCODES_HH_INCLUDED
#define KEYCODES_HH_INCLUDED

namespace trillek {
    enum keycode_t {
        K_UNKNOWN = 0,

        // If the key has an ASCII code, use that. Please map
        // letters to lower case.

        K_TAB = 9,
        K_ENTER = 13,
        K_ESCAPE = 27,
        K_SPACE = 32,
        K_BACKSPACE = 127,

        // For other keys, use code 128 and above.

        K_LCTRL = 128,
        K_LSHIFT,
        K_LALT,
        K_LSYSTEM,
        K_RCTRL,
        K_RSHIFT,
        K_RALT,
        K_RSYSTEM,
        K_MENU,

        K_INS,
        K_DEL,
        K_HOME,
        K_END,
        K_PAGEUP,
        K_PAGEDOWN,

        K_LEFT,
        K_RIGHT,
        K_UP,
        K_DOWN,

        K_PAUSE,

        K_PADPLUS,
        K_PADMINUS,
        K_PADSTAR,
        K_PADSLASH,
        K_PADDOT,
        K_PAD0,
        K_PAD1,
        K_PAD2,
        K_PAD3,
        K_PAD4,
        K_PAD5,
        K_PAD6,
        K_PAD7,
        K_PAD8,
        K_PAD9,

        K_F1,
        K_F2,
        K_F3,
        K_F4,
        K_F5,
        K_F6,
        K_F7,
        K_F8,
        K_F9,
        K_F10,
        K_F11,
        K_F12,
        K_F13,
        K_F14,
        K_F15,

        K_LAST
    };

    constexpr keycode_t k(char pC) { return static_cast<keycode_t>(pC); }
}

#endif

#ifndef WINDOW_SFML_HH_INCLUDED
#define WINDOW_SFML_HH_INCLUDED

#include <platform_sfml.hh>
#include <window.hh>
#include <keycodes.hh>

namespace trillek {

    class window_manager_sfml;
    class window_target_sfml;

    class window_sfml : public window,
            public std::enable_shared_from_this<window_sfml> {
    public:
        window_sfml(system_event_queue& pQueue);

        ~window_sfml();

        virtual std::shared_ptr<window_target> make_window_target(
            const std::shared_ptr<graphics_device>& pDevice
        );

        void get_dimensions(uint32_t& pWidth, uint32_t& pHeight) const;

        virtual void get_config(graphics_config_t& pConfig) const;

        void open_window();

        void close_window();

        void activate_for_gl();

        void swap_buffers();

        void dispatch_events();

        void activate_mouse();

        void deactivate_mouse();

        void activate_input();

        void deactivate_input();

        trillek::keycode_t translate_key(sf::Keyboard::Key pKey);

    private:
        friend class window_manager_sfml;

        void force_mouse_location();

        sf::Window mMainWin;
        sf::Vector2u mWinSize;
        sf::Vector2i mWinCentre;
        sf::ContextSettings mContextSettings;

        system_event_queue& mQueue;

        bool mInputActive;
        bool mMouseActive;
        bool mMouseInWindow;

        int mMouseX;
        int mMouseY;
    };


}

#endif // WINDOW_SFML_HH_INCLUDED

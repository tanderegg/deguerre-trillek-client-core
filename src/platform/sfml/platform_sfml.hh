#ifndef PLATFORM_SFML_HH_INCLUDED
#define PLATFORM_SFML_HH_INCLUDED

#include <utils.hh>
#include <platform_subsystem.hh>
#include <system_event.hh>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

namespace trillek {

class window_manager_sfml;
class graphics_device_gl;

class platform_subsystem_sfml : public platform_subsystem {
private:
    std::unique_ptr<window_manager_sfml> mWindowManager;
    std::shared_ptr<graphics_device_gl> mDeviceGL;

    bool mEventsActive;
    system_event_queue* mQueue;

public:
    interface_key_t implements() const {
        return platform_subsystem::s_interface;
    }

    virtual window_manager& get_window_manager();

    platform_subsystem_sfml();

    void pre_init();

    void init(const subsystem_manager& pMgr);

    void post_init();

    void pre_shutdown();

    void shutdown();

    void frame();
};

}

#endif // PLATFORM_SFML_HH_INCLUDED

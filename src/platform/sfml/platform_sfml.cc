#include <platform_sfml.hh>
#include <window_manager_sfml.hh>
#include <graphics_device_gl.hh>
#include <graphics_adapter.hh>
#include <graphics_subsystem.hh>

namespace trillek {

    namespace {
        static const char* sAdapterName = "SFML OpenGL Adapter";

        std::shared_ptr<graphics_device>
        sfml_gl_device_factory() {
            return std::make_shared<graphics_device_gl>();
        }
    }


window_manager&
platform_subsystem_sfml::get_window_manager() {
    return *mWindowManager;
}

platform_subsystem_sfml::platform_subsystem_sfml() {
    mEventsActive = false;
}

void
platform_subsystem_sfml::pre_init() {
    mWindowManager = std::unique_ptr<window_manager_sfml>(
        new window_manager_sfml()
    );
    mWindowManager->pre_init();
}

void
platform_subsystem_sfml::init(const subsystem_manager& pMgr) {
    graphics_subsystem& graphicsSubsystem
        = pMgr.lookup<graphics_subsystem>();

    // Set up the platform adapter.
    std::shared_ptr<graphics_adapter> sfmlAdapter
        = std::make_shared<graphics_adapter>();
    std::strcpy(sfmlAdapter->mName, sAdapterName);
    sfmlAdapter->mDeviceFactory = sfml_gl_device_factory;
    graphicsSubsystem.register_adapter(std::move(sfmlAdapter));

    mWindowManager->init(pMgr);
}

void
platform_subsystem_sfml::post_init() {
    mWindowManager->post_init();
}

void
platform_subsystem_sfml::pre_shutdown() {
}

void
platform_subsystem_sfml::shutdown() {
    mWindowManager->shutdown();
}

void
platform_subsystem_sfml::frame() {
    mWindowManager->frame();
}


namespace detail {
    platform_subsystem_sfml
    sSfmlPlatform;
}

platform_subsystem& get_platform_subsystem()
{
    return detail::sSfmlPlatform;
}

}



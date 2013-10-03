#include <window_manager_sfml.hh>
#include <window_sfml.hh>

namespace trillek {

void
window_manager_sfml::pre_init()
{
}

std::shared_ptr<window>
window_manager_sfml::get_main_window()
{
    return mMainWindow;
}

void
window_manager_sfml::init(const subsystem_manager& pMgr) {
    system_event_queue& evqueue = pMgr.lookup<system_event_queue>();
    mQueue = &evqueue;
    mMainWindow = std::make_shared<window_sfml>(evqueue);
    mMainWindow->open_window();
    mMainWindow->activate_mouse();
    mMainWindow->activate_input();
}

void
window_manager_sfml::post_init()
{
}

void
window_manager_sfml::shutdown() {
    mMainWindow->close_window();
}

void
window_manager_sfml::frame() {
    mMainWindow->dispatch_events();
}

window_manager_sfml::window_manager_sfml()
{
}

window_manager_sfml::~window_manager_sfml()
{
}

}

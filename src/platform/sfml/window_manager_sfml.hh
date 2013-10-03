#ifndef WINDOW_MANAGER_SFML_HH_INCLUDED
#define WINDOW_MANAGER_SFML_HH_INCLUDED

#include <platform_sfml.hh>
#include <window_manager.hh>
#include <window_sfml.hh>

namespace trillek {

class window_manager_sfml : public window_manager {
public:

    void pre_init();

    std::shared_ptr<window> get_main_window();

    void init(const subsystem_manager& pMgr);

    void post_init();

    void shutdown();

    void frame();

    window_manager_sfml();

    ~window_manager_sfml();

private:
    std::shared_ptr<window_sfml> mMainWindow;
    system_event_queue* mQueue;
};

}

#endif // WINDOW_MANAGER_SFML_HH_INCLUDED

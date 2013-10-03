#ifndef WINDOW_MANAGER_HH_INCLUDED
#define WINDOW_MANAGER_HH_INCLUDED

#include <utils.hh>

namespace trillek {

    class window;

    class window_manager {
    public:
        virtual std::shared_ptr<window> get_main_window() = 0;

    protected:
        window_manager();
        ~window_manager();
    };

}


#endif // WINDOW_MANAGER_HH_INCLUDED

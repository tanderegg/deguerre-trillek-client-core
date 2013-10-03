#ifndef PLATFORM_SUBSYSTEM_HH_INCLUDED
#define PLATFORM_SUBSYSTEM_HH_INCLUDED

#include "subsystem.hh"

namespace trillek {

    class window_manager;

    class platform_subsystem : public subsystem
    {
    public:
        static constexpr interface_key_t s_interface = "PlatformSubsystem-1";

	virtual window_manager& get_window_manager() = 0;

        virtual void frame() = 0;

    protected:
        ~platform_subsystem();
    };

}

#endif // PLATFORM_SUBSYSTEM_HH_INCLUDED

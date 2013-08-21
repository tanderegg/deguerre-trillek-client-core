#ifndef INPUT_DRIVER_HH_INCLUDED
#define INPUT_DRIVER_HH_INCLUDED

#include "subsystem.hh"

namespace trillek {

    class system_event_queue;

    class input_driver : public subsystem
    {
    public:
        static constexpr interface_key_t s_interface = "InputDriver-1";

        virtual void capture_mouse(bool pCapture) = 0;
    };

}

#endif // INPUT_DRIVER_HH_INCLUDED

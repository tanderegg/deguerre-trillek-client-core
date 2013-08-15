#ifndef INPUT_DRIVER_HH_INCLUDED
#define INPUT_DRIVER_HH_INCLUDED

#include "subsystem.hh"

namespace trillek {

    class input_driver : public subsystem
    {
    public:
        virtual void capture_mouse(bool pCapture) = 0;
    };

}

#endif // INPUT_DRIVER_HH_INCLUDED

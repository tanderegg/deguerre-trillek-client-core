#ifndef GRAPHICS_DRIVER_HH_INCLUDED
#define GRAPHICS_DRIVER_HH_INCLUDED

#include "subsystem.hh"

namespace trillek {

    class graphics_driver : public subsystem
    {
    public:
        virtual void begin_rendering() = 0;
        virtual void end_rendering() = 0;
        virtual void swap_buffers() = 0;
    };

}

#endif // GRAPHICS_DRIVER_HH_INCLUDED

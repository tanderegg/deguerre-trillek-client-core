#ifndef SUBSYSTEM_HH_INCLUDED
#define SUBSYSTEM_HH_INCLUDED

namespace trillek {

    class subsystem
    {
    public:
        virtual void init() = 0;
        virtual void pre_shutdown() = 0;
        virtual void shutdown() = 0;
    };

}

#endif // SUBSYSTEM_HH_INCLUDED

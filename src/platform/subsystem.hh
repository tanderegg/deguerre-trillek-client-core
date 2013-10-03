#ifndef SUBSYSTEM_HH_INCLUDED
#define SUBSYSTEM_HH_INCLUDED

#include <utils.hh>

namespace trillek {

    class subsystem_manager;

    typedef const char* interface_key_t;

    class subsystem
    {
    public:
        virtual interface_key_t implements() const = 0;

        // init() is the call where subsystems can access other subsystems.
        // Anything which needs to be done before this (e.g. setting up
        // stuff which other subsystems may need) is done in pre_init().
        // post_init() is called after this, assuming that all subsystem
        // interdependencies are now set up.

        virtual void pre_init() = 0;
        virtual void init(const subsystem_manager& pMgr) = 0;
        virtual void post_init() = 0;

        // pre_shutdown is for subsystems to unhook from other subsystems.
        // By the time shutdown() is called, it's assumed that nobody will
        // call your subsystem ever again.

        virtual void pre_shutdown() = 0;
        virtual void shutdown() = 0;

    protected:
        virtual ~subsystem();
    };

    class subsystem_manager : public subsystem {
    public:
        static constexpr interface_key_t s_interface = "SubsystemManger-1";

        void initialise() {
            pre_init();
            init(*this);
            post_init();
        }

        virtual void load(interface_key_t pKey,
                          subsystem& pSubsystem) = 0;

        virtual subsystem* lookup(interface_key_t pKey) const = 0;

        template<class T> T&
        lookup() const {
            interface_key_t key = T::s_interface;
            subsystem* subsys = lookup(key);
            if (subsys) {
                return static_cast<T&>(*subsys);
            }
            else {
                throw std::exception();
            }
        }

    protected:
        ~subsystem_manager();
    };

    subsystem_manager& standard_subsystem_manager();
}

#endif // SUBSYSTEM_HH_INCLUDED

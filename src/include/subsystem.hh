#ifndef SUBSYSTEM_HH_INCLUDED
#define SUBSYSTEM_HH_INCLUDED

#include <cstdint>
#include <memory>
#include <exception>
#include <iostream> // XXX

namespace trillek {

    class subsystem_manager;

    typedef const char* interface_key_t;

    class subsystem
    {
    public:
        virtual interface_key_t implements() const = 0;
        virtual void init(const subsystem_manager& pMgr) = 0;
        virtual void pre_shutdown() = 0;
        virtual void shutdown() = 0;
    };

    class subsystem_manager : public subsystem {
    public:
        static constexpr interface_key_t s_interface = "SubsystemManger-1";

        void initialise() {
            init(*this);
        }

        virtual void load(interface_key_t pKey,
                          subsystem& pSubsystem) = 0;

        virtual subsystem* lookup(interface_key_t pKey) const = 0;

        template<class T> T&
        lookup() const {
            interface_key_t key = T::s_interface;
            std::cerr << "Looking up interface for " << key << '\n';
            subsystem* subsys = lookup(key);
            if (subsys) {
                return static_cast<T&>(*subsys);
            }
            else {
                throw std::exception();
            }
        }
    };

    subsystem_manager& standard_subsystem_manager();
}

#endif // SUBSYSTEM_HH_INCLUDED

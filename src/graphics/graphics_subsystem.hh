#ifndef GRAPHICS_SUBSYSTEM_HH_INCLUDED
#define GRAPHICS_SUBSYSTEM_HH_INCLUDED

#include <subsystem.hh>

namespace trillek {

    class graphics_device;
    class graphics_adapter;

    class graphics_subsystem : public subsystem
    {
    public:
        static constexpr interface_key_t s_interface = "GraphicsSubsystem-1";

        interface_key_t implements() const {
            return graphics_subsystem::s_interface;
        }

        void pre_init();

        void init(const subsystem_manager& pMgr);

        void post_init();

        void pre_shutdown();

        void shutdown();

        void register_adapter(std::shared_ptr<graphics_adapter> pAdapter);

        std::shared_ptr<graphics_device> create_device() const;

        static graphics_subsystem& get_graphics_subsystem();

    private:
        graphics_subsystem();
        ~graphics_subsystem();

        struct impl;
        std::unique_ptr<impl> mPImpl;
    };

}

#endif // GRAPHICS_SUBSYSTEM_HH_INCLUDED

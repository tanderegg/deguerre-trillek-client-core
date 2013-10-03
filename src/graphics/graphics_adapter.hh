#ifndef GRAPHICS_ADAPTER_HH_INCLUDED
#define GRAPHICS_ADAPTER_HH_INCLUDED

#include <graphics_device.hh>

namespace trillek {

    class graphics_adapter : private boost::noncopyable
    {
    public:
        static constexpr unsigned MAX_ADAPTER_NAME_LEN = 512;

        typedef std::function<std::shared_ptr<graphics_device> ()>
            graphics_device_factory;

        graphics_adapter()
        {
        }

        ~graphics_adapter()
        {
        }

        char mName[MAX_ADAPTER_NAME_LEN];
        graphics_device_factory mDeviceFactory;
    };

}

#endif // GRAPHICS_ADAPTER_HH_INCLUDED

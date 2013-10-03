#ifndef WINDOW_HH_INCLUDED
#define WINDOW_HH_INCLUDED

#include <utils.hh>

namespace trillek {

    class graphics_device;
    class window_target;

    // XXX Valid for OpenGL, not so much for D3D.
    struct graphics_config_t {
        uint32_t mMajorVersion;
        uint32_t mMinorVersion;
        uint32_t mDepthBits;
        uint32_t mStencilBits;
        uint32_t mAntialiasingLevel;
    };

    class window {
    public:
        virtual ~window();

        virtual std::shared_ptr<window_target> make_window_target(
            const std::shared_ptr<graphics_device>& pDevice
        ) = 0;

        virtual void get_dimensions(uint32_t& pWidth, uint32_t& pHeight) const = 0;

        virtual void get_config(graphics_config_t& pConfig) const = 0;

    protected:
        window();
    };

}

#endif // WINDOW_HH_INCLUDED

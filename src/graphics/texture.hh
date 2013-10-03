#ifndef TEXTURE_HH_INCLUDE
#define TEXTURE_HH_INCLUDE

#include <graphics_device.hh>

namespace trillek {

    class texture {
    public:
        virtual ~texture();

        virtual uint32_t width() const = 0;
        virtual uint32_t height() const = 0;
        virtual uint32_t depth() const = 0;
        virtual uint32_t mipmap_levels() const = 0;

    protected:
        texture();
    };

}


#endif // TEXTURE_HH_INCLUDE

#ifndef GRAPHICS_STATE_HH_INCLUDED
#define GRAPHICS_STATE_HH_INCLUDED

#include <graphics_constants.hh>

namespace trillek {

    enum {
        C_R = 0,
        C_G,
        C_B,
        C_A,
        C_COUNT
    };

    enum {
        D_ENABLE = 0,
        D_WENABLE,
        D_COUNT
    };

    struct graphics_state
    {
        struct color_state
        {
            std::bitset<C_COUNT> mFlags;

            color_state();
        };

        struct depth_state
        {
            std::bitset<D_COUNT> mFlags;

            cmp_func_t mDepthCmp;
            float_t mDepthBias;

            depth_state();
        };

        color_state mColor;
        depth_state mDepth;

        graphics_state();
    };

}


#endif // GRAPHICS_STATE_HH_INCLUDED

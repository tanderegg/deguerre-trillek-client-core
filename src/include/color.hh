#ifndef COLOR_HH_INCLUDED
#define COLOR_HH_INCLUDED

#include <utils.hh>

namespace trillek {

struct rgb_t {
    float_t r, g, b;

    rgb_t(float_t pR, float_t pG, float_t pB)
        : r(pR), g(pG), b(pB)
    {
    }
};

struct rgba_t {
    float_t r, g, b, a;

    rgba_t(float_t pR, float_t pG, float_t pB, float_t pA = 1.0f)
        : r(pR), g(pG), b(pB), a(pA)
    {
    }

    rgba_t(const rgb_t pRgb, float_t pA = 1.0f)
        : r(pRgb.r), g(pRgb.g), b(pRgb.b), a(pA)
    {
    }
};

}

#endif // COLOR_HH_INCLUDED

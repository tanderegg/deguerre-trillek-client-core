#ifndef MATHS_HH_INCLUDED
#define MATHS_HH_INCLUDED

#include <type_traits>
#include <cmath>
#include <complex>
#include <boost/math/quaternion.hpp>

#ifdef __SSE__
#include <xmmintrin.h>
#endif

namespace trillek {

typedef float float_t;


// Approximate reciprocal square root, using the Walsh method.
//
inline float_t
rsqrtf(float_t pX) {
#ifdef __SSE__
    float   r = pX;

    _mm_store_ss( & r, _mm_rsqrt_ss( _mm_load_ss( & r ) ) );

    return r;
#else
    float x2 = pX * 0.5f;
    float y  = pX;
    int32_t i  = *reinterpret_cast<int32_t *>(&y);
    i  = 0x5f3759df - (i >> 1);
    y  = *reinterpret_cast<float *>(&i);
    return y * (1.5f - x2*y*y);
#endif
}


template<typename T>
inline T
lerp(float_t pT, const T& pX, const T& pY) {
    return (1.0f - pT) * pX + pT * pY;
}


}

#endif // MATHS_HH_INCLUDED

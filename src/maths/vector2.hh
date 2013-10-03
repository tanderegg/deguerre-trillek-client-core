#ifndef VECTOR2_HH_INCLUDED
#define VECTOR2_HH_INCLUDED

#include "maths.hh"

namespace trillek {


template<typename T>
struct xy_t {
    T x, y;

    xy_t(T pX, T pY)
        : x(pX), y(pY)
    {
    }

    void set(T pX, T pY)
    {
        x = pX;
        y = pY;
    }
};


struct vector2i_t : public xy_t<int32_t> {
    vector2i_t& operator+=(const vector2i_t& pRhs) {
        x += pRhs.x;
        y += pRhs.y;
        return *this;
    }

    vector2i_t& operator-=(const vector2i_t& pRhs) {
        x -= pRhs.x;
        y -= pRhs.y;
        return *this;
    }

    vector2i_t(int32_t pX, int32_t pY)
        : xy_t(pX, pY)
    {
    }
};


struct point2i_t : public xy_t<int32_t> {
    point2i_t& operator+=(const vector2i_t& pRhs) {
        x += pRhs.x;
        y += pRhs.y;
        return *this;
    }

    point2i_t& operator-=(const vector2i_t& pRhs) {
        x -= pRhs.x;
        y -= pRhs.y;
        return *this;
    }

    point2i_t(int32_t pX, int32_t pY)
        : xy_t(pX, pY)
    {
    }
};


inline vector2i_t
operator-(const point2i_t& pLhs, const point2i_t& pRhs) {
    return vector2i_t(pLhs.x - pRhs.x, pLhs.y - pRhs.y);
}


struct vector2_t : public xy_t<float_t>
{
    vector2_t& operator+=(const vector2_t& pRhs) {
        x += pRhs.x;
        y += pRhs.y;
        return *this;
    }

    vector2_t& operator-=(const vector2_t& pRhs) {
        x -= pRhs.x;
        y -= pRhs.y;
        return *this;
    }

    vector2_t& operator*=(float_t pRhs) {
        x *= pRhs;
        y *= pRhs;
        return *this;
    }

    vector2_t& scale(const vector2_t& pScale) {
        x *= pScale.x;
        y *= pScale.y;
        return *this;
    }

    float_t norm() const {
        return x * x + y * y;
    }

    float_t size() const {
        float_t n = norm();
        return n * rsqrtf(n);
    }

    vector2_t(float_t pX, float_t pY)
        : xy_t(pX, pY)
    {
    }
};


inline float_t
dot(const vector2_t& pV1, const vector2_t& pV2) {
    return pV1.x * pV1.x + pV1.y * pV2.y;
}


inline float_t
operator^(const vector2_t& pV1, const vector2_t& pV2) {
    return pV1.x * pV2.y - pV1.y * pV2.x;
}


template<>
inline vector2_t
lerp<vector2_t>(float_t pT, const vector2_t& pX, const vector2_t& pY) {
    return vector2_t(
        (1.0f - pT) * pX.x + pT * pY.x,
        (1.0f - pT) * pX.y + pT * pY.y
    );
}


struct point2_t : public xy_t<float_t>
{
    point2_t& operator+=(const vector2_t& pRhs) {
        x += pRhs.x;
        y += pRhs.y;
        return *this;
    }

    point2_t& operator-=(const vector2_t& pRhs) {
        x -= pRhs.x;
        y -= pRhs.y;
        return *this;
    }

    point2_t(float_t pX, float_t pY)
        : xy_t(pX, pY)
    {
    }
};


template<>
inline point2_t
lerp<point2_t>(float_t pT, const point2_t& pX, const point2_t& pY) {
    return point2_t(
        (1.0f - pT) * pX.x + pT * pY.x,
        (1.0f - pT) * pX.y + pT * pY.y
    );
}


inline vector2_t
operator-(const point2_t& pP1, const point2_t& pP2) {
    return vector2_t(pP1.x - pP2.x, pP1.y - pP2.y);
}


}


#endif // VECTOR2_HH_INCLUDED

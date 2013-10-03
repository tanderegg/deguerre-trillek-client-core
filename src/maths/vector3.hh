#ifndef VECTOR3_HH_INCLUDED
#define VECTOR3_HH_INCLUDED

#include "maths.hh"

namespace trillek {


struct xyz_t {
    float_t x, y, z;

    xyz_t(float_t pX, float_t pY, float_t pZ)
        : x(pX), y(pY), z(pZ)
    {
    }

    xyz_t()
        : x(0), y(0), z(0)
    {
    }

    void set(float_t pX, float_t pY, float_t pZ) {
        x = pX;
        y = pY;
        z = pZ;
    }
};


struct vector3_t : public xyz_t
{
    vector3_t& operator+=(const vector3_t& pRhs) {
        x += pRhs.x;
        y += pRhs.y;
        z += pRhs.z;
        return *this;
    }

    vector3_t& operator-=(const vector3_t& pRhs) {
        x -= pRhs.x;
        y -= pRhs.y;
        z -= pRhs.z;
        return *this;
    }

    vector3_t& operator*=(float_t pRhs) {
        x *= pRhs;
        y *= pRhs;
        z *= pRhs;
        return *this;
    }

    vector3_t& scale(const vector3_t& pScale) {
        x *= pScale.x;
        y *= pScale.y;
        z *= pScale.z;
        return *this;
    }

    float_t norm() const {
        return x * x + y * y + z * z;
    }

    float_t size() const {
        float_t s2 = norm();
        return s2 * rsqrtf(s2);
    }

    void normalize() {
        *this *= rsqrtf(norm());
    }

    vector3_t(float_t pX, float_t pY, float_t pZ)
        : xyz_t(pX, pY, pZ)
    {
    }

    vector3_t()
    {
    }

};


inline vector3_t
normalize(const vector3_t& pV) {
    vector3_t retval(pV);
    retval.normalize();
    return retval;
}


inline float_t
dot(const vector3_t& pV1, const vector3_t& pV2) {
    return pV1.x * pV1.x + pV1.y * pV2.y + pV1.z * pV2.z;
}


inline vector3_t
operator^(const vector3_t& pV1, const vector3_t& pV2) {
    return vector3_t(
        pV1.y * pV2.z - pV1.z * pV2.y,
        pV1.z * pV2.x - pV1.x * pV2.z,
        pV1.x * pV2.y - pV1.y * pV2.x
    );
}


inline vector3_t
operator-(const vector3_t& pV) {
    return vector3_t(-pV.x, -pV.y, -pV.z);
}


template<>
inline vector3_t
lerp<vector3_t>(float_t pT, const vector3_t& pX, const vector3_t& pY) {
    return vector3_t(
        (1.0f - pT) * pX.x + pT * pY.x,
        (1.0f - pT) * pX.y + pT * pY.y,
        (1.0f - pT) * pX.z + pT * pY.z
    );
}


struct point3_t : public xyz_t
{
    point3_t& operator+=(const vector3_t& pRhs) {
        x += pRhs.x;
        y += pRhs.y;
        z += pRhs.z;
        return *this;
    }

    point3_t& operator-=(const vector3_t& pRhs) {
        x -= pRhs.x;
        y -= pRhs.y;
        z -= pRhs.z;
        return *this;
    }

    point3_t()
    {
    }

    point3_t(float_t pX, float_t pY, float_t pZ)
        : xyz_t(pX, pY, pZ)
    {
    }
};


template<>
inline point3_t
lerp<point3_t>(float_t pT, const point3_t& pX, const point3_t& pY) {
    return point3_t(
        (1.0f - pT) * pX.x + pT * pY.x,
        (1.0f - pT) * pX.y + pT * pY.y,
        (1.0f - pT) * pX.z + pT * pY.z
    );
}


inline vector3_t
operator-(const point3_t& pP1, const point3_t& pP2) {
    return vector3_t(pP1.x - pP2.x, pP1.y - pP2.y, pP1.z - pP2.z);
}


}


#endif // VECTOR3_HH_INCLUDED

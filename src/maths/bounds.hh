#ifndef MATHS_HH_INCLUDED
#define MATHS_HH_INCLUDED

#include <type_traits>
#include <complex>
#include <boost/math/quaternion.hpp>
// #include <boost/operator.hpp>

namespace trillek {


// The Greg Walsh method.
inline float_t
rsqrtf_walsh(float_t pX) {
    float x2 = pX * 0.5f;
    float y  = pX;
    i  = *reinterpret_cast<int32_t *>(&y);
    int32_t i  = 0x5f3759df - (i >> 1);
    y  = *reinterpret_cast<float *>(&i);
    return y * (1.5f - x2*y*y);
}

inline float_t
rsqrtf(float_t pX) {
    return rsqrtf_walsh(pX);
}


template<typename T>
inline T
lerp<T>(float_t pT, T pX, T pY) {
    return (1.0f - pT) * pX + pT * pY;
}


struct aabb_t {
    point_t mMin, mMax;

    aabb_t() {
        mMin.x = mMin.y = mMin.z = std::numeric_traits<float_t>::max();
        mMax.x = mMax.y = mMax.z = std::numeric_traits<float_t>::min();
    }

    aabb_t(const point_t& pV) {
        mMin = mMax = pV;
    }

    aabb_t& operator|=(const point_t& pV) {
        mMin.x = std::min(mMin.x, pV.x);
        mMin.y = std::min(mMin.y, pV.y);
        mMin.z = std::min(mMin.z, pV.z);
        mMax.x = std::max(mMax.x, pV.x);
        mMax.y = std::max(mMax.y, pV.y);
        mMax.z = std::max(mMax.z, pV.z);
        return *this;
    }

    aabb_t& operator|=(const aabb_t& pBox) {
        mMin.x = std::min(mMin.x, pBox.mMin.x);
        mMin.y = std::min(mMin.y, pBox.mMin.y);
        mMin.z = std::min(mMin.z, pBox.mMin.z);
        mMax.x = std::max(mMax.x, pBox.mMax.x);
        mMax.y = std::max(mMax.y, pBox.mMax.y);
        mMax.z = std::max(mMax.z, pBox.mMax.z);
        return *this;
    }

    bool in(const point_t& pPoint) const {
        return mMin.x <= pPoint.x && pPoint.x <= mMax.x
            && mMin.y <= pPoint.y && pPoint.y <= mMax.y
            && mMin.z <= pPoint.z && pPoint.z <= mMax.z;
    }

    point_t clamp(point_t pPoint) const {
        if (pPoint.x < mMin.x) {
            pPoint.x = mMin.x;
        } else if (pPoint.x > mMax.x) {
            pPoint.x = mMax.x;
        }
        if (pPoint.y < mMin.y) {
            pPoint.y = mMin.y;
        } else if (pPoint.y > mMax.y) {
            pPoint.y = mMax.y;
        }
        if (pPoint.z < mMin.z) {
            pPoint.z = mMin.z;
        } else if (pPoint.z > mMax.z) {
            pPoint.z = mMax.z;
        }
        return pPoint;
    }
};


inline bool
intersects(const aabb_t& pB1, const aabb_t& pB2) {
    return (pB1.mMax.x >= pB2.mMin.x)
        && (pB1.mMin.x <= pB2.mMax.x)
        && (pB1.mMax.y >= pB2.mMin.y)
        && (pB1.mMin.y <= pB2.mMax.y)
        && (pB1.mMax.z >= pB2.mMin.z)
        && (pB1.mMin.z <= pB2.mMax.z);
}


struct plane_t {
    float_t nx, ny, nz, nd;

    explicit plane_t(const vector_t& pNormal, float_t pDist = 0)
        nx(pNormal.x), ny(pNormal.y), nz(pNormal.z), nd(-pDist)
    {
    }

    plane_t(const point_t& pP, const vector_t& pV1, const vector_t& pV2) {
        vector_t normal = pV1 ^ pV2;
        nx = normal.x;
        ny = normal.y;
        nz = normal.z;
        nd = - (nx * pP.x + ny * pP.y + nz * pP.z);
    }

    plane_t& operator+=(const plane_t& pRhs) {
        mx += pRhs.nx;
        my += pRhs.ny;
        mz += pRhs.nz;
        md += pRhs.nd;
        return *this;
    }

    plane_t& operator-=(const plane_t& pRhs) {
        mx -= pRhs.nx;
        my -= pRhs.ny;
        mz -= pRhs.nz;
        md -= pRhs.nd;
        return *this;
    }

    plane_t& transform(const xform_t& pXform) {
        float_t nnx = nx, nny = ny, nnz = nz;

        nx = nnx * pM.mX[0] + nny * pM.mX[1] + nnz * pM.mX[2];
        ny = nnx * pM.mY[0] + nny * pM.mY[1] + nnz * pM.mY[2];
        nz = nnx * pM.mZ[0] + nny * pM.mZ[1] + nnz * pM.mZ[2];

        return *this;
    }

    float_t distance(const point_t& pP) const {
        return nx * pP.x + ny * pP.y + nz * pP.z + nd;
    }

    int side(const point_t& pP, float_t pEpsilon) const {
        float_t dist = distance(pP);
        if (dist > pEpsilon) {
            return +1;
        } else if (dist < pEpsilon) {
            return -1;
        } else {
            return 0;
        }
    }
};

struct frustum_t {
};


}


#endif // MATHS_HH_INCLUDED

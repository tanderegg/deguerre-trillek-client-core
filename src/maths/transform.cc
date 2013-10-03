#include <transform.hh>

namespace trillek {

#if 0
namespace detail {
    inline float_t
    det2x2(float_t a, float_t b, float_t c, float_t d) {
        return a*d - b*c;
    }
}


#if 0
void
matrix3_t::invert() {
    float_t m00 = mX.x;
    float_t m01 = mX.y;
    float_t m02 = mX.z;
    float_t m10 = mY.x;
    float_t m11 = mY.y;
    float_t m12 = mY.z;
    float_t m20 = mZ.x;
    float_t m21 = mZ.y;
    float_t m22 = mZ.z;

    float_t det = m00 * (m11 * m22 - m21 * m22)
                - m10 * (m01 * m22 - m21 * m02)
                + m20 * (m01 * m12 - m11 * m02);
    float_t invdet = 1.0f / det;

    mX.x = (m11 * m22 - m21 * m12) * invdet;
    mX.y = (m12 * m20 - m22 * m10) * invdet;
    mX.z = (m10 * m21 - m20 * m11) * invdet;

    mY.x = (m21 * m02 - m01 * m22) * invdet;
    mY.y = (m22 * m00 - m02 * m20) * invdet;
    mY.z = (m20 * m01 - m00 * m21) * invdet;

    mZ.x = (m01 * m12 - m11 * m02) * invdet;
    mZ.y = (m02 * m10 - m12 * m00) * invdet;
    mZ.z = (m00 * m11 - m10 * m01) * invdet;
}
#endif


void
matrix3_t::invert_transpose() {
    float_t m00 = mM[0];
    float_t m01 = mM[1];
    float_t m02 = mM[2];
    float_t m10 = mM[3];
    float_t m11 = mM[4];
    float_t m12 = mM[5];
    float_t m20 = mM[6];
    float_t m21 = mM[7];
    float_t m22 = mM[8];

    float_t det = m00 * (m11 * m22 - m21 * m22)
                - m10 * (m01 * m22 - m21 * m02)
                + m20 * (m01 * m12 - m11 * m02);
    float_t invdet = 1.0f / det;

    mM[0] = (m11 * m22 - m21 * m12) * invdet;
    mM[1] = (m21 * m02 - m01 * m22) * invdet;
    mM[2] = (m01 * m12 - m11 * m02) * invdet;
    mM[3] = (m12 * m20 - m22 * m10) * invdet;
    mM[4] = (m22 * m00 - m02 * m20) * invdet;
    mM[5] = (m02 * m10 - m12 * m00) * invdet;
    mM[6] = (m10 * m21 - m20 * m11) * invdet;
    mM[7] = (m20 * m01 - m00 * m21) * invdet;
    mM[8] = (m00 * m11 - m10 * m01) * invdet;
}
#endif

}


#ifndef QUATERNION_HH_INCLUDED
#define QUATERNION_HH_INCLUDED

#include <maths.hh>
#include <boost/math/quaternion.hpp>

namespace trillek {

typedef boost::math::quaternion<float_t> quaternion_t;

inline quaternion_t
quaternion_rotation(float_t pAngle, const vector3_t& pAxis)
{
    float_t c = std::cos(0.5f * pAngle);
    float_t s = std::sin(0.5f * pAngle);
    return quaternion_t(c, s * pAxis.x, s * pAxis.y, s * pAxis.z);
}


}


#endif // QUATERNION_HH_INCLUDED

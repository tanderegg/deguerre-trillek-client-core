#ifndef RECT_HH_INCLUDED
#define RECT_HH_INCLUDED

#include <maths.hh>

namespace trillek {

    template<typename T>
    struct rect_t {
        T left, top, right, bottom;
        float_t aspect_ratio() const {
            return float_t(right - left) / float_t(bottom - top);
        }

        rect_t()
            : left(), top(), right(), bottom()
        {
        }

        rect_t(T pLeft, T pTop, T pRight, T pBottom)
            : left(pLeft), top(pTop), right(pRight), bottom(pBottom)
        {
        }

    };

    typedef rect_t<int32_t> recti_t;
    typedef rect_t<float_t> rectf_t;

}

#endif // RECT_HH_INCLUDED

#ifndef TRANSFORM_HH_INCLUDED
#define TRANSFORM_HH_INCLUDED

#include <maths.hh>
#include <vector2.hh>
#include <vector3.hh>
#include <quaternion.hh>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace trillek {
    struct matrix3_t;
    struct matrix4_t;

    struct ortho {
        float_t left, right, bottom, top, nearclip, farclip;

        ortho(float_t pLeft, float_t pRight,
                    float_t pBottom, float_t pTop,
                    float_t pNearClip, float_t pFarClip)
            : left(pLeft), right(pRight),
              bottom(pBottom), top(pTop),
              nearclip(pNearClip), farclip(pFarClip)
        {
        }
    };

    struct perspective {
        float_t fovy, aspect, nearclip, farclip;

        perspective(float_t pFovY, float_t pAspect,
                    float_t pNearClip, float_t pFarClip)
            : fovy(pFovY), aspect(pAspect),
              nearclip(pNearClip), farclip(pFarClip)
        {
        }
    };

    struct translation {
        float_t x, y, z;

        translation(float_t pX, float_t pY, float_t pZ)
            : x(pX), y(pY), z(pZ) {
        }

        translation(const vector3_t& pV)
            : x(pV.x), y(pV.y), z(pV.z) {
        }
    };

    struct dual_space {
        glm::mediump_mat3x3 mM;

        dual_space(const matrix3_t& pXform);

        dual_space(const matrix4_t& pXform);
    };

    struct matrix3_t {
        glm::mediump_mat3x3 mM;

        matrix3_t()
            : mM()
        {
        }

        matrix3_t(const dual_space& pDual)
            : mM(pDual.mM)
        {
        }

        matrix3_t&
        operator*=(const matrix3_t& pM) {
            mM *= pM.mM;
            return *this;
        }

    };

    inline vector3_t
    operator*(const matrix3_t& pM, const vector3_t& pV) {
        return vector3_t(
            pM.mM[0][0] * pV.x + pM.mM[0][1] * pV.y + pM.mM[0][2] * pV.z,
            pM.mM[1][0] * pV.x + pM.mM[1][1] * pV.y + pM.mM[1][2] * pV.z,
            pM.mM[2][0] * pV.x + pM.mM[2][1] * pV.y + pM.mM[2][2] * pV.z
        );
    }

    inline matrix3_t
    operator*(matrix3_t pL, const matrix3_t& pR) {
        pL *= pR;
        return pL;
    }

    struct matrix4_t {
        glm::mediump_mat4x4 mM;

        matrix4_t()
            : mM()
        {
        }

        matrix4_t(const translation& pTrans)
            : mM()
        {
            mM = glm::translate(mM, glm::vec3(pTrans.x, pTrans.y, pTrans.z));
        }

        matrix4_t(const ortho& pOrtho)
            : mM(glm::ortho(pOrtho.left, pOrtho.right,
                 pOrtho.bottom, pOrtho.top,
                 pOrtho.nearclip, pOrtho.farclip))
        {
        }

        matrix4_t(const perspective& pPerspective)
            : mM(glm::perspective(pPerspective.fovy, pPerspective.aspect,
                 pPerspective.nearclip, pPerspective.farclip))
        {
        }

        matrix4_t(const matrix3_t& pM)
            : mM(pM.mM)
        {
        }

        matrix4_t(const dual_space& pDual)
            : mM(pDual.mM)
        {
        }

        void
        translate(const vector3_t& pV) {
            mM = glm::translate(mM, glm::vec3(pV.x, pV.y, pV.z));
        }

        void
        rotate(float_t pAngle, const vector3_t& pAxis) {
            mM = glm::rotate(mM, pAngle, glm::vec3(pAxis.x, pAxis.y, pAxis.z));
        }

#if 0
        matrix4_t&
        operator*=(const matrix3_t& pM) {
            mM *= pM.mM;
            return *this;
        }
#endif

        matrix4_t&
        operator*=(const matrix4_t& pM) {
            mM *= pM.mM;
            return *this;
        }
    };

    inline vector3_t
    operator*(const matrix4_t& pM, const vector3_t& pV) {
        float_t w = pM.mM[3][0] * pV.x + pM.mM[3][1] * pV.y + pM.mM[3][2] * pV.z;
        float_t invw = 1.0f / w;
        return vector3_t(
            (pM.mM[0][0] * pV.x + pM.mM[0][1] * pV.y + pM.mM[0][2] * pV.z) * invw,
            (pM.mM[1][0] * pV.x + pM.mM[1][1] * pV.y + pM.mM[1][2] * pV.z) * invw,
            (pM.mM[2][0] * pV.x + pM.mM[2][1] * pV.y + pM.mM[2][2] * pV.z) * invw
        );
    }

    inline point3_t
    operator*(const matrix4_t& pM, const point3_t& pV) {
        float_t w = pM.mM[3][0] * pV.x + pM.mM[3][1] * pV.y + pM.mM[3][2] * pV.z + pM.mM[3][3];
        float_t invw = 1.0f / w;
        return point3_t(
            (pM.mM[0][0] * pV.x + pM.mM[0][1] * pV.y + pM.mM[0][2] * pV.z + pM.mM[0][3]) * invw,
            (pM.mM[1][0] * pV.x + pM.mM[1][1] * pV.y + pM.mM[1][2] * pV.z + pM.mM[1][3]) * invw,
            (pM.mM[2][0] * pV.x + pM.mM[2][1] * pV.y + pM.mM[2][2] * pV.z + pM.mM[2][3]) * invw
        );
    }

    inline matrix4_t
    operator*(matrix4_t pL, const matrix4_t& pR) {
        pL *= pR;
        return pL;
    }

    inline
    dual_space::dual_space(const matrix3_t& pXform) {
        mM = glm::inverseTranspose(pXform.mM);
    }

    inline
    dual_space::dual_space(const matrix4_t& pXform) {
        mM = glm::inverseTranspose(glm::mat3(pXform.mM));
    }

}


#endif // TRANSFORM_HH_INCLUDED

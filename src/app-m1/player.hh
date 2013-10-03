#ifndef PLAYER_HH_INCLUDED
#define PLAYER_HH_INCLUDED

#include <vector3.hh>
#include <transform.hh>
#include <rect.hh>

namespace trillek {

    // XXX Experimental; do not ship
    struct camera {

        camera() {
            mEye = point3_t{0,6,0};
            mViewDirection = vector3_t{1,0,0};
            mVUV = vector3_t{0,1,0};
        }

        void update_projection_transform(const recti_t& pViewport,
               float_t pNearClip, float_t pFarClip,
               float_t pFovY = 75.0f)
        {
            mViewport = pViewport;
            mProjectionXform = matrix4_t(perspective(pFovY,
                    pViewport.aspect_ratio(), pNearClip, pFarClip));
        }

        const matrix4_t& projection_transform() const {
            return mProjectionXform;
        }

        void update_camera_transform() {
            mCameraXform.mM = glm::lookAt(
                glm::vec3(mEye.x, mEye.y, mEye.z),
                glm::vec3(mEye.x + mViewDirection.x,
                          mEye.y + mViewDirection.y,
                          mEye.z + mViewDirection.z),
                glm::vec3(mVUV.x, mVUV.y, mVUV.z));
        }

        const matrix4_t& camera_transform() const {
            return mCameraXform;
        }

        recti_t mViewport;
        matrix4_t mProjectionXform;

        point3_t mEye;
        vector3_t mViewDirection;
        vector3_t mVUV;
        matrix4_t mCameraXform;
    };

    // XXX Experimental; do not ship
    struct player {
        player() {
            mCollisionHull = 6.00f;
            mEyeLevel = 12.80f;
            mWalkSpeed = 28.0f;
            mJumpHeight = 10.0f;
        }

        std::shared_ptr<camera> mCamera;
        float_t mCollisionHull;
        float_t mEyeLevel;
        float_t mWalkSpeed;
        float_t mJumpHeight;
    };

}

#endif // PLAYER_HH_INCLUDED

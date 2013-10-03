#ifndef GRAPHICS_DEVICE_HH_INCLUDED
#define GRAPHICS_DEVICE_HH_INCLUDED

#include <graphics_constants.hh>
#include <render_target.hh>
#include <transform.hh>
#include <color.hh>
#include <rect.hh>

namespace trillek {

    class vertex_format;
    class vertex_buffer;
    class index_buffer;
    class texture;
    class draw_immediate;
    class mesh;
    class graphics_state;

    struct graphics_device_statistics {
        std::array<uint32_t, STAT_LAST> mStats;

        void clear();

        void start(graphics_device_statistics& pSrc);

        void end(graphics_device_statistics& pSrc);
    };

    class graphics_device {
    public:
        void init();

        bool currently_active() const { return mCurrentlyActive; }

        virtual ~graphics_device();

        const matrix4_t& model_transform() const;
        matrix4_t& model_transform();

        void push_model_transform();

        void pop_model_transform();

        const matrix4_t& projection_transform() const;
        matrix4_t& projection_transform();

        const matrix4_t& camera_transform() const;
        matrix4_t& camera_transform();

        void set_viewport(const recti_t& pRect) {
            mViewport = pRect;
            mViewportDirty = true;
            mAnythingDirty = true;
        }

        std::shared_ptr<vertex_format>
        standard_vertex_format(standard_vertex_format_t pFmt);

        virtual std::unique_ptr<vertex_format>
        make_vertex_format(std::string pFormat) = 0;

        virtual std::unique_ptr<mesh>
        make_mesh(primitive_type_t pType,
                const std::shared_ptr<vertex_format>& pFmt,
                uint32_t mCount, buffer_lifetime_t pLifetime) = 0;

        virtual std::unique_ptr<vertex_buffer>
        make_vertex_buffer(std::shared_ptr<vertex_format> pFmt,
                uint32_t pVertCount, buffer_lifetime_t pLifetime) = 0;

        void set_vertex_buffer(std::shared_ptr<vertex_buffer> pBuf);

        virtual void draw_primitive(primitive_type_t pType, uint32_t pVertexStart, uint32_t pPrimitiveCount) = 0;

        virtual std::shared_ptr<window_target> make_window_target(
                const std::shared_ptr<window>& pWindow) = 0;

/*
        virtual std::unique_ptr<texture_target> make_texture_target() = 0;
*/

        void set_render_target(std::shared_ptr<render_target> pTarget);
        void push_render_target();
        void pop_render_target();

        std::shared_ptr<render_target> get_render_target()
        {
            return mCurrRT;
        }

        void set_graphics_state(std::shared_ptr<graphics_state> pState);
        void push_graphics_state();
        void pop_graphics_state();

        virtual void clear(clear_flags_t pFlags, const rgba_t& pColor,
                float_t pDepth, uint32_t pStencil) = 0;

        void begin_frame();
        void end_frame();

        draw_immediate& get_draw_immediate();

        void update_state(bool pForce = false);

    protected:
        graphics_device();

        graphics_device_statistics mStatistics;

        bool mCurrentlyActive;

        virtual void begin_frame_internal() = 0;
        virtual void end_frame_internal() = 0;

        bool mAnythingDirty;

        bool mModelXformDirty;
        uint32_t mModelXformSP;
        matrix4_t mModelXform[MAX_MODEL_TRANSFORM_STACK];

        bool mProjectionXformDirty;
        matrix4_t mProjectionXform;

        bool mCameraXformDirty;
        matrix4_t mCameraXform;

        virtual void update_transforms_internal(bool pForce) = 0;

        bool mVertexBufferDirty;
        std::shared_ptr<vertex_buffer> mCurrVB;
        std::shared_ptr<vertex_buffer> mPrevVB;

        virtual void update_vertex_buffer_internal() = 0;

        bool mIndexBufferDirty;
        std::shared_ptr<index_buffer> mCurrIB;
        std::shared_ptr<index_buffer> mPrevIB;

        virtual void update_index_buffer_internal() = 0;

        bool mRTDirty;
        std::vector<std::shared_ptr<render_target>> mRTStack;
        std::shared_ptr<render_target> mCurrRT;
        std::shared_ptr<render_target> mPrevRT;

        virtual void update_render_target_internal() = 0;

        bool mStateDirty;
        std::vector<std::shared_ptr<graphics_state>> mStateStack;
        std::shared_ptr<graphics_state> mCurrState;
        std::shared_ptr<graphics_state> mPrevState;

        virtual void update_graphics_state_internal(bool pForce) = 0;

        bool mViewportDirty;
        recti_t mViewport;

        virtual void update_viewport_internal() = 0;

        // Anything which subclasses or inline members don't need to see.
        struct impl;
        std::unique_ptr<impl> mPImpl;
    };

    inline void
    graphics_device::push_model_transform() {
        if (mModelXformSP + 1 >= MAX_MODEL_TRANSFORM_STACK) {
            throw std::logic_error("graphics_device::push_model_transform");
        }
        mModelXform[mModelXformSP+1] = mModelXform[mModelXformSP];
        ++mModelXformSP;
    }

    inline void
    graphics_device::pop_model_transform() {
        if (mModelXformSP == 0) {
            throw std::logic_error("graphics_device::pop_model_transform");
        }
        --mModelXformSP;
        mModelXformDirty = true;
        mAnythingDirty = true;
    }

    inline const matrix4_t&
    graphics_device::model_transform() const {
        return mModelXform[mModelXformSP];
    }

    inline matrix4_t&
    graphics_device::model_transform() {
        mModelXformDirty = true;
        mAnythingDirty = true;
        return mModelXform[mModelXformSP];
    }

    inline const matrix4_t&
    graphics_device::projection_transform() const {
        return mProjectionXform;
    }

    inline matrix4_t&
    graphics_device::projection_transform() {
        mProjectionXformDirty = true;
        mAnythingDirty = true;
        return mProjectionXform;
    }

    inline const matrix4_t&
    graphics_device::camera_transform() const {
        return mCameraXform;
    }

    inline matrix4_t&
    graphics_device::camera_transform() {
        mCameraXformDirty = true;
        mAnythingDirty = true;
        return mCameraXform;
    }

}


#endif // GRAPHICS_DEVICE_HH_INCLUDED

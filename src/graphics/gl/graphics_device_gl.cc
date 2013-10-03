#include <graphics_gl.hh>
#include <graphics_device_gl.hh>
#include <translate_constants_gl.hh>
#include <vertex_buffer_gl.hh>
#include <vertex_format_gl.hh>
#include <graphics_state.hh>
#include <mesh_gl.hh>

namespace trillek {

namespace gl {

    void
    throw_gl_error(GLuint err) {
        switch (err) {
        case GL_INVALID_ENUM:
            throw std::invalid_argument("GL_INVALID_ENUM");

        case GL_INVALID_VALUE:
            throw std::invalid_argument("GL_INVALID_VALUE");

        case GL_INVALID_OPERATION:
            throw std::invalid_argument("GL_INVALID_OPERATION");

        case GL_STACK_OVERFLOW:
            throw std::logic_error("GL_STACK_OVERFLOW");

        case GL_STACK_UNDERFLOW:
            throw std::logic_error("GL_STACK_UNDERFLOW");

        case GL_OUT_OF_MEMORY:
            throw std::runtime_error("GL_OUT_OF_MEMORY");

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            throw std::logic_error("GL_INVALID_FRAMEBUFFER_OPERATION");

        case GL_TABLE_TOO_LARGE:
            throw std::invalid_argument("GL_TABLE_TOO_LARGE");

        default:
            throw std::logic_error("Unknown OpenGL error");
        }
    }

}


graphics_device_gl::graphics_device_gl()
{
    init();
}


graphics_device_gl::~graphics_device_gl()
{
}


void
graphics_device_gl::update_transforms_internal(bool pForce)
{
    if (mModelXformDirty || mCameraXformDirty || pForce) {
        glMatrixMode(GL_MODELVIEW);
        mModelViewXform = mModelXform[mModelXformSP];
        mModelViewXform *= mCameraXform;
        glLoadMatrixf(&mModelViewXform.mM[0][0]);
        mModelXformDirty = false;
        mCameraXformDirty = false;
    }

    if (mProjectionXformDirty || pForce) {
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&mProjectionXform.mM[0][0]);
        mProjectionXformDirty = false;
        glMatrixMode(GL_MODELVIEW);
    }
}


namespace {

inline void
toggle_gl_state(bool pFlag, GLenum pGlFlag)
{
    if (pFlag) {
        glEnable(pGlFlag);
    }
    else {
        glDisable(pGlFlag);
    }
}


inline void
update_depth_bias_gl(float_t pBias) {
    if (pBias == 0) {
        glDisable(GL_POLYGON_OFFSET_FILL);
    }
    else {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(pBias, pBias);
    }
}


}

void
graphics_device_gl::update_graphics_state_internal(bool pForce)
{
    graphics_state& currState = *mCurrState;

    if (pForce || !mPrevState) {

        // Color
        glColorMask(currState.mColor.mFlags[C_R],
                    currState.mColor.mFlags[C_G],
                    currState.mColor.mFlags[C_B],
                    currState.mColor.mFlags[C_A]);

        // Depth
        toggle_gl_state(currState.mDepth.mFlags[D_ENABLE], GL_DEPTH_TEST);
        glDepthFunc(translate_cmp_func_gl(currState.mDepth.mDepthCmp));
        update_depth_bias_gl(currState.mDepth.mDepthBias);
        glDepthMask(currState.mDepth.mFlags[D_WENABLE] ? GL_TRUE : GL_FALSE);

        return;
    }

    graphics_state& prevState = *mPrevState;

    // Color
    {
        graphics_state::color_state& prevColor = prevState.mColor;
        graphics_state::color_state& currColor = currState.mColor;
        if (currColor.mFlags != prevColor.mFlags) {
            glColorMask(currColor.mFlags[C_R],
                        currColor.mFlags[C_G],
                        currColor.mFlags[C_B],
                        currColor.mFlags[C_A]);
        }
    }

    // Depth
    {
        graphics_state::depth_state& prevDepth = prevState.mDepth;
        graphics_state::depth_state& currDepth = currState.mDepth;

        if (currDepth.mFlags[D_ENABLE] != prevDepth.mFlags[D_ENABLE]) {
            toggle_gl_state(currDepth.mFlags[D_ENABLE], GL_DEPTH_TEST);
        }

        if (currDepth.mDepthCmp != prevDepth.mDepthCmp) {
            glDepthFunc(translate_cmp_func_gl(currDepth.mDepthCmp));
        }

        if (currDepth.mDepthBias != prevDepth.mDepthBias) {
            update_depth_bias_gl(currDepth.mDepthBias);
        }

        if (currDepth.mFlags[D_WENABLE] != prevDepth.mFlags[D_WENABLE]) {
            glDepthMask(currDepth.mFlags[D_WENABLE] ? GL_TRUE : GL_FALSE);
        }
    }
}


void
graphics_device_gl::update_vertex_buffer_internal()
{
    if (mPrevVB) {
        mPrevVB->deselect();
        mPrevVB = std::shared_ptr<vertex_buffer>();
    }

    if (mCurrVB) {
        mCurrVB->select();
    }
}


void
graphics_device_gl::update_index_buffer_internal()
{
    if (mPrevIB) {
        mPrevIB->deselect();
        mPrevIB = std::shared_ptr<index_buffer>();
    }

    if (mCurrIB) {
        mCurrIB->select();
    }
}


void
graphics_device_gl::update_render_target_internal()
{
    if (mPrevRT) {
        mPrevRT->deselect();
        mPrevRT = std::shared_ptr<render_target>();
    }

    if (mCurrRT) {
        mCurrRT->select();
    }
}


void
graphics_device_gl::update_viewport_internal()
{
    glViewport(mViewport.left, mViewport.top, mViewport.right, mViewport.bottom);
}


std::unique_ptr<vertex_format>
graphics_device_gl::make_vertex_format(std::string pName) {
    return std::unique_ptr<vertex_format>(
        new vertex_format_gl(pName)
    );
}

#if 0

        virtual std::unique_ptr<mesh>
        make_mesh(primitive_type_t pType,
                const std::shared_ptr<vertex_format>& pFmt,
                uint32_t mCount, buffer_lifetime_t pLifetime);

        virtual std::unique_ptr<vertex_buffer>
        make_vertex_buffer(const std::shared_ptr<vertex_format>& pFmt, uint32_t pVertCount,
                buffer_lifetime_t pLifetime);
#endif



inline void
graphics_device_gl::pre_draw_primitive() {
}


inline void
graphics_device_gl::post_draw_primitive(uint32_t pPrimitiveCount) {
    ++mStatistics.mStats[STAT_DRAW_CALLS];
    mStatistics.mStats[STAT_POLY_COUNT] += pPrimitiveCount;
}



void
graphics_device_gl::draw_primitive(primitive_type_t pType,
        uint32_t pVertexStart, uint32_t pPrimitiveCount)
{
    pre_draw_primitive();

    glDrawArrays(translate_primitive_type_gl(pType), pVertexStart,
        translate_index_count_gl(pType, pPrimitiveCount));

    post_draw_primitive(pPrimitiveCount);
}


void
graphics_device_gl::begin_frame_internal() {
}


void
graphics_device_gl::end_frame_internal() {
}


void
graphics_device_gl::clear(clear_flags_t pFlags, const rgba_t& pColor,
                float_t pDepth, uint32_t pStencil)
{
    glDepthMask(GL_TRUE);
    glClearDepth(pDepth);
    glClearColor(pColor.r, pColor.g, pColor.b, pColor.a);
    glClearStencil(pStencil);
    glClear(translate_clear_flags_gl(pFlags));
}


std::shared_ptr<window_target>
graphics_device_gl::make_window_target(const std::shared_ptr<window>& pWindow)
{
    return pWindow->make_window_target(shared_from_this());
}


#if 0

std::unique_ptr<texture_target> make_texture_target();

#endif

#if 0

std::unique_ptr<index_buffer>
graphics_device_gl::make_index_buffer(buffer_lifetime_t pLifetime,
            unsigned pIndexCount) {
    return std::unique_ptr<index_buffer>(
        new index_buffer_gl(pLifetime, pIndexCount)
    );
}
#endif

std::unique_ptr<vertex_buffer>
graphics_device_gl::make_vertex_buffer(std::shared_ptr<vertex_format> pFormat,
            uint32_t pVertexCount, buffer_lifetime_t pLifetime) {
    return std::unique_ptr<vertex_buffer>(
        new vertex_buffer_gl(pLifetime, std::move(pFormat), pVertexCount)
    );
}


std::unique_ptr<mesh>
graphics_device_gl::make_mesh(primitive_type_t pType,
            const std::shared_ptr<vertex_format>& pFormat,
            uint32_t pPrimitiveCount, buffer_lifetime_t pLifetime) {
    return std::unique_ptr<mesh>(
        new mesh_gl(shared_from_this(), pLifetime, pType,
                std::static_pointer_cast<vertex_format_gl>(pFormat),
                pPrimitiveCount)
    );
}


}


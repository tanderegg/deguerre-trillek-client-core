#include <graphics_device.hh>
#include <primitive.hh>
#include <draw_immediate.hh>
#include <graphics_state.hh>

namespace trillek {

struct graphics_device::impl {
    std::unique_ptr<draw_immediate> mDrawImmediate;

    std::shared_ptr<vertex_format> mStdVertexFormats[STD_VTX_FMT_COUNT];

    std::unique_ptr<vertex_format>
    make_std_vtx_fmt_pc(graphics_device& pDevice) {
        auto vf = pDevice.make_vertex_format("STD_PC");
        vf->add_element(VERTDATA_POSITION, VERTDATA_TYPE_FLOAT3);
        vf->add_element(VERTDATA_COLOR, VERTDATA_TYPE_FLOAT4);
        return vf;
    }

    impl(graphics_device& pDevice) {
    }

    void init(graphics_device& pDevice) {
        mStdVertexFormats[STD_VTX_FMT_PC] = make_std_vtx_fmt_pc(pDevice);

        mDrawImmediate = std::unique_ptr<draw_immediate>(
            new draw_immediate(pDevice)
        );
    }
};

void
graphics_device_statistics::clear() {
    for (auto& s : mStats) {
        s = 0;
    }
}

void
graphics_device_statistics::start(graphics_device_statistics& pSrc) {
    for (unsigned i = 0; i < STAT_LAST; ++i) {
        mStats[i] = pSrc.mStats[i];
    }
}

void
graphics_device_statistics::end(graphics_device_statistics& pSrc) {
    for (unsigned i = 0; i < STAT_LAST; ++i) {
        mStats[i] = pSrc.mStats[i] - mStats[i];
    }
}


draw_immediate&
graphics_device::get_draw_immediate()
{
    return *mPImpl->mDrawImmediate;
}


graphics_device::graphics_device()
    : mPImpl(new graphics_device::impl(*this))
{
    mAnythingDirty = true;

    mModelXformDirty = true;
    mModelXformSP = 0;
    mModelXform[mModelXformSP] = matrix4_t();

    mProjectionXformDirty = true;
    mProjectionXform = matrix4_t();

    mCameraXformDirty = true;
    mCameraXform = matrix4_t();

    mViewport = recti_t{};
    mViewportDirty = true;

    mVertexBufferDirty = true;
    mIndexBufferDirty = true;
    mRTDirty = true;
}


graphics_device::~graphics_device()
{
}


void
graphics_device::init()
{
    mPImpl->init(*this);
}


void
graphics_device::begin_frame()
{
    mStatistics.clear();
    update_state();
    begin_frame_internal();
}


void
graphics_device::end_frame()
{
    end_frame_internal();
}


std::shared_ptr<vertex_format>
graphics_device::standard_vertex_format(standard_vertex_format_t pFmt) {
    if (pFmt >= STD_VTX_FMT_COUNT) {
        throw std::logic_error("render_subsystem::standard_vertex_format");
    }
    return mPImpl->mStdVertexFormats[pFmt];
}


void
graphics_device::set_vertex_buffer(std::shared_ptr<vertex_buffer> pBuf)
{
    if (pBuf == mCurrVB) {
        return;
    }
    if (!mVertexBufferDirty) {
        if (mPrevVB) {
            mPrevVB->deselect();
        }
        mPrevVB = std::move(mCurrVB);
    }
    mVertexBufferDirty = true;
    mAnythingDirty = true;
    mCurrVB = std::move(pBuf);
}


void
graphics_device::set_render_target(std::shared_ptr<render_target> pTarget)
{
    if (pTarget == mCurrRT) {
        return;
    }
    if (!mRTDirty) {
        if (mPrevRT) {
            mPrevRT->deselect();
        }
        mPrevRT = std::move(mCurrRT);
    }

    mRTDirty = true;
    mAnythingDirty = true;
    mCurrRT = std::move(pTarget);
}


void
graphics_device::push_render_target()
{
    mRTStack.push_back(mCurrRT);
}


void
graphics_device::pop_render_target()
{
    set_render_target(mRTStack.back());
    mRTStack.pop_back();
}


void
graphics_device::set_graphics_state(std::shared_ptr<graphics_state> pState)
{
    if (pState == mCurrState) {
        return;
    }

    if (!mStateDirty) {
        mPrevState = std::move(mCurrState);
    }

    mStateDirty = true;
    mAnythingDirty = true;
    mCurrState = std::move(pState);
}


void
graphics_device::push_graphics_state()
{
    mStateStack.push_back(mCurrState);
}


void
graphics_device::pop_graphics_state()
{
    set_graphics_state(mStateStack.back());
    mStateStack.pop_back();
}


void
graphics_device::update_state(bool pForce)
{
    if (pForce) {
        update_transforms_internal(true);

        update_graphics_state_internal(true);
        mStateDirty = false;

        update_vertex_buffer_internal();
        mVertexBufferDirty = false;

        update_index_buffer_internal();
        mIndexBufferDirty = false;

        update_render_target_internal();
        mRTDirty = false;

        mAnythingDirty = false;
        return;
    }

    if (!mAnythingDirty) {
        return;
    }

    mAnythingDirty = false;

    if (mProjectionXformDirty || mModelXformDirty || mCameraXformDirty) {
        update_transforms_internal(false);
    }

    if (mStateDirty) {
        update_graphics_state_internal(false);
        mStateDirty = false;
    }

    if (mVertexBufferDirty) {
        update_vertex_buffer_internal();
        mVertexBufferDirty = false;
    }

    if (mIndexBufferDirty) {
        update_index_buffer_internal();
        mIndexBufferDirty = false;
    }

    if (mRTDirty) {
        update_render_target_internal();
        mRTDirty = false;
    }

    if (mViewportDirty) {
        update_viewport_internal();
        mViewportDirty = false;
    }
}

}


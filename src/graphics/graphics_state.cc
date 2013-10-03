#include <graphics_state.hh>

namespace trillek {

graphics_state::color_state::color_state()
{
    mFlags[C_R] = true;
    mFlags[C_G] = true;
    mFlags[C_B] = true;
    mFlags[C_A] = true;
}


graphics_state::depth_state::depth_state()
{
    mFlags[D_ENABLE] = true;
    mFlags[D_WENABLE] = true;
    mDepthCmp = CMP_LE;
    mDepthBias = 0.0f;
}


graphics_state::graphics_state()
{
}


}



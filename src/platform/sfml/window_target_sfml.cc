#include <window_target_sfml.hh>
#include <window_sfml.hh>
#include <graphics_device_gl.hh>

namespace trillek {


window_target_sfml::window_target_sfml(
        const std::shared_ptr<window_sfml>& pWin,
	const std::shared_ptr<graphics_device_gl>& pDevice)
    : window_target(std::static_pointer_cast<window>(pWin)),
      mDevice(pDevice), mWinSfml(pWin)
{
}


window_target_sfml::~window_target_sfml()
{
}


format_t
window_target_sfml::get_format() const
{
    return format_t{};
}


vector2i_t
window_target_sfml::get_dimensions()
{
    return vector2i_t{0,0};
}


void
window_target_sfml::select()
{
}


void
window_target_sfml::deselect()
{
}


void
window_target_sfml::copy_to_texture(texture& pTex)
{
}


bool
window_target_sfml::swap_buffers()
{
    mWinSfml->swap_buffers();
    return true;
}



}



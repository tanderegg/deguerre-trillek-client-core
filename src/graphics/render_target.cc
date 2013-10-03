#include <render_target.hh>
#include <window.hh>

namespace trillek {

render_target::render_target()
{
}

render_target::~render_target()
{
}


window_target::window_target(std::weak_ptr<window> pWindow)
    : mWindow(std::move(pWindow))
{
}

}



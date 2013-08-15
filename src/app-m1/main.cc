#include <graphics_driver.hh>
#include <input_driver.hh>
#include <trillek_gl.hh>

namespace trillek {
    extern graphics_driver& get_graphics_driver();
    extern input_driver& get_input_driver();
}

int
main(int argc, char* argv[])
{
    trillek::graphics_driver& graphics = trillek::get_graphics_driver();
    trillek::input_driver& input = trillek::get_input_driver();

    graphics.init();
    input.init();

    // input.capture_mouse(false);

    for (;;)
    {
        graphics.begin_rendering();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        graphics.end_rendering();
        graphics.swap_buffers();
    }

    graphics.pre_shutdown();
    input.pre_shutdown();
    graphics.shutdown();
    input.shutdown();
}



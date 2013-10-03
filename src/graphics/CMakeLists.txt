add_subdirectory(gl)

set(trillek-graphics_SRCS
    graphics_subsystem.cc
    graphics_device.cc
    graphics_state.cc
    render_target.cc
    primitive.cc
    draw_immediate.cc
)

include_directories(trillek-graphics
    ${TRILLEK_INCLUDE_DIRS}
)

add_library(trillek-graphics STATIC
    ${trillek-graphics_SRCS}
)

target_link_libraries(trillek-graphics
    ${TRILLEK_GRAPHICS_LIBRARY}
)

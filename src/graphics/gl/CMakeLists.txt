include(GL)

set(trillek-graphics-gl_SRCS
    graphics_device_gl.cc
    vertex_buffer_gl.cc
    texture_target_gl.cc
    mesh_gl.cc
)

include_directories(trillek-graphics
    ${TRILLEK_INCLUDE_DIRS}
    ${TRILLEK_GL_DIRS}
    .
)

add_library(trillek-graphics-gl STATIC
    ${trillek-graphics-gl_SRCS}
)



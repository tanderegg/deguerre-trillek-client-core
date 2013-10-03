include(GL)
include(SFML)

set(trillek-platform-sfml_SRCS
    platform_sfml.cc
    window_sfml.cc
    window_manager_sfml.cc
    window_target_sfml.cc
)

add_library(trillek-platform-sfml STATIC
    ${trillek-platform-sfml_SRCS}
)

include_directories(trillek-platform-sfml
    ${TRILLEK_INCLUDE_DIRS}
    ${TRILLEK_GL_INCLUDE_DIRS}
    ${SFML_INCLUDE_DIR}
    .
)

target_link_libraries(trillek-platform-sfml
    ${SFML_LIBRARIES}
)

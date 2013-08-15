include(GL)
include(SFML)

set(trillek-client-m1_SRCS
    main.cc
    sfml_device.cc
)

add_executable(trillek-client-m1 ${trillek-client-m1_SRCS})

include_directories(trillek-client-m1
    ${TRILLEK_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${SFML_INCLUDE_DIR}
)

target_link_libraries(trillek-client-m1
    ${OPENGL_LIBRARIES}
    ${SFML_LIBRARIES}
)

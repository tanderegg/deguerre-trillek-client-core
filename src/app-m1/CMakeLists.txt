
set(trillek-client-m1_SRCS
    main.cc
)

add_executable(trillek-client-m1 ${trillek-client-m1_SRCS})

include_directories(trillek-client-m1
    ${TRILLEK_INCLUDE_DIRS}
    ${TRILLEK_GL_INCLUDE_DIRS}
)

target_link_libraries(trillek-client-m1
    ${TRILLEK_LIBRARIES}
    ${TRILLEK_GRAPHICS_LIBRARY}
)

add_subdirectory(sfml)

set(trillek-platform_SRCS
    system_event_queue.cc
    window.cc
    window_manager.cc
    subsystem_manager.cc
    platform_subsystem.cc
)

add_library(trillek-platform STATIC
    ${trillek-platform_SRCS}
)

include_directories(trillek-platform
    ${TRILLEK_INCLUDE_DIRS}
)

target_link_libraries(trillek-platform
    ${TRILLEK_PLATFORM_LIBRARY}
)

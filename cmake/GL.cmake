if (MSVC_OR_MINGW)
elseif (APPLE)
    add_definitions(-DGL_GLEXT_PROTOTYPES)
else ()
    add_definitions(-DGL_GLEXT_PROTOTYPES)
endif()

find_package(OpenGL REQUIRED)


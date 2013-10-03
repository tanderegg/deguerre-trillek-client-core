project(trillek-client)

cmake_minimum_required(VERSION 2.6)
set(PACKAGE_BUGREPORT "need-an-email-address@trillek.org")
set(PACKAGE_NAME "trillek-client")
set(PACKAGE_VERSION "0.0.0a")
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TARNAME "${PACKAGE_NAME}")

option(BUILD_tests "build the tests" ON)

if(BUILD_tests)
    enable_testing()
endif(BUILD_tests)

set(CMAKE_MODULE_PATH ${trillek-client_SOURCE_DIR}/cmake)

set(TRILLEK_INCLUDE_DIRS
    ${trillek-client_SOURCE_DIR}/src/include
    ${trillek-client_SOURCE_DIR}/src/maths
    ${trillek-client_SOURCE_DIR}/src/platform
    ${trillek-client_SOURCE_DIR}/src/graphics
)

set(TRILLEK_LIBRARIES
    trillek-graphics
    trillek-platform
    trillek-maths
)

set(TRILLEK_PLATFORM_LIBRARY
    trillek-platform-sfml
)

set(TRILLEK_GRAPHICS_LIBRARY
    trillek-graphics-gl
    ${OPENGL_gl_LIBRARY}
)

set(TRILLEK_GL_INCLUDE_DIRS
    ${trillek-client_SOURCE_DIR}/src/graphics/gl
)

include(Platform)
include(Boost)

add_subdirectory(src)

if(BUILD_tests)
endif(BUILD_tests)

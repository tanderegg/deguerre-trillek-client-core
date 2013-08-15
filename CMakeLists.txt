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
)

include(Platform)

add_subdirectory(src)

if(BUILD_tests)
endif(BUILD_tests)

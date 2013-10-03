set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS system REQUIRED)
if (Boost_FOUND)
    MESSAGE(${Boost_SYSTEM_LIBRARY})
    include_directories(${Boost_INCLUDE_DIRS})
endif (Boost_FOUND)

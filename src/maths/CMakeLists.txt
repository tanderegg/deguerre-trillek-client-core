
set(trillek-maths_SRCS
    transform.cc
)

add_library(trillek-maths STATIC
    ${trillek-maths_SRCS}
)

include_directories(trillek-maths
    ${TRILLEK_INCLUDE_DIRS}
)


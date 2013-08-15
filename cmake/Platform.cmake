if (CMAKE_HOST_APPLE)
    set(TRILLEK_SEARCH_PATHS
        /usr
        /Applications/Xcode.app/Contents/Developer/Platforms.MacOSX.platform/Developer/SDKs
        /Library/Frameworks
        /usr/local
        /opt/local
    )
else (CMAKE_HOST_APPLE)
    # OS X is a Unix, but it's not a normal Unix as far as search paths go.
    if (CMAKE_HOST_UNIX)
        message(WARNING "This build has not yet been tested on Unix")
        set(TRILLEK_SEARCH_PATHS
            /usr
            /usr/local
            /opt/local
        )
    endif (CMAKE_HOST_UNIX)
endif (CMAKE_HOST_APPLE)

if (CMAKE_HOST_WIN32)
    message(WARNING "This build has not yet been tested on Win32")
    set(TRILLEK_SEARCH_PATHS
        /usr
        /usr/local
        /opt/local
    )
endif (CMAKE_HOST_WIN32)

if (MSVC)
    message(WARNING "This build has not yet been tested with VC++")
    set(PLATFORM_FLAGS)
else (MSVC)
    set(PLATFORM_FLAGS "-Winline" "-Wall" "-fomit-frame-pointer" "-ffast-math")
endif (MSVC)

add_definitions(${PLATFORM_FLAGS})



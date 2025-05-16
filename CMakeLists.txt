cmake_minimum_required(VERSION 3.30)
project(StenoByte_Prototype C)

set(CMAKE_C_STANDARD 23)

# Linux Variant
if (LINUX)
    # Finds Libevdev Library
    find_package(PkgConfig REQUIRED)
    pkg_search_module(LIBEVDEV REQUIRED libevdev)

    # Prints Location of Libevdev Library
    message(STATUS "libevdev include dirs: ${LIBEVDEV_INCLUDE_DIRS}")
    message(STATUS "libevdev libraries: ${LIBEVDEV_LIBRARIES}")

    ## StenoByte Library for Linux Library
    add_library(StenoByte_Library STATIC
            includes/StenoByte_Helper_for_Linux.c
            includes/StenoByte_Core.c)
    target_include_directories(StenoByte_Library PRIVATE
            ${LIBEVDEV_INCLUDE_DIRS}
            includes)
    target_link_libraries(StenoByte_Library PRIVATE ${LIBEVDEV_LIBRARIES})

# MacOS Variant
elseif (APPLE)
    message(FATAL_ERROR "Unfortunately StenoByte is not yet compatible with MacOS,
     but support for MacOS is in the works!")

# Windows Variant
elseif (WIN32)
    message(FATAL_ERROR "Unfortunately StenoByte is not yet compatible with Windows,
    but support for Windows is in the works!")

# Throws an error if system is incompatible
else ()
    message(FATAL_ERROR "Unfortunately your system is not compatible with StenoByte")
endif ()

# Main App
add_executable(StenoByte_Prototype main.c)
target_include_directories(StenoByte_Prototype PRIVATE ${LIBEVDEV_INCLUDE_DIRS} StenoByte_Library)
target_link_libraries(StenoByte_Prototype PRIVATE ${LIBEVDEV_LIBRARIES} StenoByte_Library)
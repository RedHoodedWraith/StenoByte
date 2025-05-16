cmake_minimum_required(VERSION 3.30)
project(StenoByte_Prototype C)

set(CMAKE_C_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBEVDEV REQUIRED libevdev)

add_executable(StenoByte_Prototype main.c
        StenoByte_Helper_for_Linux.c
        StenoByte_Helper.h
        StenoByte_Core.c
        StenoByte_Core.h)
target_include_directories(StenoByte_Prototype PRIVATE ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(StenoByte_Prototype PRIVATE ${LIBEVDEV_LIBRARIES})
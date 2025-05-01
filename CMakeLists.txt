cmake_minimum_required(VERSION 3.31)
project(StenoByte_Prototype C)

set(CMAKE_C_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBEVDEV REQUIRED libevdev)

add_executable(StenoByte_Prototype main.c)
target_include_directories(StenoByte_Prototype PRIVATE ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(StenoByte_Prototype PRIVATE ${LIBEVDEV_LIBRARIES})
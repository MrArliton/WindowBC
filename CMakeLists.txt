cmake_minimum_required(VERSION 3.9)

project(clastwindow VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/wxWidgets)

set(wxWidgets_USE_LIBS base core net)

add_executable(clastwindow ./module.cpp)

target_link_libraries(clastwindow wx::net wx::core wx::base)


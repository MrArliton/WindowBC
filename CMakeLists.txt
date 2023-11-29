
macro(copy src dst)
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${src} ${dst}
        RESULT_VARIABLE ret
        OUTPUT_VARIABLE out
        ERROR_VARIABLE  err
    )
    if(ret)
        message(FATAL_ERROR "ERROR ---> could not be copied: ${err}")
    endif()
endmacro()

cmake_minimum_required(VERSION 3.9)
cmake_policy(VERSION 3.9)

project(clastwindow VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Python libs
find_package (Python3 COMPONENTS Interpreter Development)
include_directories(Python3_INCLUDE_DIRS)
#

add_subdirectory(libs/wxWidgets)

set(wxWidgets_USE_LIBS base core net)

add_executable(clastwindow ./module.cpp)


target_link_libraries(clastwindow Python3::Python)

target_link_libraries(clastwindow wx::net wx::core wx::base)

enable_testing()

add_subdirectory(tests/py_executor_tests)
copy(${PROJECT_SOURCE_DIR}/libs/python311.dll ${PROJECT_BINARY_DIR}/python311.dll)

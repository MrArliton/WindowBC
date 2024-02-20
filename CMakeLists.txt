
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

macro(copy_dir src dst)
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${src} ${dst}
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

# DEV options 
set(ENABLE_TESTS OFF)


add_subdirectory(libs/wxWidgets)

set(wxWidgets_USE_LIBS base core net)

add_executable(clastwindow ./module.cpp)

target_link_libraries(clastwindow wx::net wx::core wx::base)

if(ENABLE_TESTS)
    enable_testing()

    add_executable(NowTesting src/tests/NowTesting.cpp)
    add_test(NAME NowTesting COMMAND "./NowTesting")
endif()

    
copy_dir(${PROJECT_SOURCE_DIR}/scripts ${PROJECT_BINARY_DIR}/scripts)
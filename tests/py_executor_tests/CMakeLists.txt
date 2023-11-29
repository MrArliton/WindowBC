macro(copy_directory src dst)
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

project(py_exec_test)

#Python libs
find_package (Python3 COMPONENTS Interpreter Development)
include_directories(Python3_INCLUDE_DIRS)
#


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(py_exec_test ./py_executor_test.cpp)


target_link_libraries(py_exec_test Python3::Python)

#Prepary for testing
copy_directory(${CMAKE_SOURCE_DIR}/tests/tests_files ${CMAKE_BINARY_DIR}/test_files)

add_test(NAME py_exec_test
         COMMAND py_exec_test)


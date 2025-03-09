cmake_minimum_required(VERSION 3.28)

# Project
if(DEFINED input_handling_GUARD)
    if(NOT input_handling_GUARD STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
        return() # Project has been already included by someone else
    endif()
else()
    set(input_handling_GUARD ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "input_handling guard")
endif()
project(input_handling
    VERSION 0.3.0
    LANGUAGES CXX
)

# C/C++ standard.
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Multithreaded build.
set(CMAKE_BUILD_PARALLEL_LEVEL 16)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# Static library build.
add_library(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input_codes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input_handling_public.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input_ifc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input_state_set.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input_handling.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input_handling.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input_keyboard_mouse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input_reporting.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${cglm_INCLUDE_DIR}
)

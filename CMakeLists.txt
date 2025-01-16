cmake_minimum_required(VERSION 3.20)
project(
    nw
    VERSION 0.1.0
    LANGUAGES CXX)

set(CMAKE_CXX_CLANG_TIDY clang-tidy)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(DCMAKE_GENERATOR_PLATFORM "x64")

set(NW_OS AUTO CACHE STRING "AUTO WINDOWS LINUX MACOS")
set_property(
    CACHE
    NW_OS PROPERTY
    STRINGS AUTO WINDOWS LINUX MACOS
)

if (NW_OS STREQUAL "AUTO")
    if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        set(NW_OS "WINDOWS")
    elseif (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
        set(NW_OS "LINUX")
    elseif (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
        set(NW_OS "MACOS")
    endif()
endif()

if (NW_OS STREQUAL "WINDOWS")
    set(NW_PATH "Windows")
elseif (NW_OS STREQUAL "LINUX")
    set(NW_PATH "linux")
elseif (NW_OS STREQUAL "MACOS")
    set(NW_PATH "macOS")
else()
    message(SEND_ERROR "Can't find matched NW_OS")
endif()

file(GLOB_RECURSE FILE_SOURCES RELATIVE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nw/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nw/${NW_PATH}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/${NW_PATH}/*.cpp
)

add_library(
    ${PROJECT_NAME}
    ${FILE_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set_target_properties(nw PROPERTIES
        VS_GLOBAL_RunCodeAnalysis true
        VS_GLOBAL_EnableMicrosoftCodeAnalysis true
        VS_GLOBAL_EnableClangTidyCodeAnalysis true
    )
endif()
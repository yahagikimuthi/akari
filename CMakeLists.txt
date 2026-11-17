cmake_minimum_required(VERSION 3.30)

project(akr
    LANGUAGES CXX
    VERSION 2.0.0
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(akr INTERFACE)
add_library(akr::akr ALIAS akr)

target_include_directories(akr INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if(PROJECT_IS_TOP_LEVEL)
    add_library(project_warnings INTERFACE)
    target_compile_options(project_warnings INTERFACE
        -Wall
        -Werror
        -Wextra
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wpedantic
        -Wunreachable-code
        -Wimplicit-fallthrough
        -Wnoexcept
        -fcf-protection
        -mtune=native
        -march=native
        -flto
        -fcontracts
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Debug>:-D_GLIBCXX_DEBUG>
        $<$<CONFIG:Debug>:-D_GLIBCXX_ASSERTIONS>
        $<$<CONFIG:Debug>:-D_FORTIFY_SOURCE=2>
        $<$<CONFIG:Debug>:-fstack-protector-strong>
        $<$<CONFIG:Debug>:-fstack-clash-protection>
        $<$<CONFIG:Debug>:-fsanitize=address,undefined,float-divide-by-zero,float-cast-overflow>
        $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
    )
        target_link_options(project_warnings INTERFACE
        -fcontracts
        $<$<CONFIG:Debug>:-fsanitize=address,undefined,float-divide-by-zero,float-cast-overflow>
    )

    file(GLOB_RECURSE MAIN_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

    add_executable(main.exe)
    target_sources(main.exe PRIVATE ${MAIN_SOURCES})
    target_link_libraries(main.exe PRIVATE
        akr
        project_warnings
    )
    target_include_directories(main.exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()
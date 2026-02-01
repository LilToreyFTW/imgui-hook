cmake_minimum_required(VERSION 3.14)
project(Loader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
add_executable(Loader WIN32 main.cpp)
target_link_libraries(Loader PRIVATE Shlwapi)

if(MSVC)
    set_target_properties(Loader PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
        WIN32_EXECUTABLE TRUE
    )
endif()

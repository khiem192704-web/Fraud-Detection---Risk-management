cmake_minimum_required(VERSION 3.15)
project(epfd_ras VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(include)

# Core Library Sources (excluding main executables and Qt GUI)
file(GLOB_RECURSE CORE_SRCS
    "sources/core/*.cpp"
    "sources/engine/*.cpp"
    "sources/dal/*.cpp"
    "sources/bll/*.cpp"
    "sources/ui/ConsoleUI.cpp"
)

add_library(epfd_core STATIC ${CORE_SRCS})
target_include_directories(epfd_core PUBLIC include)

# Main Console Executable
add_executable(epfd_app sources/main.cpp)
target_link_libraries(epfd_app PRIVATE epfd_core)

# Unit Test Executable
enable_testing()
add_executable(epfd_tests tests/test_rules_step_by_step.cpp)
target_include_directories(epfd_tests PRIVATE include tests)
target_link_libraries(epfd_tests PRIVATE epfd_core)
add_test(NAME AllTests COMMAND epfd_tests)

# Qt GUI Executable
find_package(Qt6 COMPONENTS Widgets Core Gui QUIET)
if (Qt6_FOUND)
    message(STATUS "Found Qt6 (${Qt6_VERSION}). Configuring epfd_gui target.")
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
    add_executable(epfd_gui 
        sources/ui/main_gui.cpp 
        sources/ui/MainWindow.cpp 
        include/ui/MainWindow.h
        sources/ui/LoginDialog.cpp
        include/ui/LoginDialog.h
    )
    set_target_properties(epfd_gui PROPERTIES AUTOMOC ON)
    target_include_directories(epfd_gui PRIVATE include)
    target_link_libraries(epfd_gui PRIVATE epfd_core Qt6::Widgets Qt6::Core Qt6::Gui)
endif()

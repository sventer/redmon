cmake_minimum_required(VERSION 2.8)

project(RedMon)

set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets Network Xml REQUIRED)

file(GLOB_RECURSE redmon_SOURCE_FILES "src/*.cpp" "src/*.h")

include_directories("src")

add_executable("RedMon" WIN32 MACOSX_BUNDLE ${redmon_SOURCE_FILES})
target_link_libraries("RedMon" Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Xml)

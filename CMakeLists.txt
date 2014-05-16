cmake_minimum_required(VERSION 2.8.11)

project(RedMon)

set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets Network Xml REQUIRED)

file(GLOB_RECURSE redmon_SOURCE_FILES "src/*.cpp" "src/*.h")

include_directories("src")

add_executable("RedMon" WIN32 MACOSX_BUNDLE ${redmon_SOURCE_FILES})
target_link_libraries("RedMon"
  ${Qt5Core_LIBRARIES}
  ${Qt5Gui_LIBRARIES}
  ${Qt5Widgets_LIBRARIES}
  ${Qt5Network_LIBRARIES}
  ${Qt5Xml_LIBRARIES}
)

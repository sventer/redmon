cmake_minimum_required(VERSION 2.8.11)

project(RedMon)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_LIBRARY_PATH "C:\\Program Files (x86)\\Windows Kits\\8.0\\Lib\\win8\\um\\x64")

find_package(Qt5 COMPONENTS Core Gui Widgets Network Xml REQUIRED)

file(GLOB_RECURSE redmon_SOURCE_FILES "src/*.cpp" "src/*.h")

if(APPLE)
  list(APPEND redmon_SOURCE_FILES "resources/redmon.icns")
  set_source_files_properties("resources/redmon.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  set("MACOSX_BUNDLE_ICON_FILE" "redmon.icns")
endif(APPLE)

include_directories("src")

add_executable("RedMon" WIN32 MACOSX_BUNDLE ${redmon_SOURCE_FILES})
target_link_libraries("RedMon"
  ${Qt5Core_LIBRARIES}
  ${Qt5Gui_LIBRARIES}
  ${Qt5Widgets_LIBRARIES}
  ${Qt5Network_LIBRARIES}
  ${Qt5Xml_LIBRARIES}
)

if(APPLE)
  set_target_properties("RedMon" PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST
    "${PROJECT_SOURCE_DIR}/cmake/MacOSXBundleInfo.plist.in"
  )
endif()

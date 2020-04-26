cmake_minimum_required(VERSION 3.16)

project(json_tool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Widgets Gui WebEngineWidgets LinguistTools REQUIRED)

set(qt_path "C:/Qt/Qt5.12.4/5.12.4/msvc2017/include")
include_directories(${qt_path}/QtWebEngineWidgets)

file(GLOB head_files ./*.h)
source_group("Header Files" FILES ${head_files})

file(GLOB sour_files ./*.cpp)
source_group("Source Files" FILES ${sour_files})

file(GLOB ui_files ./*.ui)
source_group("Form Files" FILES ${ui_files})

file(GLOB qrc_files ./*.qrc)
source_group("Resource Files" FILES ${qrc_files})

file(GLOB bmp_files ./Resources/*.bmp ./Resources/*.ico)

set(src_list
 ${head_files}
 ${sour_files}
 ${ui_files}
 ${qrc_files}
 
 ./json_tool.rc
 ${bmp_files}
 )

set(executeable_output_path ${PROJECT_SOURCE_DIR}/build/bin)
add_executable(${PROJECT_NAME} ${src_list})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Gui Qt5::WebEngineWidgets)
set_Target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

cmake_minimum_required(VERSION 3.27.5)

# set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")

get_filename_component(current_dir "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "" current_dir "${current_dir}")
string(REPLACE "-" "_" current_dir "${current_dir}")
message("CMake start \n\t directory name: ${current_dir}")

set(CMAKE_VERBOSE_MAKEFILE ON)
#set(fmt_DIR "C:/PCL-1.14.1/3rdParty/fmt-10.1.1-lib")

add_subdirectory("C:/PCL-1.14.1/3rdParty" 3rdParty.out)

#set(current_dir "${current_dir}_${CMAKE_BUILD_TYPE}")

project("${current_dir}" VERSION 0.1 LANGUAGES CXX)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick Widgets Svg)

qt_standard_project_setup(REQUIRES 6.5)

file(GLOB ${PROJECT_NAME_UPPER}_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB ${PROJECT_NAME_UPPER}_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

find_package(OpenCV REQUIRED)
#find_package(fmt)

qt_add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME_UPPER}_SOURCES}
    ${${PROJECT_NAME_UPPER}_HEADERS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Quick
    Qt6::Widgets
    Qt6::Svg
#    fmt::fmt
    ${OpenCV_LIBS}
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    ${Eigen_DIR}
)

get_target_property(target_RUNTIME_location ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
message("\t${PROJECT_NAME} target_RUNTIME_location: ${target_RUNTIME_location}")
get_target_property(target_ARCHIVE_location ${PROJECT_NAME} ARCHIVE_OUTPUT_DIRECTORY)
message("\t${PROJECT_NAME} target_ARCHIVE_location: ${target_ARCHIVE_location}")
get_target_property(target_LIBRARY_location ${PROJECT_NAME} LIBRARY_OUTPUT_DIRECTORY)
message("\t${PROJECT_NAME} target_LIBRARY_location: ${target_LIBRARY_location}")

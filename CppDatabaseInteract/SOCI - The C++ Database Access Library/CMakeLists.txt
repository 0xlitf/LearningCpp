cmake_minimum_required(VERSION 3.27.5)

get_filename_component(current_dir "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "" current_dir "${current_dir}")
string(REPLACE "-" "_" current_dir "${current_dir}")

set(CMAKE_VERBOSE_MAKEFILE ON)
message("CMake start \n\t directory name: ${current_dir}")

project("${current_dir}" VERSION 0.1 LANGUAGES CXX)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("C:/Work/wucon-windows/3rdparty" 3rdParty.out)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# set(Boost_DIR "C:/PCL-1.14.1/3rdParty/Boost" CACHE STRING "")
# MESSAGE( STATUS "SOCI Boost_DIR:${Boost_DIR}")
# list(APPEND CMAKE_PREFIX_PATH ${Boost_DIR})

find_package(Boost 1.84 REQUIRED)
find_package(SOCI REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    ${sqlite3_DIR}/sqlite3.lib
    SOCI::soci_core
    SOCI::soci_sqlite3
    ${Boost_LIBRARIES}
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${sqlite3_DIR}
    ${Boost_INCLUDE_DIRS}
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

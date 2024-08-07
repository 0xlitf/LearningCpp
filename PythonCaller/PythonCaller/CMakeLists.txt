cmake_minimum_required(VERSION 3.27.5)

get_filename_component(current_dir "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "" current_dir "${current_dir}")
string(REPLACE "-" "_" current_dir "${current_dir}")
message("CMake start \n\t directory name: ${current_dir}")

set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory("C:/PCL-1.14.1/3rdParty" 3rdParty.out)

project("${current_dir}" VERSION 0.1 LANGUAGES CXX)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup(REQUIRES 6.5)

file(GLOB ${PROJECT_NAME_UPPER}_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB ${PROJECT_NAME_UPPER}_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

set(CUSTOM_LIB_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_lib)
set(CUSTOM_BIN_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_bin)
if(${CMAKE_GENERATOR} MATCHES "Visual Studio*")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CUSTOM_LIB_DIR}/${CMAKE_SYSTEM_NAME})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CUSTOM_LIB_DIR}/${CMAKE_SYSTEM_NAME})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CUSTOM_BIN_DIR}/${CMAKE_SYSTEM_NAME})

    set(EXACT_OUTPUT_DIRECTORY ${CUSTOM_BIN_DIR}/${CMAKE_SYSTEM_NAME})
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CUSTOM_LIB_DIR}/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CUSTOM_LIB_DIR}/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CUSTOM_BIN_DIR}/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})

    set(EXACT_OUTPUT_DIRECTORY ${CUSTOM_BIN_DIR}/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})
endif()

message(STATUS "***** EXACT_OUTPUT_DIRECTORY: ${EXACT_OUTPUT_DIRECTORY} *****")

qt_add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME_UPPER}_SOURCES}
    ${${PROJECT_NAME_UPPER}_HEADERS}
    PyFunc.py
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(${PROJECT_NAME} PRIVATE debug
        Qt6::Core
        Qt6::Widgets
        C:/Python312/libs/python3_d.lib
        C:/Python312/libs/python312_d.lib
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(${PROJECT_NAME} PRIVATE optimized
        Qt6::Core
        Qt6::Widgets
        C:/Python312/libs/python3.lib
        C:/Python312/libs/python312.lib
    )
endif()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    C:/Python312/include
)

get_target_property(target_RUNTIME_location ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
message("\t${PROJECT_NAME} target_RUNTIME_location: ${target_RUNTIME_location}")
get_target_property(target_ARCHIVE_location ${PROJECT_NAME} ARCHIVE_OUTPUT_DIRECTORY)
message("\t${PROJECT_NAME} target_ARCHIVE_location: ${target_ARCHIVE_location}")
get_target_property(target_LIBRARY_location ${PROJECT_NAME} LIBRARY_OUTPUT_DIRECTORY)
message("\t${PROJECT_NAME} target_LIBRARY_location: ${target_LIBRARY_location}")

set(pymodule PyFunc.py)
if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/${pymodule})
    message(STATUS "***** pymodule: ${pymodule} *****")
    if(${CMAKE_GENERATOR} MATCHES "Visual Studio*")
        file(COPY ${CMAKE_CURRENT_LIST_DIR}/${pymodule}
             DESTINATION ${EXACT_OUTPUT_DIRECTORY}/Debug)
        file(COPY ${CMAKE_CURRENT_LIST_DIR}/${pymodule}
             DESTINATION ${EXACT_OUTPUT_DIRECTORY}/Release)
    else()
        file(COPY ${CMAKE_CURRENT_LIST_DIR}/${pymodule}
             DESTINATION ${EXACT_OUTPUT_DIRECTORY}/)
    endif()

else()
    message(STATUS "***** pymodule not exist *****")
endif()

cmake_minimum_required(VERSION 3.27.5)

get_filename_component(current_dir "${CMAKE_CURRENT_LIST_DIR}" NAME)
string(REPLACE " " "" current_dir "${current_dir}")
string(REPLACE "-" "_" current_dir "${current_dir}")
message(STATUS "CMake start \n\t directory name: ${current_dir}")

set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory("C:/PCL-1.14.1/3rdParty" 3rdParty.out)
add_subdirectory(${QtWrapper_DIR} QtWrapper.out)

project("${current_dir}" VERSION 0.1 LANGUAGES C CXX)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

if(CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_BUILD_POSTFIX "${CMAKE_RELEASE_POSTFIX}")
    set(PROJECT_BUILD_TYPE release)
elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
    set(CMAKE_BUILD_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}")
    set(PROJECT_BUILD_TYPE unknow)
    set(PROJECT_BUILD_TYPE release)
elseif(CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
    set(CMAKE_BUILD_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}")
    set(PROJECT_BUILD_TYPE release)
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_BUILD_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
    set(PROJECT_BUILD_TYPE debug)
else()
    set(CMAKE_BUILD_POSTFIX "")
    set(PROJECT_BUILD_TYPE unknow)
endif()

#if(MSVC)
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
#endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(add_subDirs)
    message(STATUS "function(add_subDirs)")

    file(GLOB subdirectories LIST_DIRECTORIES true "${CMAKE_CURRENT_LIST_DIR}/*")

    set(file_to_find "CMakeLists.txt")
    foreach(subdir ${subdirectories})
        if(IS_DIRECTORY "${subdir}")
            # message(STATUS "Found subdir: ${subdir}")

            set(files)

            file(GLOB files_in_directory LIST_FILES "${subdir}/*")
            # message(STATUS "files_in_directory: ${files_in_directory}")

            foreach(fin ${files_in_directory})
                get_filename_component(file "${fin}" NAME ABSOLUTE)
                list(APPEND files ${file})
            endforeach()

            list(FIND files "${file_to_find}" file_index)

            if(${file_index} EQUAL -1)
                message(STATUS "${file_to_find} not found in the directory: ${subdir}")
            else()
                message(STATUS "${file_to_find} found in the directory: ${subdir}")
                add_subdirectory("${subdir}")
            endif()

        endif()
    endforeach()

endfunction()

function(print_subDirs)
    get_property(inited_subdirectories DIRECTORY ${CMAKE_CURRENT_LIST_DIR} PROPERTY SUBDIRECTORIES)

    message(STATUS "\n inited subdirectory: \n")
    foreach(subdir ${inited_subdirectories})
        message(STATUS "\t\t${subdir}")
    endforeach()

    message(STATUS "\n\n")
endfunction()

# if (NOT EXISTS ${CMAKE_BUILD_TYPE})
#     set(CMAKE_BUILD_TYPE "Debug")
# else()
# endif()

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

#if(CMAKE_BUILD_TYPE MATCHES "Debug")
#
#endif()
#if(CMAKE_BUILD_TYPE MATCHES "Release")
#
#endif()

#if(CMAKE_BUILD_TYPE MATCHES "Release")
    set(deployfile windeployqt6.bat)
    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/${deployfile})
        message(STATUS "***** deployfile: ${deployfile} *****")
        if(${CMAKE_GENERATOR} MATCHES "Visual Studio*")
            file(COPY ${CMAKE_CURRENT_LIST_DIR}/${deployfile}
                 DESTINATION ${EXACT_OUTPUT_DIRECTORY}/Debug)
            file(COPY ${CMAKE_CURRENT_LIST_DIR}/${deployfile}
                 DESTINATION ${EXACT_OUTPUT_DIRECTORY}/Release)
        else()
            file(COPY ${CMAKE_CURRENT_LIST_DIR}/${deployfile}
                 DESTINATION ${EXACT_OUTPUT_DIRECTORY}/)
        endif()

    else()
        message(STATUS "***** deployfile not exist *****")
    endif()
#endif()

set(BINARY_DIR ${CMAKE_BINARY_DIR})

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})
message(STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})

file(GLOB subdirectories LIST_DIRECTORIES true "${CMAKE_CURRENT_LIST_DIR}/*")

print_subDirs()

add_subDirs()

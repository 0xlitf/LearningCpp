cmake_minimum_required(VERSION 3.27.4)

get_filename_component(current_dir "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "" current_dir "${current_dir}")
string(REPLACE "-" "_" current_dir "${current_dir}")

#set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory("C:/PCL 1.13.1/3rdParty" 3rdParty.out)

message("CMAKE_BUILD_TYPE \n\t : ${CMAKE_BUILD_TYPE}")

message("CMake start \n\t directory name: ${current_dir}")

project("${current_dir}" VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(add_subDirs)
    message("function(add_subDirs)")

    file(GLOB subdirectories LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/*")

    set(file_to_find "CMakeLists.txt")
    foreach(subdir ${subdirectories})
        if(IS_DIRECTORY "${subdir}")
            # message("Found subdir: ${subdir}")

            set(files)

            file(GLOB files_in_directory LIST_FILES "${subdir}/*")
            # message("files_in_directory: ${files_in_directory}")

            foreach(fin ${files_in_directory})
                get_filename_component(file "${fin}" NAME ABSOLUTE)
                list(APPEND files ${file})
            endforeach()

            list(FIND files "${file_to_find}" file_index)

            if(${file_index} EQUAL -1)
                message("${file_to_find} not found in the directory: ${subdir}")
            else()
                message("${file_to_find} found in the directory: ${subdir}")
                add_subdirectory("${subdir}")
            endif()

        endif()
    endforeach()

endfunction()

function(print_subDirs)
    get_property(inited_subdirectories DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY SUBDIRECTORIES)

    message("\n inited subdirectory: \n")
    foreach(subdir ${inited_subdirectories})
        message("\t\t${subdir}")
    endforeach()

    message("\n\n")
endfunction()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subDirs()
print_subDirs()

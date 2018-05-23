cmake_minimum_required(VERSION 3.8)
project(projet-info)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")


file(GLOB_RECURSE SOURCE_FILES src/*)

message(STATUS ${SOURCE_FILES})

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_HOME_DIRECTORY}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            )
endif(DOXYGEN_FOUND)

add_executable(projet-info ${SOURCE_FILES})
target_link_libraries(projet-info stdc++fs)
target_link_libraries(projet-info pthread)
target_link_libraries(projet-info X11)
target_link_libraries(projet-info m)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin/${CMAKE_BUILD_TYPE})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})


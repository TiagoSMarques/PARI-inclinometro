#
cmake_minimum_required (VERSION 2.8)
project (ProjetoFinal)
set(PROG btcom)


include_directories(${GTK_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/srcutils) 
include_directories(${PROJECT_SOURCE_DIR}/srcutils/img)
#--------------------------------------
set(SOURCES src/myf.c src/main.c src/callbacks.c)
#set(SOURCES src/server_test.c )
add_executable(${PROG} ${SOURCES})
set( CMAKE_CXX_FLAGS "-rdynamic")
#-----------------------------------------------
#The recommended way to find packages automatically using PkgConfig
#See more on http://www.cmake.org/Wiki/CMake:How_To_Find_Libraries
find_package(PkgConfig REQUIRED)  #necessary for next command to work
pkg_check_modules(GTK REQUIRED gtk+-3.0)

link_directories(${GTK_LIBRARY_DIRS})
target_link_libraries(${PROG} ${GTK_LIBRARIES} bluetooth m)

#-----------------------------------------------------
# --- add custom mtargets (and dependencies)
#add_custom_target( cpfiles
#                 "${PROJECT_SOURCE_DIR}/src/*.glade" "${PROJECT_BINARY_DIR}"
#                COMMAND cp "${PROJECT_SOURCE_DIR}/src/*.png" "${PROJECT_BINARY_DIR}"
#                 )

#Add dependencies
#add_dependencies(${PROG}) #cpfiles

#-------- Adições à CMakeLists.txt existente ----
# The project version number. Used in documentation generation and similar.
# Optional but recommended
set(VERSION_MAJOR   1   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   0   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

#-- add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install doc (requires Doxygen)" ${DOXYGEN_FOUND})
if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()
    set(doxyfile_in "${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in" )
    set(doxyfile "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    set(doxy_main_page "${CMAKE_CURRENT_SOURCE_DIR}/doc/mainpage.dox")
    configure_file(${doxyfile_in} ${doxyfile} @ONLY)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()
#code inspired on: http://www.tty1.net/blog/2014/cmake-doxygen_en.html
#---------------------------------------------------

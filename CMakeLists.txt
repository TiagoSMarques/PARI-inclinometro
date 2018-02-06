#
cmake_minimum_required (VERSION 2.8)
project (ProjetoFinal)
set(PROG btcom)


include_directories(${GTK_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/srcutils) 
include_directories(${PROJECT_SOURCE_DIR}/srcutils/img)
#--------------------------------------
set(SOURCES src/myf.c src/main.c)
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

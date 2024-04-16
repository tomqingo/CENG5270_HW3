#########
# Setup #
#########

# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8)

# Message
message(STATUS "hw3")
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS ${PROJECT_SOURCE_DIR})

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Project's name
project(hw3)

# Set some paths
set(PATH_PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_COMPILER ${CMAKE_CXX_COMPILER}) # use g++ for .c files (flute)

set(PATH_ICCAD19 ${CMAKE_CURRENT_SOURCE_DIR})

###################
# Warnings/Errors #
###################

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror=return-type")

######################
# Check Dependencies #
######################
# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_USE_STATIC_LIBS ON)
# find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
# find_package(Threads)

###############
# Source Code #
###############

file(GLOB_RECURSE TARGET_H "${PATH_ICCAD19}/*.h")
file(GLOB_RECURSE SRC_FILES_ICCAD19 ${PATH_ICCAD19}/*.cpp)

set(SRC_FILES ${SRC_FILES_ICCAD19} ${TARGET_H})

###########
# Targets #
###########

add_executable(hw3 ${SRC_FILES} ${SRC_FILES_FLUTE})
message(STATUS "hw3 built")

#######################
# Include Directories #
#######################

include_directories(${PATH_ICCAD19})

#################
# Linker Flags  #
#################

# TODO: set static under release mode only
# set_target_properties(HierPR PROPERTIES LINK_FLAGS "-Wl,--whole-archive -rdynamic -ldl -lpthread -Wl,--no-whole-archive")
# set_target_properties(HierPR PROPERTIES LINK_FLAGS "-static -Wl,--whole-archive -rdynamic -ldl -lpthread -Wl,--no-whole-archive")
set_target_properties(hw3 PROPERTIES LINK_FLAGS "-Wl,--whole-archive -rdynamic -ldl -lpthread -Wl,--no-whole-archive")

target_link_libraries(hw3 ${Boost_LIBRARIES} pthread)
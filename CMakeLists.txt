cmake_minimum_required(VERSION 3.7)
project(RPiDrivers)

set(CMAKE_CXX_STANDARD 11)

# Enable PiGPIO if installed
set(PIGPIO 1)

######################################Include everuthing src##################################
file(GLOB_RECURSE Proj_SOURCES "src/*.cpp" "src/*.c" "include/*.c" "include/*.cpp")
file(GLOB_RECURSE Proj_HEADERS "src/*.h" "src/*.hpp")

set( Proj_INCLUDE_DIRS "include/" "include/drivers/")
foreach (_headerFile ${Proj_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND Proj_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES Proj_INCLUDE_DIRS)
include_directories(${Proj_INCLUDE_DIRS})
link_directories(${Proj_INCLUDE_DIRS})
##############################################################################################

add_executable (${PROJECT_NAME} ${Proj_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Proj_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} pthread)

#Enable pigpio if on RPi
if(PIGPIO)
    add_definitions(-DPIGPIO=1)
    target_link_libraries(${PROJECT_NAME} pigpio)
    target_link_libraries(${PROJECT_NAME} rt)
endif()
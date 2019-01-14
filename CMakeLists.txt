cmake_minimum_required(VERSION 3.7)
project(RPiDrivers)

set(CMAKE_CXX_STANDARD 11)

# Enable PiGPIO if installed
set(PIGPIO 1)

######################################Include everuthing src##################################
file(GLOB_RECURSE RPiFirmware_SOURCES "src/*.cpp")
file(GLOB_RECURSE RPiFirmware_HEADERS "src/*.h")

set (RPiFirmware_INCLUDE_DIRS "include/")
foreach (_headerFile ${RPiFirmware_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND RPiFirmware_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES RPiFirmware_INCLUDE_DIRS)
##############################################################################################

add_executable (RPiFirmwareExe ${RPiFirmware_SOURCES})
target_include_directories(RPiFirmwareExe PRIVATE ${RPiFirmware_INCLUDE_DIRS})

target_link_libraries(RPiFirmwareExe pthread)

#Enable pigpio if on RPi
if(PIGPIO)
    add_definitions(-DPIGPIO=1)
    target_link_libraries(RPiFirmwareExe pigpio)
    target_link_libraries(RPiFirmwareExe rt)
endif()
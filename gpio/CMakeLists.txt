#-
# $Copyright$
#
cmake_minimum_required(VERSION 2.8)
project(phisch)

get_property(GPIO_ROOT GLOBAL PROPERTY GPIO_ROOT)
message(STATUS "Building libgpio at ${GPIO_ROOT}")

#*******************************************************************************
# GPIO Library
#*******************************************************************************
set(TARGET_NAME "gpio")
add_library(${TARGET_NAME} INTERFACE)
target_include_directories(${TARGET_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(${TARGET_NAME} INTERFACE
    cmsis
    stm32
)

#*******************************************************************************
# Testcases
#*******************************************************************************
if (UNITTEST)
    MESSAGE(STATUS "Building libgpio Testcases")
    add_subdirectory(testcases)
endif()

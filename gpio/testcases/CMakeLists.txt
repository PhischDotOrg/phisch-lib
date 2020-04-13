#-
# $Copyright$
#
cmake_minimum_required(VERSION 2.8)
project(phisch)

#*******************************************************************************
# Testcases Requirements
#*******************************************************************************
get_property(GMOCK_FOUND GLOBAL PROPERTY GMOCK_FOUND)
if(NOT GMOCK_FOUND)
    message(FATAL_ERROR "The libgpio Testcases cannot be build without GMock!")
endif(NOT GMOCK_FOUND)

get_property(GMOCK_ROOT GLOBAL PROPERTY GMOCK_ROOT)
include_directories("${GMOCK_ROOT}/include")

get_property(GTEST_ROOT GLOBAL PROPERTY GTEST_ROOT)
include_directories("${GTEST_ROOT}/include")

get_property(GPIO_ROOT GLOBAL PROPERTY GPIO_ROOT)
include_directories("${GPIO_ROOT}")
include_directories("${GPIO_ROOT}/include")

#*******************************************************************************
# Compile Environment
#*******************************************************************************
add_definitions("-DGPIO_ENV=GTest")

#*******************************************************************************
# Testcases
#*******************************************************************************
set(COMPONENTS
    GpioAccessVia74x595
    GpioAccessViaLpt
    GpioEngine
    GpioPin
)

foreach(COMPONENT IN LISTS COMPONENTS)
    message(STATUS "Adding Tests for Component '${COMPONENT}'")

    set(COMPONENT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../${COMPONENT}.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../${COMPONENT}.cpp
        test_${COMPONENT}.cpp
    )

    set(COMPONENT_TESTRUNNER gpio_${COMPONENT}_tests)

    add_executable(${COMPONENT_TESTRUNNER}
        ${COMPONENT_SOURCES}
    )
    target_link_libraries(${COMPONENT_TESTRUNNER}
        gmock
        gmock_main
    )
    add_test(NAME devices_${COMPONENT} COMMAND ${COMPONENT_TESTRUNNER})
endforeach()

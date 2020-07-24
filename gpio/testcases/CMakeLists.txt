#-
# $Copyright$
#
cmake_minimum_required(VERSION 2.8)
project(phisch)

#*******************************************************************************
# Compile Environment
#*******************************************************************************
remove_definitions("-DGPIO_ENV=STM32")
add_definitions("-DGPIO_ENV=GTest")

add_definitions("-DUNITTEST")

#*******************************************************************************
# Testcases
#*******************************************************************************
set(COMPONENTS
    # GpioAccessVia74x595
    # GpioAccessViaLpt
    GpioEngine
    GpioPin
)

foreach(COMPONENT IN LISTS COMPONENTS)
    message(STATUS "Adding Tests for Component '${COMPONENT}'")

    set(COMPONENT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../${COMPONENT}.hpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/../${COMPONENT}.cpp
        test_${COMPONENT}.cpp
    )

    set(COMPONENT_TESTRUNNER gpio_${COMPONENT}_tests)

    add_executable(${COMPONENT_TESTRUNNER}
        ${COMPONENT_SOURCES}
    )
    target_link_libraries(${COMPONENT_TESTRUNNER}
        gpio
        gmock
        gmock_main
    )
    add_test(NAME devices_${COMPONENT} COMMAND ${COMPONENT_TESTRUNNER})
endforeach()

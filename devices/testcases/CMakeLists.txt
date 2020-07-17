#-
# $Copyright$
#
cmake_minimum_required(VERSION 2.8)
project(phisch)

add_definitions("-DUNITTEST")

#*******************************************************************************
# Component Testcases
#*******************************************************************************
set(COMPONENTS
    LedMatrixGTM2088RGB
    Sr74x595
    Ws2812bStrip
)

foreach(COMPONENT IN LISTS COMPONENTS)
    message(STATUS "Adding Tests for Component '${COMPONENT}'")

    set(COMPONENT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../${COMPONENT}.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../${COMPONENT}.cpp
        test_${COMPONENT}.cpp
    )

    set(COMPONENT_TESTRUNNER devices_${COMPONENT}_tests)

    add_executable(${COMPONENT_TESTRUNNER}
        ${COMPONENT_SOURCES}
    )
    target_link_libraries(${COMPONENT_TESTRUNNER}
        devices
        gmock
        gmock_main
    )
    add_test(NAME devices_${COMPONENT} COMMAND ${COMPONENT_TESTRUNNER})
endforeach()

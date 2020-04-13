#-
# $Copyright$
#
cmake_minimum_required(VERSION 2.8)
project(phisch)

get_property(UART_ROOT GLOBAL PROPERTY UART_ROOT)
message(STATUS "Building libuart at ${UART_ROOT}")

get_property(PRINTF_ROOT GLOBAL PROPERTY PRINTF_ROOT)

#*******************************************************************************
# UART Library
#*******************************************************************************
set(TARGET_NAME "uart")
set(TARGET_SOURCES
    UartAccess.cpp
    UartDevice.cpp
)
add_library(${TARGET_NAME} INTERFACE)
target_include_directories(${TARGET_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(${TARGET_NAME} INTERFACE
    cmsis
    gpio
    stm32f4
    tpf
)

#*******************************************************************************
# Testcases
#*******************************************************************************
if (GMOCK_FOUND)
    MESSAGE(STATUS "Building libuart Testcases")
    add_subdirectory(testcases)
    target_link_libraries(${TARGET_NAME}
        gmock
    )
endif(GMOCK_FOUND)

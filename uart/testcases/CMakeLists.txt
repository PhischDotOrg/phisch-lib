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
    message(FATAL_ERROR "The libuart Testcases cannot be build without GMock!")
endif(NOT GMOCK_FOUND)

get_property(GMOCK_ROOT GLOBAL PROPERTY GMOCK_ROOT)
include_directories("${GMOCK_ROOT}/include")

get_property(GTEST_ROOT GLOBAL PROPERTY GTEST_ROOT)
include_directories("${GTEST_ROOT}/include")

get_property(UART_ROOT GLOBAL PROPERTY UART_ROOT)
include_directories("${UART_ROOT}/include")

#*******************************************************************************
# Testcases
#*******************************************************************************
set(TC_SOURCES
    test_UartDevice.cpp
)

#*******************************************************************************
# Test Executor
#*******************************************************************************
set(TARGET_EXE test_libs_uart)
add_executable(${TARGET_EXE} main.cpp ${TC_SOURCES})
target_link_libraries(${TARGET_EXE}
    gmock
    uart
)

#*******************************************************************************
# Configure CTest and the CDash Drop Location
#*******************************************************************************
add_test(NAME libuart COMMAND ${TARGET_EXE})


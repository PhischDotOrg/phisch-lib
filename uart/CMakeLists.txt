#-
# $Copyright$
#
message(STATUS "Building libuart at ${CMAKE_CURRENT_SOURCE_DIR}")

#*******************************************************************************
# UART Library
#*******************************************************************************
set(TARGET_NAME "uart")
set(TARGET_SOURCES
    UartDevice.cpp
)
add_library(${TARGET_NAME} INTERFACE)
target_include_directories(${TARGET_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(${TARGET_NAME} INTERFACE
    cmsis
    gpio
    stm32
    tpf
)

#*******************************************************************************
# Testcases
#*******************************************************************************
if (UNITTEST)
    MESSAGE(STATUS "Building libuart Testcases")
    add_subdirectory(testcases)
endif(UNITTEST)

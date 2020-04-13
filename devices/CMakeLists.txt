#-
# $Copyright$
#
cmake_minimum_required(VERSION 2.8)
project(phisch)

get_property(DEVICES_ROOT GLOBAL PROPERTY DEVICES_ROOT)
message(STATUS "Building libdevices at ${DEVICES_ROOT}")

#*******************************************************************************
# Devices Library
#*******************************************************************************
set(TARGET_NAME devices)
set(TARGET_SRC
    FlashViaSTM32F4.cpp
    NvicViaSTM32F4.cpp
    PwrViaSTM32F4.cpp
    RccViaSTM32F4.cpp
    ScbViaSTM32F4.cpp
)
add_library(${TARGET_NAME} ${TARGET_SRC})
target_link_libraries(${TARGET_NAME}
    cmsis
    FreeRTOS
)
target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/devices
)
target_include_directories(${TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#*******************************************************************************
# Testcases
#*******************************************************************************
if (GMOCK_FOUND AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/testcases)
    add_subdirectory(testcases)
endif()

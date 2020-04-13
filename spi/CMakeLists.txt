#-
# $Copyright$
#
cmake_minimum_required(VERSION 2.8)
project(phisch)

get_property(SPI_ROOT GLOBAL PROPERTY SPI_ROOT)
message(STATUS "Building libspi at ${SPI_ROOT}")

#*******************************************************************************
# SPI Library
#*******************************************************************************
set(TARGET_NAME "spi")
set(TARGET_SOURCES
    SpiDevice.cpp
    # SpiTransfer.cpp
)
add_library(${TARGET_NAME} ${TARGET_SOURCES})
target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/spi
)
target_include_directories(${TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${TARGET_NAME}
    gpio
)

#*******************************************************************************
# Testcases
#*******************************************************************************
if (GMOCK_FOUND)
    MESSAGE(STATUS "Building libspi Testcases")
    add_subdirectory(testcases)
        target_link_libraries(${SPI_LIB}
        gmock
    )
endif(GMOCK_FOUND)

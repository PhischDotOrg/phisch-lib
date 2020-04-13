#-
# $Copyright$
#
cmake_minimum_required(VERSION 2.8)
project(phisch)

get_property(DMA_ROOT GLOBAL PROPERTY DMA_ROOT)
message(STATUS "Building libdma at ${DMA_ROOT}")

#*******************************************************************************
# Direct Memory Access (DMA) Library
#*******************************************************************************
set(TARGET_NAME dma)
set(TARGET_SRC
)
if (TARGET_SRC)
    add_library(${TARGET_NAME} ${TARGET_SRC})
endif()

#*******************************************************************************
# Testcases
#*******************************************************************************
if (GMOCK_FOUND AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/testcases)
    add_subdirectory(testcases)
endif()

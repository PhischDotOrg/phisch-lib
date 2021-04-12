#-
# $Copyright$
#
get_property(DEVICES_ROOT GLOBAL PROPERTY DEVICES_ROOT)
message(STATUS "Building libdevices at ${DEVICES_ROOT}")

#*******************************************************************************
# Devices Library
#*******************************************************************************
set(TARGET_NAME devices)
add_library(${TARGET_NAME} INTERFACE)
target_link_libraries(${TARGET_NAME} INTERFACE
    cmsis
    FreeRTOS
    spi
)
target_include_directories(${TARGET_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

#*******************************************************************************
# Testcases
#*******************************************************************************
if (UNITTEST)
    MESSAGE(STATUS "Building libdevices Testcases")
    add_subdirectory(testcases)
endif(UNITTEST)

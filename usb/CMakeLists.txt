#-
# $Copyright$
#

#*******************************************************************************
# Universal Serial Bus (USB) Library
#*******************************************************************************
set(TARGET_NAME usb)
set(TARGET_SRC
    UsbControlPipe.cpp
    UsbDevice.cpp
    UsbInterface.cpp
)
add_library(${TARGET_NAME} ${TARGET_SRC})
target_link_libraries(${TARGET_NAME}
    uart
    usb-stm32f4
)
target_include_directories(${TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#*******************************************************************************
# Testcases
#*******************************************************************************
if (UNITTEST AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/testcases)
    MESSAGE(STATUS "Building ${TARGET_NAME} Testcases")
    add_subdirectory(testcases)
endif()

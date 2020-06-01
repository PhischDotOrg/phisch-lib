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

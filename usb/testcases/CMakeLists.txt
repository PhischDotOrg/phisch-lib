#-
# $Copyright$
#

#*******************************************************************************
# Compile Environment
#*******************************************************************************
add_definitions("-DUNITTEST")

#*******************************************************************************
# Testcases
#*******************************************************************************
set(TC_SOURCES
    # main.cpp
    test_UsbCtrlInEndpoint.cpp
)

set(TARGET_EXE test_usb)

if(${HAVE_USB_OTG})
    add_executable(${TARGET_EXE} ${TC_SOURCES})
    target_link_libraries(${TARGET_EXE}
        gmock
        gmock_main
        usb
    )

    add_test(NAME usb COMMAND ${TARGET_EXE})
endif()
